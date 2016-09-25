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

OPT=
cmd=time ../bin/cic

CELL=
GDS=${CELL}.gds
SPI=${CELL}.spi
LIBPATH=../../icarus_mpw2/lib/

tech=../examples/tech.json


all:json


FILE=devices

json: 
	${cmd}   ../examples/${FILE}.json ${tech} ${FILE} ${OPT}


esscirc:
	${cmd}   ../examples/SAR_ESSCIRC16_28N.json ../../cnano/examples/soi_io.json SAR_ESSCIRC16_28N ${OPT}
	cat SAR_ESSCIRC16_28N.spi | perl -pe 'if(m/^\s*M\d+/ig){s/\n//ig;$$_ = $$_ . " p_la=0 \n";}' > SAR_ESSCIRC16_28N_tmp.spi;
	rm SAR_ESSCIRC16_28N.spi;
	mv SAR_ESSCIRC16_28N_tmp.spi SAR_ESSCIRC16_28N.spi


#--------------------------------------------------------------------
# Sections below are used for testing of ADC compile, which is not public (yet)
#--------------------------------------------------------------------

sar:
	${cmd}   ../../cnano/examples/SAR_LPWR_ST28N/SAR_LPWR_ST28N.json ../../cnano/examples/soi.json SAR_LPWR_ST28N ${OPT}
	cat SAR_LPWR_ST28N.spi | perl -pe 'if(m/^\s*M\d+/ig){s/\n//ig;$$_ = $$_ . " p_la=0 \n";}' > SAR_LPWR_ST28N_tmp.spi;
	rm SAR_LPWR_ST28N.spi;
	mv SAR_LPWR_ST28N_tmp.spi SAR_LPWR_ST28N.spi



devices:
	${cmd}   ../examples/devices.json ../../cnano/examples/soi.json DEVICES ${OPT}
	cat DEVICES.spi | perl -pe 'if(m/^\s*M\d+/ig){s/\n//ig;$$_ = $$_ . " p_la=0 \n";}' > DEVICES_tmp.spi;
	rm DEVICES.spi;
	mv DEVICES_tmp.spi DEVICES.spi

drc:
	-rm calibre/${CELL}_drc.svrf
	-rm calibre/${CELL}_drc.log
	cat ${LIBPATH}/calibre/calibre.cell.drc | perl -pe 's#gdsii/myCell.gds#${GDS}#ig' | perl -pe 's/myCell/${CELL}/ig' > calibre/${CELL}_drc.svrf
	calibre -drc -hier -nowait calibre/${CELL}_drc.svrf > calibre/${CELL}_drc.log
	-cat Cdrc/${CELL}.drc.summary | grep RULECHECK | grep -v "= 0" | grep -v "NOT EXECUTED" | grep -v "STATISTICS"| egrep -v '^\s+'


lvsl:
	-mkdir calibre
	-rm -rf svdb_${CELL}
	-rm calibre/${CELL}_lvs.svrf
	-rm calibre/${CELL}_lvs.log
	cat ${PDKITROOT}/DATA/LVS/CALIBRE/calibrelvs.ctrl |perl -pe 's#myCell\.gds#${GDS}#ig;s#myCell\.cdl#${SPI}#ig;s/\"svdb\"/\"svdb\/${CELL}\"/ig;s/qrc\/compare.rep/${CELL}\.lvs\.report/ig;s/SOFTCHK YES/SOFTCHK NO/ig;s/SUPPLY ERROR YES/SUPPLY ERROR NO/ig;s/myCell\.lvs/Clvs\/${CELL}.lvs/ig;s/myCell\.erc/Cerc\/${CELL}.erc/ig;s/erc\.summary/Cerc\/${CELL}_erc.summary/ig;s/myCell/${CELL}/ig;s#^INCLUDE#INCLUDE "${PROJECT}/lib/calibre/calibre.lvs"\nINCLUDE#ig;' > calibre/${CELL}_lvs.svrf
	calibre -spice Clvs/${CELL}.sp -lvs -hier -automatch -nowait calibre/${CELL}_lvs.svrf > calibre/${CELL}_lvs.log

lvs: lvsl
	less Clvs/${CELL}.lvs.report
	less Clvs/${CELL}.lvs.report.ext
