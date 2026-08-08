Connectivity Route
==================

ConnectivityRoute uses regular expressions to match the net names. For
example, take the spice circuit below

.. code:: spice

   .subckt TEST A B
   XA1 B DDD
   XA2 A DDD
   XB1 B DDD  
   XB2 A DDD
   .ends

Without any routes, we’ll get

.. code:: json

   { "name" : "TEST", "class" : "cIcCore::LayoutCell"}

.. image:: ../graphics/TEST.png

We can add a route by matching for example ‘B’ with

.. code:: json

   { "name" : "Connectivity_B_-|--",
     "inherit": "TEST",
     "beforeRoute": [
       {"addConnectivityRoutes" : [["M1","B","-|--"]]}
     ]
   }


.. image:: ../graphics/Connectivity_B.png

Or even matching both A and B

.. code:: json

   { "name" : "Connectivity_A|B_-",
     "inherit": "TEST",
     "beforeRoute": [
         {"addConnectivityRoutes" : [["M1","A|B","-"]]}
     ]
   }


.. image:: ../graphics/Connectivity_AB.png

Even though we match both nets, the compiler knows it should not try and
route A->B. But the Router is very dum, it does not know if it’s created a
short.
