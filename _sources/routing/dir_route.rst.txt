Directed Route
===============

DirectedRoute is for the cases were you want to access a port deep in
the hierarchy, but it’s not available on top level via a port. The

DirectedRoute should be avoided if at all possible, because it’s hard
linked to the hierarchy, so if you change instance names, or the
hierarchy, they will no longer work. But it’s perfect for those tricky
situations where ConnectivityRoute falls short.

The arguments for directed route are layer, net, and ‘route command’.

Route Command
^^^^^^^^^^^^^

A route command looks like this

.. code:: json

   "XA1:S-|--XB2:S"

The route command contains three parts, start rectangles path regex,
route type, and stop rectangles path regex. See addDirectedRoute in
[[LayoutCell]] for details.

A ‘path regex’ looks like this ‘XA1:XB2:D,XA2:XB2:D’, the colon denotes
a hierarchy border, while the stuff between are the regular expressions
used to match instance names. The last ‘D’ is the port name of an
instance. Multiple paths can be specified in one routecommand, separated
by a comma.

The `-|--` is the [[Route Type]] of this particular route command.

With the spice

.. code:: spice

   .subckt TEST A B
   XA1 B DDD
   XA2 A DDD
   XB1 B DDD  
   XB2 A DDD
   .ends

.. code:: json

   { "name" : "Directed_StartLeft_Left(-|--)",
     "inherit": "TEST",
     "beforeRoute": [
       {"addDirectedRoutes" : [["M1","S","XA1:S-|--XB2:S"]]}
     ]
   }

Will produce

.. image:: ../graphics/DirectedRoute.png
