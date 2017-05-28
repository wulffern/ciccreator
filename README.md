# ciccreator
Back in 2013 I started playing with the thought of generating a SAR ADC with a script, such that it would be easy to port between technologies. The prototype was written in Perl, and contained 16k lines of code. The Perl compiler is closed source, and not available outside NTNU. cIcCreator is the C++ version, it's been written from scratch to support the same input files as the Perl compiler, but with a vastly improved performance. 

# Master plan, part 1
:white_check_mark: Create prototype of compiler in Perl that can compile SAR ADCs (Aug 2014) 

:white_check_mark: Tapeout SAR ADC in 28nm FDSOI (Jan 2015)

:white_check_mark: Demonstrate effectiveness of compiled approach ([ESSCIRC 2016](http://ieeexplore.ieee.org/document/7598271/),[JSSC 2017](http://ieeexplore.ieee.org/document/7906479/)) (Sept 2016, April 2017)

:white_check_mark: Rewrite the compiler in C++ under a GPL license and compile equivalent GDS with Perl compiler (Nov 2016)

:x: Encourage development of open source, free IP blocks for academic use 

The vision is that "Custom IC Creator" will be a rapid layout generation tool for SAR ADCs. 

# Dependencies
- Qt 5.4

# Compile
Install > Qt5.4 (i.e http://download.qt.io/archive/qt/5.4/5.4.2/qt-opensource-linux-x64-5.4.2.run), and edit bashrc.config to point to your Qt5.4 path, then

    source bashrc.config
    make

# Test

    make esscirc

# Viewing
Open GDSII file or open built in viewer

    make view

Or you can use GDS3D from https://sourceforge.net/projects/gds3d/ 

    make view3d

# cIcCreator in action

Minecraft video - https://youtu.be/J9lhEt0f3Wk 

cIcCreator in action - https://youtu.be/hv9u9aQGHQc

# Directions
I have a full-time job, and three kids, which means that my time for this project is unplannable, so things will be coded when they are coded. But I'd like to share what directions I see for ciccreator. 
- Rewamp the cic-gui: Today it's a relativley straightforward implemnetation, and usable to look at the designs. But I don't get the warm fuzzy feeling that I would want from a GUI. I'd like to transfer it to a quick QGraphicsScene.
- Add LVS: One of the main issues with developing with ciccreator is that the routing needs to be verified by layout versus schematic. As of today one has to use commercial LVS tools to check the GDSII vs the netlist. It's possible, however, that it could be doable to implement an "LVS light" into ciccreator itself. After all, it knows where are the rectangles are, and what they should be connected too. It might be a "easier" problem to solve than the standard "GDSII vs netlist".

If you feel you can contribute to any of the topics above, then get in touch.

