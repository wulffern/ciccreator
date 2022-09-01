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

VERSION=0.1.4+
VERSION_DATE=${VERSION} built on $(shell date)
VERSION_HASH=${shell git describe --tags}

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
CICGUI=../bin/${OSBIN}/cic-gui.app/Contents/MacOS/cic-gui
else
CIC= ../bin/${OSBIN}/cic
endif



.PHONY: doxygen coverage sim

all: compile

lay:
	mkdir lay

compile:
	-mkdir build
	echo "#define CICVERSION \""${VERSION_DATE}"\""  > cic/src/version.h
	echo "#define CICHASH \""${VERSION_HASH}"\""  >> cic/src/version.h
	echo "#define CICVERSION \""${VERSION_DATE}"\""  > cic-gui/src/version.h
	echo "#define CICHASH \""${VERSION_HASH}"\""  >> cic-gui/src/version.h
	qmake -o qmake.make  ciccreator.pro
	${MAKE} -f qmake.make
	test -d build || mkdir build
	cp build/${CIC} build/cic.${OSBIN}_${VERSION}
	cp build/${CICGUI} build/cic-gui.${OSBIN}_${VERSION}

clean:
	${MAKE} -f qmake.make clean
	-rm -rf GDS3D
	-rm -rf GDS3D_1.8
	-rm cic/Makefile
	-rm cic-core/Makefile

doxygen:
	doxygen

help:
	@echo " make               Compile ciccreator"
	@echo " make docker        Make a docker image, and compile ciccreator"
	@echo " make run           Compile stuff inside docker"
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
	cd lay; ${CIC} --gds ${EXAMPLE}/${LIBNAME}.json ${TECHFILE} ${LIBNAME} ${OPT}

GDS3D:
	wget https://sourceforge.net/projects/gds3d/files/GDS3D%201.8/GDS3D_1.8.tar.bz2/download
	tar -zxvf download
	ln -s GDS3D_1.8 GDS3D
	rm download

view:
	cd lay; ${CICGUI} SAR_ESSCIRC16_28N.cic  ${TECHFILE}


view3d: GDS3D
	echo ${GDS3D}
	 ${GDS3D} -p examples/tech_gds3d.txt -i lay/SAR_ESSCIRC16_28N.gds -t SAR9B_EV

CONT=cic_qt_groovy:latest

docker:
	docker build   -t ${CONT} .

run:
	docker run --rm -it -v `pwd`:/lcic ${CONT} sh -c 'cd /lcic && make && cp /lcic/bin/linux/cic /lcic/build/cic.ubuntu_groovy_${VERSION}'

sh:
	docker run --rm -it -v `pwd`:/lcic ${CONT} bash

gds: esscirc_gds routes_gds

esscirc_gds:
	cd lay; docker run --rm --workdir /lcic/lay -v `pwd`/../:/lcic -t ${CONT} sh -c  '/lcic/bin/cic --gds --spi /lcic/examples/SAR_ESSCIRC16_28N.json /lcic/examples/tech.json SAR_ESSCIRC16_28N'

routes_gds:
	cd lay; docker run --rm --workdir /lcic/lay -v `pwd`/../:/lcic -t ${CONT} sh -c  '/lcic/bin/cic --gds --spi /lcic/examples/routes.json /lcic/examples/tech.json routes'
