

[![Docker Image CI](https://github.com/wulffern/ciccreator/actions/workflows/docker-image.yml/badge.svg?branch=master)](https://github.com/wulffern/ciccreator/actions/workflows/docker-image.yml)
[![Documentation Status](https://github.com/wulffern/ciccreator/actions/workflows/documentation.yaml/badge.svg?branch=master)](https://github.com/wulffern/ciccreator/actions/workflows/documentation.yaml)
      
# ciccreator
Back in 2013 I started playing with the thought of generating a SAR ADC with a
script, such that it would be easy to port between technologies. The prototype
was written in Perl, and contained 16k lines of code. The Perl compiler is
closed source, and not available outside NTNU. cIcCreator is the C++ version,
it's been written from scratch to support the same input files as the Perl
compiler, but with a vastly improved performance.

# Documentation

Head over to
[wulffern.github.io/ciccreator](http://wulffern.github.io/ciccreator/index.html)
or [ciccreator.readthedocs.io](https://ciccreator.readthedocs.io/en/latest/index.html)

# Master plan
:white_check_mark: Create prototype of compiler in Perl that can compile SAR ADCs (Aug 2014) 

:white_check_mark: Tapeout SAR ADC in 28nm FDSOI (Jan 2015)

:white_check_mark: Demonstrate effectiveness of compiled approach ([ESSCIRC 2016](http://ieeexplore.ieee.org/document/7598271/),[JSSC 2017](http://ieeexplore.ieee.org/document/7906479/)) (Sept 2016, April 2017)

:white_check_mark: Rewrite the compiler in C++ under a GPL license and compile equivalent GDS with Perl compiler (Nov 2016)

# Compile and run, the easy version

Although it's possible to run "make" to compile ciccreator on most platforms,
the compile does require QT. Head over to ciccreator-bin repository for latest binaries.

For simulation in Skywater 130 nm PDK, head over to [aicex](https://github.com/wulffern/aicex)

# Test 
Have a look at the SAR 

``` bash
make esscirc 
make view 
```

Or the route tests

``` bash
make routes
make viewroutes
```


# Videos

Minecraft video - https://youtu.be/J9lhEt0f3Wk 
cIcCreator in action - https://youtu.be/hv9u9aQGHQc


# Comments/Questions
Should you find issues, don't hesitate to add an issue on gitub. 





# Changelog

## v0.1.5
* Added verticalMultiplyVector to patterntile to simplify analog transistor generation
* Route, RouteRing, Guard will be loaded as LayoutCell to fix view problem of cuts.
* Fixed issue rotateAngle in LayoutRotateCell. Seems like QJsonValue is no
  longer automatically casted to QString.
* removed GDS and SPICE output. Only *.cic will be supported from now on. Use
  cicpy to convert to other formats
  
## v0.1.4
* layoutcell: Added ability to route to rails.
With addDirectedRoute one can do ["M1","AVSS","xa:S-|--rail_r_AVSS","strap"], which will strap all mosfet sources to right route rail.
The rails are named rail_(t|b|r|l)_(NAME)
* cic-gui: Increased step size for arrow keys
* cic-gui: Added rubber band zoom.
