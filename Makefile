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


CMD=time ../bin/cic

.PHONY: doxygen coverage

all: qmake compile

qmake:
	qmake -o qmake.make ciccreator.pro

xcode:
	qmake -o qmake.make ciccreator.pro -spec  macx-xcode

#- Use a wrapper around qmake, I like defining my own makefiles
compile:
	${MAKE} -f qmake.make

clean:
	${MAKE} -f qmake.make clean

#- Run the program with the example json file
EXAMPLE=../examples
LIBNAME=devices
JSONFILE=${EXAMPLE}/${LIBNAME}.json
TECHFILE=${EXAMPLE}/tech.json
devices:
	cd lay; ${CMD} ${JSONFILE} ${TECHFILE} ${LIBNAME} ${OPT}


doxygen:
	doxygen

coverage:
	lcov --capture --directory . --output-file coverage/converage.info
	genhtml coverage/converage.info --output-directory coverage/