test:
	../../bin/cic ../../examples/SAR_ESSCIRC16_28N.json ../../examples/tech.json SAR
	python3 ../check_ports.py SAR.cic
	cicpy svg SAR.cic ../../examples/tech_svg.json SAR --x 300 --y 400

clean:
	rm -f SAR.cic
	rm -rf SAR_svg
	rm -f SAR_svg.html
