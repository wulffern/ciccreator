# ciccreator
A few years ago I started playing with the thought of generating a SAR ADC with a script, such that it would be easy to port between technologies, the vision is that "Carsten's IC creator" will be a rapid layout generation tool for SAR ADCs. 

#Dependencies
- Qt 5.4

#Compile
1. Install > Qt5.4 (i.e http://download.qt.io/archive/qt/5.4/5.4.2/qt-opensource-linux-x64-5.4.2.run )
2. Edit bashrc.config and point to your Qt5.4 path

    source bashrc.config
    make

#Test
cd lay/; make

#Viewing
Open GDSII file or open built in viewer

    ../bin/cic-gui ../examples/tech.json devices.json 

