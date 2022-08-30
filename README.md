
[![Docker Image CI](https://github.com/wulffern/ciccreator/actions/workflows/docker-image.yml/badge.svg?branch=master)](https://github.com/wulffern/ciccreator/actions/workflows/docker-image.yml)

# ciccreator
Back in 2013 I started playing with the thought of generating a SAR ADC with a
script, such that it would be easy to port between technologies. The prototype
was written in Perl, and contained 16k lines of code. The Perl compiler is
closed source, and not available outside NTNU. cIcCreator is the C++ version,
it's been written from scratch to support the same input files as the Perl
compiler, but with a vastly improved performance.

# Master plan
:white_check_mark: Create prototype of compiler in Perl that can compile SAR ADCs (Aug 2014) 

:white_check_mark: Tapeout SAR ADC in 28nm FDSOI (Jan 2015)

:white_check_mark: Demonstrate effectiveness of compiled approach ([ESSCIRC 2016](http://ieeexplore.ieee.org/document/7598271/),[JSSC 2017](http://ieeexplore.ieee.org/document/7906479/)) (Sept 2016, April 2017)

:white_check_mark: Rewrite the compiler in C++ under a GPL license and compile equivalent GDS with Perl compiler (Nov 2016)

# Compile and run, the easy version

Although it's possible to run "make" to compile ciccreator on most platforms,
the compile does require qt 5.4, which is starting to be hard to come by.

As such, the easiest is to use the docker environment (although, for some
reason, the GDS output is super slow)

    make docker run

To compile the SAR and the routes example do

    make gds

, which will generate the following files in lay/

Example of routing options:

    routes.cic                      JSON output
    routes.gds                      Layout stream file (KLayout openable)
    
Successive-Approximation Analog-To-Digital Converter:

    SAR_ESSCIRC16_28N.cic           JSON output
    SAR_ESSCIRC16_28N.gds           Layout stream file (KLayout openable)

Another nice option for viewing GDS is [GDS3D](https://sourceforge.net/projects/gds3d/)

For simulation in Skywater 130 nm PDK, head over to [aicex](https://github.com/wulffern/aicex)
 

# Videos

Minecraft video - https://youtu.be/J9lhEt0f3Wk 
cIcCreator in action - https://youtu.be/hv9u9aQGHQc

# Directions
I have a full-time job, and three kids, which means that my time for this
project is unplannable, so things will be coded when they are coded.


# Comments/Questions
I'd be happy to answer most questions, but I do get around 100 emails a day, so
it might take some time to get through them all.

Email: carsten@ciccreator.com


# Changelog

## v0.1.4

## Added ability to route to rails. 
With addDirectedRoute one can do ["M1","AVSS","xa:S-|--rail_r_AVSS","strap"], which will strap all mosfet sources to right route rail.
The rails are named rail_(t|b|r|l)_(NAME) 
