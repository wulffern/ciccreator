# ciccreator
Back in 2013 I started playing with the thought of generating a SAR ADC with a script, such that it would be easy to port between technologies. The prototype was written in Perl, and contained 16k lines of code. The Perl compiler is closed source, and not available outside NTNU. cIcCreator is the C++ version, it's been written from scratch to support the same input files as the Perl compiler, but with a vastly improved performance.   

# Master plan, part 1
:white_check_mark: Create prototype of compiler in Perl that can compile SAR ADCs (Aug 2014) 

:white_check_mark: Tapeout SAR ADC in 28nm FDSOI (Jan 2015)

:white_check_mark: Demonstrate effectiveness of compiled approach ([ESSCIRC 2016](http://ieeexplore.ieee.org/document/7598271/),[JSSC 2017](http://ieeexplore.ieee.org/document/7906479/)) (Sept 2016, April 2017)

:white_check_mark: Rewrite the compiler in C++ under a GPL license and compile equivalent GDS with Perl compiler (Nov 2016)

# Dependencies
- Qt 5.4

# Compile
I'd recommend the docker environment

    cd docker
    make build

# Test
    make run-sh
    make esscirc routes

# Viewing GDS output

I'd recommend [Klayout](www.klayout.de), you can find a Layer property file in
 examples/
 
Another nice option for viewing GDS is [GDS3D](https://sourceforge.net/projects/gds3d/)

# Videos

Minecraft video - https://youtu.be/J9lhEt0f3Wk 
cIcCreator in action - https://youtu.be/hv9u9aQGHQc

