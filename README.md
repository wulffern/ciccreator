# ciccreator
Back in 2013 I started playing with the thought of generating a SAR ADC with a script, such that it would be easy to port between technologies. The prototype was written in Perl, and contained 16k lines of code. The Perl compiler is closed source, and not available outside NTNU. cIcCreator is the C++ version, it's been written from scratch to support the same input files as the Perl compiler, but with a vastly improved performance.   

# Master plan
:white_check_mark: Create prototype of compiler in Perl that can compile SAR ADCs (Aug 2014) 

:white_check_mark: Tapeout SAR ADC in 28nm FDSOI (Jan 2015)

:white_check_mark: Demonstrate effectiveness of compiled approach ([ESSCIRC 2016](http://ieeexplore.ieee.org/document/7598271/),[JSSC 2017](http://ieeexplore.ieee.org/document/7906479/)) (Sept 2016, April 2017)

:white_check_mark: Rewrite the compiler in C++ under a GPL license and compile equivalent GDS with Perl compiler (Nov 2016)

# Compile and run, the easy version
I'd recommend the docker environment

    cd docker
    make build
    make run
    
This should compile the inputs into a gds file in lay/


# Viewing GDS output

I'd recommend [Klayout](www.klayout.de), you can find a Layer property file in
 examples/
 
Another nice option for viewing GDS is [GDS3D](https://sourceforge.net/projects/gds3d/)

# Videos

Minecraft video - https://youtu.be/J9lhEt0f3Wk 
cIcCreator in action - https://youtu.be/hv9u9aQGHQc

# Directions
I have a full-time job, and three kids, which means that my time for this project is unplannable, so things will be coded when they are coded. But I'd like to share what directions I see for ciccreator. 
- Rewamp the cic-gui: Today it's a relativley straightforward implemnetation, and usable to look at the designs. But I don't get the warm fuzzy feeling that I would want from a GUI. I'd like to transfer it to a quick QGraphicsScene.
- Add LVS: One of the main issues with developing with ciccreator is that the routing needs to be verified by layout versus schematic. As of today one has to use commercial LVS tools to check the GDSII vs the netlist. It's possible, however, that it could be doable to implement an "LVS light" into ciccreator itself. After all, it knows where are the rectangles are, and what they should be connected too. It might be a "easier" problem to solve than the standard "GDSII vs netlist".

If you feel you can contribute to any of the topics above, then get in touch.

# Comments/Questions
I'd be happy to answer most questions, but I do get around 100 emails a day, so it might take some time to get through them all.
Email: carsten@ciccreator.com


