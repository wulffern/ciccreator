# ciccreator
A few years ago I started playing with the thought of generating a SAR ADC with a script, such that it would be easy to port between technologies, the vision is that "Carsten's IC creator" will be a rapid layout generation tool for SAR ADCs. 

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

