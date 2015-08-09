
OPT=
cmd=time ../bin/cic

CELL=
GDS=${CELL}.gds
SPI=${CELL}.spi


tech=../examples/soi.json

device: 
	${cmd}   ../examples/devices.json ${tech}  ${OPT}
test: 
	${cmd}   ../examples/test.json ${tech}  ${OPT}

sar:
	${cmd}   ../../cnano/examples/SAR_LPWR_ST28N/SAR_LPWR_ST28N.json ${tech} ${OPT}


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
