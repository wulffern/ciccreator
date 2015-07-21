# ciccreator
A few years ago I started playing with the thought of generating a SAR ADC with a script, such that it would be easy to port between technologies, the vision is that "Carsten's IC creator" will be a rapid layout generation tool for SAR ADCs. 

#Compile
 qmake; make

#Test
cd lay/; make


##Disclamer
This tool is not functional yet. I do have a Perl based tool which works, but that's not publicly available. The Perl tool is slow, however, and I've started the redesign of the tool in C++, which should speed it up. I have very few development hours available, so it might take some time before this tool works :-)
