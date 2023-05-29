

test:
	../../bin/cic ip.json sky130.tech SAR
	cat SAR.cic | jq .cells > sar.json
	cat ref.cic | jq .cells > ref.json
	diff sar.json ref.json || exit 1;

clean:
	rm -f SAR.cic
	rm -f sar.json
	rm -f ref.json
