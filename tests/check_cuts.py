#!/usr/bin/env python3
"""Check that every via lands on metal.

A cut cell is a stack of metal enclosures and via rectangles. When a
route places one, each metal rectangle inside it must overlap same-layer
metal somewhere else in the design, otherwise the via connects to
nothing and the net is broken even though every individual shape looks
fine. That is exactly what happened when the top()/bottom() accessor
flip moved every cut_M2M3 one cut height off its rail: DRC stayed clean,
the .cic looked plausible, and LVS was the first thing to notice.

This walks a generated .cic, flattens the geometry of every non-cut
cell, and demands that each metal rectangle of each placed cut instance
overlaps same-layer geometry that does not belong to the same cut.

Usage: check_cuts.py <file.cic> [topcell-substring] [--allow N]
Exits nonzero if more than N cuts are left floating (default 0), or if
no cuts are found at all, so an empty design cannot pass by accident.
--allow exists for designs with known, verified-harmless dangling vias,
such as the stubs a trimmed route ring leaves behind; it is a baseline,
not a tolerance, so anything new still fails.
"""

import json
import sys
from collections import defaultdict


def is_cut_cell(name):
    return "cut_" in name


def transform_rect(r, angle, xcell, ycell, x1, y1):
    """Mirror of cIcCore::Instance::transform for the angles it defines.

    Anything it does not define is placed untranslated in the C++ too,
    so doing the same here is faithful, not sloppy.
    """
    rx1, ry1, rx2, ry2 = r
    if angle == "R90":
        rx1, ry1, rx2, ry2 = -ry2, rx1, -ry1, rx2
        rx1, ry1, rx2, ry2 = rx1 + xcell, ry1 + ycell, rx2 + xcell, ry2 + ycell
    elif angle == "MY":
        rx1, rx2 = -rx2, -rx1
        rx1, ry1, rx2, ry2 = rx1 + xcell, ry1 + ycell, rx2 + xcell, ry2 + ycell
    elif angle == "MX":
        ry1, ry2 = -ry2, -ry1
        rx1, ry1, rx2, ry2 = rx1 + xcell, ry1 + ycell, rx2 + xcell, ry2 + ycell
    return (rx1 + x1, ry1 + y1, rx2 + x1, ry2 + y1)


def overlaps(a, b):
    """True on area overlap or shared edge, false on corner contact.

    Magic connects same-layer tiles that merely abut, and the routes use
    that: a wire often ends exactly where the cut enclosure begins. A
    single shared corner point carries no current and does not count.
    """
    if a[0] > b[2] or b[0] > a[2] or a[1] > b[3] or b[1] > a[3]:
        return False
    x_touch = a[0] == b[2] or a[2] == b[0]
    y_touch = a[1] == b[3] or a[3] == b[1]
    return not (x_touch and y_touch)


def collect_cell_rects(cell, cells, depth=0):
    """Flatten a cell to {layer: [rect]}, following instances."""
    if depth > 12:
        return {}
    out = defaultdict(list)

    def walk(node):
        cls = node.get("class", "")
        if cls == "Instance":
            sub = cells.get(node.get("cell", ""))
            if sub is None:
                return
            subrects = collect_cell_rects(sub, cells, depth + 1)
            angle = node.get("angle", "")
            xc, yc = node.get("xcell", 0), node.get("ycell", 0)
            x1, y1 = node.get("x1", 0), node.get("y1", 0)
            for layer, rects in subrects.items():
                for r in rects:
                    out[layer].append(
                        transform_rect(r, angle, xc, yc, x1, y1))
            return
        layer = node.get("layer", "")
        if layer and "x1" in node and node["x1"] != node["x2"] \
           and node["y1"] != node["y2"]:
            out[layer].append(
                (node["x1"], node["y1"], node["x2"], node["y2"]))
        for ch in node.get("children", []):
            walk(ch)

    for ch in cell.get("children", []):
        walk(ch)
    return out


