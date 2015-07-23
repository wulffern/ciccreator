all:	libgds write_gds_examples

libgds libgds.a: libgds.c libgds.h
	rm -f libgds.a libgds.o
	gcc -g -c libgds.c -lc -lm -o libgds.o
	ar -cvq libgds.a libgds.o

write_gds_examples: write_gds_examples.c libgds.a
	gcc -g -o write_gds_examples write_gds_examples.c -lm -lc libgds.a

