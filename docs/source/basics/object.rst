Object Definition File
=======================

The object definition file contains the almost everything except for the
placement information, and the connectivity, both of which are in the
Netlist.

The two main objects are ``header``\ and ``cells``.

Header
------

In ``header``\ you can write whatever you want, everything is ignored by
the compiler as long as it follows JSON syntax.

Cells
-----

``cells`` is an array of all the cells in the design. The order of the
cells are important, since a cell can’t be referenced until it’s been
created.
