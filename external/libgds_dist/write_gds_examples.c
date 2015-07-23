/* 

WRITE GDS EXAMPLES

requires libgds.c and libgds.h,
which should be in the same directory as write_gds_examples.c

To compile, use a makefile containing the following (but not the dashes)

--------------------------------------------
libgds libgds.a: libgds.c libgds.h
	rm libgds.a libgds.o
	gcc -g -c libgds.c -lc -lm -o libgds.o
	ar -cvq libgds.a libgds.o

write_gds_examples: write_gds_examples.c libgds.a
	gcc -g -o write_gds_examples write_gds_examples.c -lm -lc libgds.a
--------------------------------------------

then use the command "make write_gds_examples"

Assuming that goes well, you can run the program by typing
"write_gds_examples"


*/

#include "libgds.h"


/*------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------*/

 
main( int argc, char *argv[] )
    {
    
    int fd,     // output gds file descriptor
        x[5],
        y[5];

    fd = open( "stuff.gds", O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH );

    if( fd <= 0 ) BAILOUT( "UNABLE TO OPEN OUTPUT FILE" );

    // start the gds library with HEADER, BGNLIB, LIBNAME, and UNITS

    gds_create_lib( fd, "meats", 0.001 /* um per bit */ );

    // create a cell called "hotdogs"

    gds_write_bgnstr( fd );
    gds_write_strname( fd, "hotdogs" );

    //-----------------------------------------------------------------------------
    // create a polygon 

    gds_write_boundary( fd );       // write just the token
    gds_write_layer( fd, 0 );       // layer 0, for example
    gds_write_datatype( fd, 1 );    // datatype 1, for example

    x[0] =    0;  y[0] =   0;       // signed four-byte integers
    x[1] =    0;  y[1] = 500;       
    x[2] = 1000;  y[2] = 500;       // in this example 1 integer unit = 1 nm
    x[3] = 1000;  y[3] =   0;
    x[4] =    0;  y[4] =   0;       // required repetition of first point (yup, that's stupid)

    gds_write_xy( fd, x, y, 5 );    // polygon, four vertices, first vertex repeated => 5 points
    gds_write_endel( fd );          // end of element

    //-----------------------------------------------------------------------------
    // create some text, reflected about the x axis

//    gds_write_text( fd );
//    gds_write_layer( fd, 1 );
//    gds_write_texttype( fd, 0 );
//    gds_write_presentation( fd, 0, 1, 1 );  // fd, font, hp, vp
//    gds_write_width( fd, 500 );
//    gds_write_strans( fd, 1, 0, 0 );        // fd, reflect, abs_angle, abs_mag
//    x[0] = 2000;  
//    y[0] = 2000;
//    gds_write_xy( fd, x, y, 1 );  
//    gds_write_string( fd, "reflected" );
//    gds_write_endel( fd );

    //-----------------------------------------------------------------------------
    // create some text, using the helper function instead.
    // arguments: file-descriptor, string, x, y, layer, size
    // where x, y, and size are in database units (nanometers, usually)

	//   gds_create_text( fd, "not reflected", 2000, 1500, 2, 500 );  


    //-----------------------------------------------------------------------------
    // create a path

//    gds_write_path( fd );
//    gds_write_layer( fd, 3 );               // layer 3
	//   gds_write_datatype( fd, 4 );            // datatype 4
    //gds_write_pathtype( fd, 2 );            // extended square ends
    //gds_write_width( fd, 200 );             // 200 nm wide
    //x[0] = 2000;  y[0] = 3000;
    //x[1] = 2000;  y[1] = 4000;
    //x[2] = 2500;  y[2] = 3500;
    //gds_write_xy( fd, x, y, 3 );
    //gds_write_endel( fd );

    //-----------------------------------------------------------------------------
    // create a box, which is stupid

    //gds_write_box( fd );            // write just the token
    //gds_write_layer( fd, 6 );       // layer 6, for example
    //gds_write_boxtype( fd, 12 );    // boxtype 12, for example same as datatype

    //x[0] = 3000;  y[0] =   0;       // signed four-byte integers
    //x[1] = 3000;  y[1] = 500;       
    //x[2] = 4000;  y[2] = 500;       // in this example 1 integer unit = 1 nm
    //x[3] = 4000;  y[3] =   0;
    //x[4] = 3000;  y[4] =   0;       // required repetition of first point (yup, that's stupid)

	// gds_write_xy( fd, x, y, 5 );    // polygon, four vertices, first vertex repeated => 5 points
    //gds_write_endel( fd );          // end of element


    // end the structure (the cell hotdogs)

    gds_write_endstr( fd );

    //-----------------------------------------------------------------------------
    // Create a new cell, which will contain an instance of the previous cell 

//    gds_write_bgnstr( fd );                  // new cell (structure)
//    gds_write_strname( fd, "sausage" );      // called "sausage"
	//  gds_write_sref( fd );                    // contains an instance of...
    //gds_write_sname( fd, "hotdogs" );        // the cell hotdogs
    //gds_write_mag( fd, 5.0 );                // which will be 5 times larger
    //gds_write_angle( fd, 15.4 );             // and tilted at some weird angle
    //x[0] =  2000;
    //y[0] = -2000;
    //gds_write_xy( fd, x, y, 1 );             // at these coordinates (database units)
    //gds_write_endel( fd );                   // end of element
    //gds_write_endstr( fd );                  // end of structure (cell)

    //-----------------------------------------------------------------------------
    // Create a new cell "meatball" containing an array of the cell "sausage"

    //gds_write_bgnstr( fd );                  // new cell
    //gds_write_strname( fd, "meatball" );     // called "meatball"
    //gds_write_aref( fd );                    // containing an array of...
    //gds_write_sname( fd, "sausage" );        // the cell "sausage"
    //gds_write_colrow( fd, 2, 5 );            // 2 columns, 5 rows

    //x[0] =   5000;  y[0] =   5000;           // array anchor point

    //x[1] =  85000;  y[1] =   5000;           // displacement from anchor plus ncols*pitch
                                             // which makes the column pitch 40 um
    //x[2] =   5000;  y[2] = 205000;           // displacement from anchor plus nrows*pitch
                                             // which makes the row pitch 40 um
    //gds_write_xy( fd, x, y, 3 );             // See how you could have a diagonal matrix? 
                                             // That would be so goofy!
    //gds_write_endel( fd );                   // end of element
    //gds_write_endstr( fd );                  // end of structure (cell) "meatball"

    // end of library

    gds_write_endlib( fd );    
    close( fd );

    printf( "\nDone. Look at stuff.gds\n\n" );


    }
