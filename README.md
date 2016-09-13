# ciccreator
Back in 2013 I started playing with the thought of generating a SAR ADC with a script, such that it would be easy to port between technologies. The prototype was written in Perl, and contained 16k lines of code. The Perl compiler is closed source, and not available outside NTNU. cIcCreator is the C++ version, it's been written from scratch to support the same input files as the Perl compiler, but with a vastly improved performance. 

# Master plan, part 1
:white_check_mark: Create prototype of compiler in Perl that can compile SAR ADCs (Aug 2014) 

:white_check_mark: Tapeout SAR ADC in 28nm FDSOI (Jan 2015)

:white_check_mark: Demonstrate effectiveness of compiled approach ([ESSCIRC 2016](http://www.epapers.org/ess2016/ESR/paper_details.php?PHPSESSID=1mkof6sr17ddpdc6thubflica1&paper_id=5064))

:x: Rewrite the compiler in C++ under a GPL license and compile equivalent GDS with Perl compiler

:x: Encourage development of open source, free IP blocks for academic use 

The vision is that "Custom IC Creator" will be a rapid layout generation tool for SAR ADCs. 

#Dependencies
- Qt 5.4

#Compile
Install > Qt5.4 (i.e http://download.qt.io/archive/qt/5.4/5.4.2/qt-opensource-linux-x64-5.4.2.run), and edit bashrc.config to point to your Qt5.4 path, then

    source bashrc.config
    make

#Test
    cd lay
    make
	make devices

#Viewing
Open GDSII file or open built in viewer

    make view

#cIcCreator in action
Minecraft video - https://youtu.be/J9lhEt0f3Wk 

