Route Options
=============

The router has some options to ease routing. Both connectivity routes
and directed routes have options. Options are separated by a comma.

Sort options
^^^^^^^^^^^^

These options specify which rectangle to route from. They are useful for
connectivity route where the start rectangles are not specified.

onTopB
''''''

Put the bottom rectangle as start rectangle


.. image:: ../graphics/Options_onTopB.png

onTopT
''''''

Put the top rectangle as
start rectangle

.. image:: ../graphics/Options_onTopT.png

onTopR
'''''''

Put the
right rectangle as start rectangle


.. image:: ../graphics/Options_onTopR.png

onTopL
''''''''

Put the left rectangle as start rectangle

.. image:: ../graphics/Options_onTopL.png

Offset
^^^^^^

These options can offset the routing rectangle to avoid nearby routings

offsethigh
''''''''''

.. image:: ../graphics/Options_offsethigh.png

offsetlow
''''''''''

.. image:: ../graphics/Options_offsetlow.png

offsethighend
''''''''''''''''''''

.. image:: ../graphics/Options_offsethighend.png

offsetlowend
''''''''''''''''''''

.. image:: ../graphics/Options_offsetlowend.png

Track
^^^^^

‘track’ can be used to specifiy how many grids the route is offset. It
uses “ROUTE” and “horizontalgrid”.

For example ‘track4’, gives

.. image:: ../graphics/Options_track4.png

For example ‘track8’, gives

.. image:: ../graphics/Options_track8.png

Cuts
^^^^

The number of cuts used by the route can be modified. Default is two
cuts for the horizontal via.

.. _cuts-1:

\\d+cuts
''''''''''''''''''''''

For example ‘3cuts’

.. image:: ../graphics/Options_3cuts.png

And for example ‘5cuts’

.. image:: ../graphics/Options_5cuts.png
