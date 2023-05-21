.. Custom IC Creator documentation master file, created by
   sphinx-quickstart on Mon Dec  5 20:08:26 2022.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

Custom IC Creator
=============================================

cIcCreator is a compiler for integrated circuits.

cIcCreator reads a JSON object definition file, technology rule file
and a SPICE netlist (assumes same name as object definition file)
and outputs a cic description file (.cic).

Use cicpy (https://github.com/wulffern/cicpy) to transpile the .cic file to other formats.


Getting started
===============

First clone the repo

.. code-block:: bash

   git clone https://github.com/wulffern/ciccreator
   cd ciccreator
   git clone https://github.com/wulffern/ciccreator-bin release

In the release folder, you hopefully find the binary for your platform.

There are three input files needed to run the compiler

* `Technology file <https://github.com/wulffern/ciccreator/blob/master/examples/tech.json>`_
* `Object definition file <https://github.com/wulffern/ciccreator/blob/master/examples/SAR_ESSCIRC16_28N.json>`_
* `Netlist <https://github.com/wulffern/ciccreator/blob/master/examples/SAR_ESSCIRC16_28N.json>`_

The description below is for MacOS. For other OS replace the .darwin with the
binary that applies to your platform.

.. code-block:: bash

   release/cic.darwin-latest examples/SAR_ESSCIRC16_28N.json examples/tech.json SAR_TEST

The output of the compiler will be SAR_TEST.cic

To view the output, do

.. code-block:: bash

   release/cic-gui.darwin-latest SAR_TEST.cic examples/tech.json


The examples/routes.json show some examples of the routings, and other options

.. code-block:: bash

   release/cic.darwin-latest examples/routes.json examples/tech.json routes
   release/cic-gui.darwin-latest routes.cic examples/tech.json



.. toctree::
   :maxdepth: 2
   :caption: Contents:

   basics/basics
   cells/cells
   routing/route
   api
