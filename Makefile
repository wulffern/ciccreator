######################################################################
##        Copyright (c) 2016 Carsten Wulff Software, Norway 
## ###################################################################
## Created       : wulff at 2016-1-29
## ###################################################################
##   This program is free software: you can redistribute it and/or modify
##   it under the terms of the GNU General Public License as published by
##   the Free Software Foundation, either version 3 of the License, or
##   (at your option) any later version.
## 
##   This program is distributed in the hope that it will be useful,
##   but WITHOUT ANY WARRANTY; without even the implied warranty of
##   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
##   GNU General Public License for more details.
## 
##   You should have received a copy of the GNU General Public License
##   along with this program.  If not, see <http://www.gnu.org/licenses/>.
######################################################################

VERSION=0.1.2

#- Figure out which platform we're running on
ifeq ($(OS),Windows_NT)
	#- Not compatible with windows yet
OSNAME=Windows
OSBIN=windows
GDS3D=WINDOWS
else
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
OSNAME=macOS
OSBIN=darwin
GDS3D=GDS3D/mac/GDS3D.app/Contents/MacOS/GDS3D
endif
ifeq ($(UNAME_S),Linux)
OSNAME=Linux
OSBIN=linux
GDS3D=GDS3D/linux/GDS3D
endif
endif

ifeq ($(UNAME_S),Darwin)
CIC=../bin/${OSBIN}/cic.app/Contents/MacOS/cic
else
CIC= ../bin/${OSBIN}/cic
endif


.PHONY: doxygen coverage sim

all: qmake compile

lay:
	mkdir lay

qmake:
	qmake -o qmake.make ciccreator.pro 

windeploy:
	windeployqt bin/windows/cic-gui.exe

xcode:
	qmake -o qmake.make ciccreator.pro -spec  macx-xcode

#- Use a wrapper around qmake, I like defining my own makefiles
compile:
	${MAKE} -f qmake.make

clean:
	${MAKE} -f qmake.make clean
	-rm cic/Makefile
	-rm cic-core/Makefile

doxygen:
	doxygen

help:
	@echo " make               Compile ciccreator"
	@echo " make docker        Make a docker image, and compile ciccreator"
	@echo " make run           Compile stuff inside docker"
	@echo " make sim           Simulate circuits"
	@echo " make esscirc       Compile SAR ADC from ESSCIRC paper"
	@echo " make view3d         View SAR in GDS3D"

#- Run the program with the example json file
EXAMPLE=../examples
LIBNAME=SAR_ESSCIRC16_28N
CELL=SAR9B_EV
JSONFILE=${EXAMPLE}/${LIBNAME}.json
TECHFILE=${EXAMPLE}/tech.json

routes: lay
	cd lay; ${CIC} ${EXAMPLE}/routes.json ${TECHFILE} routes ${OPT}

esscirc: lay
	cd lay; ${CIC} ${EXAMPLE}/${LIBNAME}.json ${TECHFILE} ${LIBNAME} ${OPT}
	-./scripts/cics2aimspice  lay/${LIBNAME}.cic  lay/${LIBNAME}.spice


GDS3D:
	wget https://sourceforge.net/projects/gds3d/files/GDS3D%201.8/GDS3D_1.8.tar.bz2/download
	tar -zxvf download
	ln -s GDS3D_1.8 GDS3D
	rm download

view3d: GDS3D
	echo ${GDS3D}
	 ${GDS3D} -p examples/tech_gds3d.txt -i lay/SAR_ESSCIRC16_28N.gds -t SAR9B_EV

CONT=cic_qt_bionic

docker:
	docker build   -t ${CONT} .

sh:
	docker run --rm -it -v `pwd`:/lcic ${CONT} bash

gds: esscirc_gds routes_gds

esscirc_gds:
	cd lay; docker run --rm --workdir /lcic/lay -v `pwd`/../:/lcic -t ${CONT}  /ciccreator/bin/cic --gds --spi /lcic/examples/SAR_ESSCIRC16_28N.json /lcic/examples/tech.json SAR_ESSCIRC16_28N

routes_gds:
	cd lay; docker run --rm --workdir /lcic/lay -v `pwd`/../:/lcic -t ${CONT}  /ciccreator/bin/cic --gds --spi /lcic/examples/routes.json /lcic/examples/tech.json routes

sim:
	-./scripts/cics2aimspice  lay/${LIBNAME}.cic  lay/${LIBNAME}.spice
	cd sim; make sar plot_sar
