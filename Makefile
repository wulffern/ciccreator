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

VERSION=0.1.0

CMD=time ../bin/cic

#- Figure out which platform we're running on
ifeq ($(OS),Windows_NT)
	#- Not compatible with windows yet
OSNAME=Windows
GDS3D=WINDOWS
else
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
OSNAME=macOS
GDS3D=GDS3D/mac/GDS3D.app/Contents/MacOS/GDS3D
endif
ifeq ($(UNAME_S),Linux)
OSNAME=Linux
GDS3D=GDS3D/linux/GDS3D
endif
endif

.PHONY: doxygen coverage

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
	-rm cic-gui/Makefile
	-rm cic-color/Makefile

doxygen:
	doxygen

coverage:
	lcov --capture --directory . --output-file coverage/converage.info
	genhtml coverage/converage.info --output-directory coverage/


#- Run the program with the example json file
EXAMPLE=../examples
LIBNAME=SAR_ESSCIRC16_28N
CELL=SAR9B_EV
JSONFILE=${EXAMPLE}/${LIBNAME}.json
TECHFILE=${EXAMPLE}/tech.json

minecraft: lay
	cd lay; ${CMD} ${JSONFILE} ${EXAMPLE}/tech_minecraft.json minecraft ${OPT}

routes: lay
	cd lay; ${CMD} ${EXAMPLE}/routes.json ${TECHFILE} routes ${OPT}

esscircbulk:
	${MAKE}	esscirc LIBNAME=SAR_ESSCIRC16_28NBULK

esscirc: lay

#ifeq ($(OS),Windows_NT)
#	cd lay; ../bin/windows/cic.exe ${EXAMPLE}/${LIBNAME}.json ${TECHFILE} ${LIBNAME} ${OPT}
#else
	cd lay; ${CMD} ${EXAMPLE}/${LIBNAME}.json ${TECHFILE} ${LIBNAME} ${OPT}
	-./scripts/cics2aimspice  lay/${LIBNAME}.cics  lay/${LIBNAME}.spice
	cd lay	; ../bin/cic2eps ${EXAMPLE}/${LIBNAME}.json ${EXAMPLE}/tech_eps.json ${CELL}
	cd lay	; ../bin/cic2png ${EXAMPLE}/${LIBNAME}.json ${TECHFILE} ${EXAMPLE}/${LIBNAME}.hier
#endif

view: lay
	cd lay; ../bin/cic-gui ${TECHFILE} ${LIBNAME}.cicl &

view-routes: lay
	cd lay; ../bin/cic-gui ${TECHFILE} routes.json &

GDS3D:
	wget https://sourceforge.net/projects/gds3d/files/GDS3D%201.8/GDS3D_1.8.tar.bz2/download
	tar -zxvf download
	ln -s GDS3D_1.8 GDS3D
	rm download

view3d: GDS3D
	echo ${GDS3D}
	 ${GDS3D} -p examples/tech_gds3d.txt -i lay/SAR_ESSCIRC16_28N.gds -t SAR9B_EV


tar:
	cd ..; tar cfz ciccreator/ciccreator_${OSNAME}_${VERSION}.tar.gz ciccreator/bin ciccreator/Makefile ciccreator/README.md ciccreator/lib ciccreator/examples ciccreator/LICENSE 


