Routing
=======

Routing in ciccreator is done using functions in LayoutCell,
addDirectedRoutes and addConnectivityRoutes. One should start with
addConnectivityRoutes, because that is easiest to use, however, it can
only access the ports on instances in a subcircuit.

See
https://github.com/wulffern/ciccreator/tree/master/examples/routes.spi
and
https://github.com/wulffern/ciccreator/tree/master/examples/routes.json
for the example source code used in this wiki.

.. toctree::
   :maxdepth: 1

   con_route
   dir_route
   via
   type
   options
