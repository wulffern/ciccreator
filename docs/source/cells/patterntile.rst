PatternTile
============
PatternTile is the base functions for all ASCII to Layout objects.
PatternTile can be found in ``cic-core/src/core/patterntile.cpp``

Properties
~~~~~~~~~~

-  yoffset : number [optional] : Vertical grid offset of the origin
-  xoffset : number [optional] : Horizontal grid offset of the origin
-  widthoffset : number [optional] : Reduce the width by X grid

Functions
~~~~~~~~~

fillCoordinatesFromString
^^^^^^^^^^^^^^^^^^^^^^^^^

The argument is an array of arrays that contain the ASCII

.. code:: json

   [
     [ "Layer Name",
      "rectangle definitions",
     ]
   ]

for example

.. code:: json

   [
       [ "M1",
       "--xxxQxxxxx---",
       "----------x---",
       "--xxxxxkx-x---",
       "--------x-D---",
       "--xxxxxxx-x---",
       "--------x-x---",
       "--xxxxxkx-x---",
       "----------x---",
       "--xxxQxxxxx---"
       ]
   ]

The possible rectangle definitions are

* ‘-’ : Empty rectangle
* ‘x’ : Fill rectangle completely
* ‘X’ :
* ‘m’ : Fill rectangle horizontally, but use “mingatelength” rule for height
* ‘w’ : Fill rectangle horizontally, but use “width” rule for height
* ‘D,G,S,B,A’ : Add Port
* ‘c’ : Add cut in the center of the current grid
* ‘C’ : Add cut aligned on the left edge of the current grid
* ‘K’ : Add two cuts with the first cut aligend on the left edge of the current grid
* ‘k’ : Add two cuts with the first cut aligend in the center of the current grid
* ‘Q’ : Add two cuts centered in the center of the current grid
* ‘r’ : Add metal resistor