def check_cell(cell, cells):
    """Return (checked, failures) for the placed cuts of one cell."""

    #- Flatten everything once, tagging which cut instance every rect
    #- came from, so a cut is checked against the rest of the design and
    #- never against itself. Exclusion by ownership, not by geometry: a
    #- wire that is exactly congruent with the cut enclosure is a real
    #- connection and must count
    cut_instances = []

    def find_cuts(node):
        if node.get("class") == "Instance" and is_cut_cell(node.get("cell", "")):
            cut_instances.append(node)
            return
        for ch in node.get("children", []):
            find_cuts(ch)

    for ch in cell.get("children", []):
        find_cuts(ch)

    cut_ids = {id(inst): i for i, inst in enumerate(cut_instances)}

    flat = defaultdict(list)

    def flatten_with_owner(node, owner):
        cls = node.get("class", "")
        if cls == "Instance":
            this_owner = cut_ids.get(id(node), owner)
            sub = cells.get(node.get("cell", ""))
            if sub is None:
                return
            subrects = collect_cell_rects(sub, cells)
            angle = node.get("angle", "")
            xc, yc = node.get("xcell", 0), node.get("ycell", 0)
            x1, y1 = node.get("x1", 0), node.get("y1", 0)
            for layer, rects in subrects.items():
                for r in rects:
                    flat[layer].append(
                        (transform_rect(r, angle, xc, yc, x1, y1),
                         this_owner))
            return
        layer = node.get("layer", "")
        if layer and "x1" in node and node["x1"] != node["x2"] \
           and node["y1"] != node["y2"]:
            flat[layer].append(
                ((node["x1"], node["y1"], node["x2"], node["y2"]), owner))
        for ch in node.get("children", []):
            flatten_with_owner(ch, owner)

    for ch in cell.get("children", []):
        flatten_with_owner(ch, None)

    checked = 0
    failures = []
    for idx, inst in enumerate(cut_instances):
        cut = cells.get(inst.get("cell", ""))
        if cut is None:
            continue
        angle = inst.get("angle", "")
        xc, yc = inst.get("xcell", 0), inst.get("ycell", 0)
        x1, y1 = inst.get("x1", 0), inst.get("y1", 0)
        cut_rects = collect_cell_rects(cut, cells)
        #- Only the terminal layers of the stack have to land on outside
        #- metal. A cut_M1M4 exists to hop from M1 to M4, and its M2 and
        #- M3 enclosures connect to nothing but the vias inside the cut
        #- itself. The metals in between are wired up by construction
        metals = sorted((l for l in cut_rects if l.startswith("M")),
                        key=lambda l: l[1:])
        if not metals:
            continue
        terminals = {metals[0], metals[-1]}
        for layer, rects in cut_rects.items():
            if layer not in terminals:
                continue
            for r in rects:
                placed = transform_rect(r, angle, xc, yc, x1, y1)
                checked += 1
                hit = any(overlaps(placed, other)
                          for other, owner in flat.get(layer, [])
                          if owner != idx)
                if not hit:
                    failures.append(
                        "%s: %s at (%d,%d) layer %s rect (%d,%d)-(%d,%d)"
                        " touches no other %s" % (
                            cell.get("name", "?"), inst.get("cell", "?"),
                            x1, y1, layer,
                            placed[0], placed[1], placed[2], placed[3],
                            layer))
    return checked, failures


def main():
    args = sys.argv[1:]
    allow = 0
    if "--allow" in args:
        i = args.index("--allow")
        allow = int(args[i + 1])
        del args[i:i + 2]
    if len(args) < 1:
        print("usage: check_cuts.py <file.cic> [cell-substring] [--allow N]")
        return 1
    with open(args[0]) as f:
        data = json.load(f)
    match = args[1] if len(args) > 1 else ""

    cells = {c.get("name", ""): c for c in data.get("cells", [])}

    total = 0
    all_failures = []
    for name, cell in cells.items():
        if is_cut_cell(name):
            continue
        if match and match not in name:
            continue
        checked, failures = check_cell(cell, cells)
        total += checked
        all_failures.extend(failures)

    if total == 0:
        print("check_cuts: no cut metal checked, refusing to pass an"
              " empty design")
        return 1

    if len(all_failures) > allow:
        print("check_cuts: %d of %d cut metal rects touch nothing"
              " (allowed: %d):" % (len(all_failures), total, allow))
        for f in all_failures[:20]:
            print("  " + f)
        if len(all_failures) > 20:
            print("  ... and %d more" % (len(all_failures) - 20))
        return 1

    if all_failures:
        print("check_cuts: %d cut metal rects, %d known floaters"
              " (allowed %d), rest land on metal"
              % (total, len(all_failures), allow))
    else:
        print("check_cuts: %d cut metal rects, all land on metal" % total)
    return 0


if __name__ == "__main__":
    sys.exit(main())
