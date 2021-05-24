######################################################################
##        Copyright (c) 2020 Carsten Wulff Software, Norway
## ###################################################################
## Created       : wulff at 2020-11-27
## ###################################################################
##  The MIT License (MIT)
##
##  Permission is hereby granted, free of charge, to any person obtaining a copy
##  of this software and associated documentation files (the "Software"), to deal
##  in the Software without restriction, including without limitation the rights
##  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
##  copies of the Software, and to permit persons to whom the Software is
##  furnished to do so, subject to the following conditions:
##
##  The above copyright notice and this permission notice shall be included in all
##  copies or substantial portions of the Software.
##
##  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
##  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
##  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
##  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
##  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
##  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
##  SOFTWARE.
##
######################################################################

CONT=cic_qt_bionic

all:

build:
	docker build   -t ${CONT} .

sh:
	docker run --rm -it -v `pwd`/../:/lcic ${CONT} bash

run: esscirc_gds routes_gds

esscirc_gds:
	cd ../lay; docker run --rm --workdir /lcic/lay -v `pwd`/../:/lcic -t ${CONT}  /ciccreator/bin/cic --gds --spi /lcic/examples/SAR_ESSCIRC16_28N.json /lcic/examples/tech.json SAR_ESSCIRC16_28N

routes_gds:
	cd ../lay; docker run --rm --workdir /lcic/lay -v `pwd`/../:/lcic -t ${CONT}  /ciccreator/bin/cic --gds --spi /lcic/examples/routes.json /lcic/examples/tech.json routes
