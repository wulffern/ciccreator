Object Definition File
=======================

The object definition file contains the almost everything except for the
placement information, and the connectivity, both of which are in the
Netlist.

An example object definition file can be seen at `ip.json <https://github.com/wulffern/sun_tr_sky130nm/blob/main/cic/ip.json>`_


.. code-block:: json

  {
    "options" :
    {
        "ignoreSetYoffsetHalf" : false,
        "prefix" : "SUNTR_"
    },
    "include" : [
        "dmos_sky130nm_core.json",
        "dcap.json",
        "tr.json",
        "resistors.json",
        "dig.json",
        "components.json",
        "capacitor.json"

    ],
    "cells" : [ ]
  }


Comments
--------

Any line starting with `\s*//` (space followed by //), will be ignored by the compiler.

Options
-------

Options are instructions to cic. For example, "ignoreSetYoffsetHalf" is a global
parameter to override overlap of source/drain (custom feature).

Prefix is the name to prepend to all cells, which is useful for technologies
where one need to have multiple, slightly different, versision of the design.

Include
-------

The include sections link to other object definition files, and is mostly to
avoid having everything in one big file. The array is read sequentially, as
such, the order of the files are important. For example, if `tr.json` has an
instance of a MOSFET from `dmos_sky130nm_core.json` the MOSFET must be created
first.

Cells
-----

``cells`` is an array of all the cells in the design. The order of the
cells are important, since a cell can’t be referenced until it’s been
created.

An example of cell definition can be seen in `dig.json <https://github.com/wulffern/sun_tr_sky130nm/blob/main/cic/dig.json>`_

Each of the cells are read by cic, one by one, in sequence. cic will create an
instance of the class, and run methods on that class.

.. code-block:: json

  "cells":
    [
        { "name": "TAPCELLB_CV",
          "class" : "Layout::LayoutDigitalCell",
          "meta" : {
              "symbol" : "cic_wbulk/tap"
          },

          "boundaryIgnoreRouting" : 1,
          "beforeRoute" : {
              "addDirectedRoutes" : [ ["M1","AVSS","MN1:B->MN1:G"],
                                      ["M1","AVSS","MN1:G-|--MN1:S"],
                                      ["M1","AVSS","MN1:G-|--MN1:D"],
                                      ["M1","AVDD","MP:S-|--MP:G"],
                                      ["M1","AVDD","MP:D-|--MP:G"],
                                      ["M1","AVDD","MP:G->MP:B"]
                                    ]

          }

        }
    ]


Name
^^^^

All cells must have a name. The name is used to search the spice file (assumed
to be called `dig.spi` for a `dig.json` file). The name will also be used for
the cell in the output file

Class
^^^^^

The class correspond to a class inside cic
