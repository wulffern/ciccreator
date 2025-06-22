Technology File
===============

The technology file has sections: layers, technology, rules.

An example of a technology file for `Skywater 130 nm <https://github.com/wulffern/sun_tr_sky130nm/blob/main/cic/sky130.tech>`_


layers
------
cic expect to find some common layers, like "OD" (diffusion, active, the stuff
that is under the poly gates), "PO" (the gate material), "M1" "M2" (meta
layers).

cic does not care that much about the number, or datatype anymore, but they
still need to be there. The
important parts are the material, previous, next, and pin.

The color is used by cic-gui.

::

   "PO"  : { "number" : 7,         //SKILL layer number
             "datatype" : 0,       //SKILL layer datatype
             "alias" : "poly",     //Name in Skywater 130 nm Magic VLSI
             "material" : "poly",  //Type of layer [poly,cut,metal,marker,implant]    
             "previous" : "",      //Previous layer in the connect stack. For M1 it would be CO
             "next" : "CO",        //Layer to used to connect to the next routing layer
             "pin" : "PO_pin" ,    //Layer to be used for pins in this layer (i.e port text)
             "color" : "red"       //Color of layer in cic-gui
           }

technology
----------

Most of the technology file is irrelevant for cic, but the rule file still
expects to find gamma, grid, spiceunit, devices (name,devicetype,ports).

The "gamma" keyword is the unit used to translate from "rules" into real numbers
(Ångstrøm). The default gamma is 50 Ångstrøm.

The "grid" is the minimum snap distance, for example all units will be rounded
to "5 nm"

The "spiceunit" is no longer applicable, but has not been removed from "core/rules.cpp"



.. code:: json

   "gamma" : 500,
   "grid" : 5
   "spiceunit": 1,

rules
-----

All rules are defined as a multiplum of the gamma.

.. code:: json

   "M2"   : {                      // Layer to apply rules for
       "space" : 1,                // rule name : multiplum of gamma
       "width" : 1
    },

Common rule names are

* space : distance between two rectangles
* width : default width
* height : default height, common for cut layers
* enclosure : default enclosure of for example OD around this layer
* XXXenclosure : enclosure of current layer around layer XXX
* XXXencOpposite : enclosure of current layer around layer XXX, used when there is more than two cuts on a via

A special rule is the ‘ROUTE’ layer, which does not correspond to a
layer. It’s used to set the grid for the PatternTile objects, and only
has ‘verticalgrid’ and ‘horizontal grid’ rule names.

.. code:: json

    "ROUTE": { "horizontalgrid": 18, "verticalgrid" : 22 }
