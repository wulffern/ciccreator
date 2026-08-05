#!/usr/bin/env python3
"""Fail if any port in a cic file has no geometry.

A port is drawn like a plain pattern cell, and its rectangle is what the
routers and the LVS pin geometry hang on to. A zero size port means the
paint step lost it, which has happened once already: a stray break after
the port cases in PatternTile::fillCoordinatesFromString turned every port
into an empty rectangle and quietly removed the metal the port painted
(see the fallthrough comment in patterntile.cpp). This check pins the
behaviour so it cannot come back unnoticed.

Usage: check_ports.py <file.cic>
"""

import json
import sys


def main():
    if len(sys.argv) != 2:
        print("usage: check_ports.py <file.cic>")
        return 1

    with open(sys.argv[1]) as fi:
        design = json.load(fi)

    total = 0
    zero = []
    for cell in design.get("cells", []):
        if not isinstance(cell, dict):
            continue
        for child in cell.get("children", []):
            if not isinstance(child, dict):
                continue
            if "Port" not in str(child.get("class", "")):
                continue
            total += 1
            if child.get("x1") == child.get("x2") or child.get("y1") == child.get("y2"):
                zero.append(f"{cell.get('name')}:{child.get('name')}")

    if total == 0:
        print(f"check_ports: no ports found in {sys.argv[1]}, nothing checked")
        return 1

    if zero:
        print(f"check_ports: {len(zero)} of {total} ports have no geometry:")
        for z in zero[:20]:
            print(f"  {z}")
        return 1

    print(f"check_ports: {total} ports, all have geometry")
    return 0


if __name__ == "__main__":
    sys.exit(main())
