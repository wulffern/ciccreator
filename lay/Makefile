
OPT=
cmd=time ../bin/cic

CELL=
GDS=${CELL}.gds
SPI=${CELL}.spi
LIBPATH=../../icarus_mpw2/lib/

tech=../examples/tech.json

compile:
	cd ..; ${MAKE};

FILE=devices

json: 
	${cmd}   ../examples/${FILE}.json ${tech} ${FILE} ${OPT}

sar:
	${cmd}   ../../cnano/examples/SAR_LPWR_ST28N/SAR_LPWR_ST28N.json ../../cnano/examples/soi.json SAR_LPWR_ST28N ${OPT}
	cat SAR_LPWR_ST28N.spi | perl -pe 'if(m/^\s*M\d+/ig){s/\n//ig;$$_ = $$_ . " p_la=0 \n";}' > SAR_LPWR_ST28N_tmp.spi;
	rm SAR_LPWR_ST28N.spi;
	mv SAR_LPWR_ST28N_tmp.spi SAR_LPWR_ST28N.spi

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
