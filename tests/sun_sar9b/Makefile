

test:
	../../bin/cic ip.json sky130.tech sar
	cat sar.cic | jq .cells > sar.json
	cat ref.cic | jq .cells > ref.json
	diff sar.json ref.json || exit 1;
	cicpy svg sar.cic sky130.tech sar --x 300 --y 400 --scale 10

clean:
	rm -f SAR.cic
	rm -f sar.json
	rm -f ref.json
