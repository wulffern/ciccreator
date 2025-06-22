Via/Cut
=======

For most routes the vias will be automatically inserted. It will
automatically find the layer for the start/stop rectangles, and add a
via up to the routing layer specified.

Automatic via
^^^^^^^^^^^^^

Routes will find the necessary transisitons for routes.

::

   .subckt TESTVIA S D B
   XA1 S D B DDMVIA
   XA2 S D B DDMVIA
   XB1 S D B DDMVIA
   XB2 S D B DDMVIA
   .ends

.. code:: json

   { "name" : "TESTVIA",
     "class" : "cIcCore::LayoutCell",
     "beforeRoute": [
     {"addConnectivityRoutes" : [
       ["M4","S","-|--","track0"],
       ["M1","D","-|--","track4"],
       ["M2","B","-|--","track6"]
     ]}
   ]

.. image:: ../graphics/TESTVIA.png
