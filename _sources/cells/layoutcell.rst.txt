LayoutCell
==========

LayoutCell extends Cell, it’s the most basic cell. It’s the default
class unless ``class`` parameter is defined. LayoutCell can be found in
``cici-core/src/core/layoutcell.cpp``


setYoffsetHalf
^^^^^^^^^^^^^^

If called it will calculate the height of the cell, and adjust the
height of the cell to half. It’s used to overlap transistor drain/source

noPowerRoute : 1 \| 0
^^^^^^^^^^^^^^^^^^^^^

At paint() the power will be routedd unless ``noRoutePower : 1``

addDirectedRoute : Array
^^^^^^^^^^^^^^^^^^^^^^^^

Adds a route from start rectangles to stop rectangles. The rectangles
are determined based on the route command.

**Arguments**

.. code-block:: json

    [ "M1",  //Layer name
      "CKN", //Net name
      "XA1:MP0:D-|--XA2:MP0:G", //Route command
      "offsetlowend" //Route Options [optional]
    ]

**Route command** The routecommand is split with the following regex

.. code::

   ^([^-\|<>]*)([-\|<>]+)([^-\|<>]*)$

A route is defined by the characters ``-|<>``, everything before is put
into a “start rectangle” path regex, and everything after is put into a
“stop rectangle” path regex.

The path regex uses [[Cell::findAllRectangles(pathRegex,layer)|Cell]]

See [[Route]] for the route definitions and options

addConnectivityRoute: Array
^^^^^^^^^^^^^^^^^^^^^^^^^^^

Adds a route based on the connectivity of the subcircuit.

.. code-block:: json

   [ "M1",    //Layer name
     "^ENO$", //Regular expression for net match
     "-|--",  //Route
     "onTopL,offsetlow",  //Route options [optional]
     "",                  //Number of cuts to use [optional]
     "IVX|STATE"          //Instances to include [optional]
   ]

addPortOnRect: Array
^^^^^^^^^^^^^^^^^^^^

Define which rectangles to add ports on

.. code-block:: json

   [
    "CMP_OP",   //Port name
    "M1",       //Layer name
    "XA4:A"     //Path regex
    ]

addVia: Array
^^^^^^^^^^^^^

Add a via at an horizontal offset to a rectangle defined by a path regex

.. code-block:: json

   [ "M3",          //Start layer
     "M4",          //Stop layer
     "MP1:D",       //Path regex to find rectangle
     2,             //Horizontal cuts
     1,             //Vertical cuts       [optional]
     8,             //Horizontal offset   [optional]
     "CUST_VREF"    //Custom name for via [optional]
   ]

addConnectivityVia: Array
^^^^^^^^^^^^^^^^^^^^^^^^^

Add a via on ports defined by a net name regular epression

.. code-block:: json

   ["M4",      //Start layer
    "M5",      //Stop layer
    "C16",     //Path regex 
    1000,      //Grid overrride, if 0 it's equal the grid is a cut width
    2,         //Vertical cuts
    1,         //Horizontal cuts    [optional]
    15,        //Horizontal offset  [optional]
    -0.5,      //Vertical offset    [optional]
    "CUST_C16" //Custom name, searchable by path regex [optional]
    ],

addPortVia: Array
^^^^^^^^^^^^^^^^^

Adds a via, and places a port on the via

.. code-block:: json

   [ "M2",         //Start layer
     "M4",         //Stop layer
     "RESN",       //Port name
     "X9$:MN1$:D", //Path regex
     1,            //Vertical cuts
     2,            //Horizontal cuts
     -3,           //Horizontal offset, multiplum of via width
     -1,           //Vertical offset, multiplum of via height
     "CUST_RESN"   //Custom name, searchable by path regex [optional]
   ]

addVerticalrect: Array
^^^^^^^^^^^^^^^^^^^^^^

Adds a custom rectangle for the height of the module


.. code-block:: json

    ["M5",          //Layer
     "CUST_C16",    //Path regex
     1              //Cuts, default 0, if 0 then use rectangle width
    ]
