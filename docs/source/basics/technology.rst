Technology File
===============

The technology file is divided into sections the following files:
layers, technology, rules

layers
------

All GDSII layers used must be described here. Some common layers are OD,
PO, M1, M2, etc. But you can define your own layers, for example

::

   "PO"  : { "number" : 7,         //GDS layer number
             "datatype" : 0,       //GDS layer datatype
             "material" : "poly",  //Type of layer [poly,cut,metal,marker,implant]    
             "previous" : "",      //Previous layer in the connect stack. for M1 it would be CO
             "next" : "CO",        //Layer to used to connect to the next routing layer
             "pin" : "PO_pin" ,    //Layer to be used for pins in this layer (i.e port text)
             "color" : "red"       //Color of layer in cic-gum
           }

technology
----------

The ‘devices’ keyword defines the known devices

::

     "pch_lvt_io" : {                   //name used internally in the compiler
        "name" : "pch_lvt_io",          //name to use in the SPICE netlist
        "ports" : ["d","g","s","b"]     //port names on the PDK device, currently not used
     },

The ‘gamma’ keyword can be used to specify the scaling used for the
rules. The default gamma is 50 ångstræm.

.. code:: json

   "gamma" : 500

rules
-----

All rules are defined as a multiplum of the gamma.

::

   "M2"   : {                      //Layer to apply rules for
       "space" : 1,                // rule name : multiplum of gamma
       "width" : 1 },

The used rule names are listed below - space : distance between two
rectangles - width : default width - height : default height, common for
cut layers - enclosure : default enclosure of for example OD around this
layer - XXXenclosure : enclosure of current layer around layer XXX -
XXXencOpposite :, enclosure of current layer around layer XXX, used when
there is more than two cuts on a via

A special rule is the ‘ROUTE’ layer, which does not correspond to a GDS
layer. It’s used to set the grid for the PatternTile objects, and only
has ‘verticalgrid’ and ‘horizontal grid’ rule names.
