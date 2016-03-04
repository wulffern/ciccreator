/* 

GDSLIB 2.4

    A library of C functions for reading and writing GDSII Stream format,
    following the specification of Release 6 from 1987. This library was
    written by M. Rooks, Yale University, and released under the GNU
    public license.

    GDSII was originally developed by the Calma Company, and later sold
    to General Electric. GE sold the format to Cadence Design Systems,
    which released GDSII format into the public domain in 1997 by allowing
    most of the specification to be reprinted in the SPIE Handbook of
    Microlithography, Micromachining and Microfabrication, vol 1,
    P. Rai-Choudhury ed.

    GDSII format is not compact. Lossless compression programs such as
    'zip' and 'compress' can reduce file size by factors of three or more.

    Why use GDSII instead of a simple format like CIF?  Indeed, CIF is
    quite adequate for most algorithmically generated patterns. GDSII
    is useful when you need also to generate cell names, arrays,
    doses (datatypes), text, and rotated cells. It also lets you skip
    the conversion step when combining patterns. Therefore GDS is just
    a tiny bit better.
    
    Diagonal arrays are not supported, because they are stupid.
    Arrays at angles other than 0, 90, 180, and 270 are not supported
    because it's too complicated and subject to misinterpretation.
    Besides, who really wants a rotated array? That's silly. You could
    simply rotate the cell in which the array appears.

    2.1  Filters out degenerate vertices.

    2.2  gds_read_double() by Hans Romijn replaces gds_read_float()

    2.3  gds_write_xy no longer flips the bytes of x[] and y[] upon returning

    2.4  Pad strings to make their length even (to conform to the silly gds spec)
         only because the python gds library enforces this rule.

2015-08-20 Carsten Wulff: Changed definition of gds_write_float to avoid float representation problem. Copied from GDS2.pm on CPAN.


Supported tokens:

    HEADER              version number (6)
    BGNLIB              creation date
    LIBNAME             library name
    UNITS               database units
    ENDLIB              end of library
    BGNSTR              begin structure (cell)
    STRNAME             structure (cell) name
    ENDSTR              end structure (cell)
    BOUNDARY            polygon
    PATH                path = line with width
    SREF                structure reference = cell instance
    AREF                array reference = matrix of cells
    TEXT                text, not to be printed
    LAYER               layer number
    DATATYPE            datatype number (often used for dose assignment)
    WIDTH               path width
    XY                  coordinates for cell placement or polygon verticies
    ENDEL               end of element
    SNAME               structure (cell) name
    COLROW              number of columns and rows (for aref)
    TEXTTYPE            just like datatype, for text
    PRESENTATION        text justification
    STRING              character string
    STRANS              contains mirroring and other stuff
    MAG                 magnification
    ANGLE               angle
    PATHTYPE            describes ends of paths
    GENERATIONS         how many generations to retain (useless)
    BOX                 boxes were originally intended as documentation only
    BOXTYPE             boxes are often used like polygons, in practice
                        which makes them redundant. They also require
                        five vertices, which makes them stupid.


Unsupported, discontinued, and unused tokens:

    REFLIBS     SPACING     STRCLASS     PLEX       ELFLAGS
    FONTS       TEXTNODE    RESERVED     BGNEXTN    PROPVALUE
    NODE        UINTEGER    FORMAT       ENDEXTN    ELKEY
    NODETYPE    USTRING     MASK         LINKKEYS   LINKTYPE
    PROPATTR    STYPTABLE   ENDMASKS     TAPENUM    SRFNAME
    ATTRTABLE   STRTYPE     LIBDIRSIZE   TAPECODE   LIBSECUR


Token order:

    Library:       Structure:   Elements:

    HEADER         BGNSTR       BOUNDARY     PATH         SREF      AREF      TEXT           BOX
    BGNLIB         STRNAME      LAYER        LAYER        SNAME     SNAME     LAYER          LAYER
    LIBNAME        <element>    DATATYPE     DATATYPE     STRANS*   STRANS*   TEXTTYPE       BOXTYPE
    UNITS          ENDSTR       XY           PATHTYPE*    MAG*      MAG*      PRESENTATION*  XY
    <structures>                ENDEL        WIDTH*       ANGLE*    ANGLE*    PATHTYPE*      ENDEL
    ENDLIB                                   XY           XY        COLROW    WIDTH*
                                             ENDEL        ENDEL     XY        STRANS*
                                                                    ENDEL     MAG*
                                                                              ANGLE*
                                                                              XY
                                                                              STRING
                                                                              ENDEL

    * optional

    

*/

#include "libgds.h"

BYTE  gdsswap;
short gdsword;
/*------------------------------------------------------------------------------------------*/
//    UTILITY FUNCTIONS
/*------------------------------------------------------------------------------------------*/


void
gds_swap4bytes( BYTE *four  )
{
  static BYTE temp;
#if BYTESWAP
  temp    = four[0];
  four[0] = four[3];
  four[3] = temp;
  temp    = four[1];
  four[1] = four[2];
  four[2] = temp;
#endif
}

/*------------------------------------------------------------------------------------------*/

void
gds_swap2bytes( BYTE *two )
{
  static BYTE temp;
#if BYTESWAP
  temp   = two[0];
  two[0] = two[1];
  two[1] = temp;
#endif
}

/*------------------------------------------------------------------------------------------*/

void
gds_make_next_item( struct gds_itemtype **ci )
{
  // mag, angle and strans must be tallied
  // only when flattening the pattern.
  static struct gds_itemtype
      *current_item;


  current_item = *ci;
  current_item->nextitem = (struct gds_itemtype *) malloc( sizeof( struct gds_itemtype ) );

  if ( ! current_item->nextitem ) BAILOUT( "UNABLE TO ALLOCATE NEXT ITEM" );

  current_item = current_item->nextitem;

  // Each cell has a rotation, magnification and strans, but we do not need
  // to keep a running tally unless we are flattening the pattern.

  current_item->type          = -2;               // invalid
  current_item->width         = 0;
  current_item->n             = 0;
  current_item->layer         = 0;
  current_item->dt            = 0;
  current_item->cell_number   = -1;               // invalid
  current_item->path_end      = 0;
  current_item->mag           = 1.0;              /* mag */
  current_item->angle         = 0.0;              /* angle */
  current_item->abs_angle     = FALSE;            /* from strans */
  current_item->abs_mag       = FALSE;            /* from strans */
  current_item->reflect       = FALSE;            /* from strans, reflect before rotation */
  current_item->rows          = 0;                /* n cols */
  current_item->cols          = 0;                /* n rows */
  current_item->col_pitch     = 0;                /* column pitch */
  current_item->row_pitch     = 0;	            /* row pitch    */
  current_item->col_pitchy    = 0;                /* diagonal components are */
  current_item->row_pitchx    = 0;	            /* hopefully zero always */
  current_item->nextitem = NULL;

  *ci = current_item;

}  // make_next_item

/*------------------------------------------------------------------------------------------*/

double
gds_read_double( int fd )

/* Real numbers are not represented in IEEE format. A floating point number is    */
/* made up of three parts: the sign, the exponent, and the mantissa. The value    */
/* of the number is defined to be (mantissa) (16) (exponent) . If "S" is the      */
/* sign bit, "E" are exponent bits, and "M" are mantissa bits then an 8-byte      */
/* real number has the format                                                     */
/*                                                                                */
/* SEEEEEEE MMMMMMMM MMMMMMMM MMMMMMMM                                            */
/* MMMMMMMM MMMMMMMM MMMMMMMM MMMMMMMM                                            */
/*                                                                                */
/* The exponent is in "excess 64" notation; that is, the 7-bit field shows a      */
/* number that is 64 greater than the actual exponent. The mantissa is always     */
/* a positive fraction greater than or equal to 1/16 and less than 1. For an      */
/* 8-byte real, the mantissa is in bits 8 to 63. The decimal point of the         */
/* binary mantissa is just to the left of bit 8. Bit 8 represents the value       */
/* 1/2, bit 9 represents 1/4, and so on.                                          */
/*                                                                                */
/* In order to keep the mantissa in the range of 1/16 to 1, the results of        */
/* floating point arithmetic are normalized. Normalization is a process whereby   */
/* the mantissa is shifted left one hex digit at a time until its left four       */
/* bits represent a non-zero quantity. For every hex digit shifted, the           */
/* exponent is decreased by one. Since the mantssa is shifted four bits at a      */
/* time, it is possible for the left three bits of a normalized mantissa to be    */
/* zero. A zero value is represented by a number with all bits zero. The          */
/* representation of negative numbers is the same as that of positive numbers,    */
/* except that the highest order bit is 1, not 0.                                 */

/* Contributed by Hans Romijn (Raith/Vistec)                                      */

{
  BYTE
      e,
      m[8];

  int
      i,
      b,
      bit,
      sign,
      bitmask,
      exponent;

  long mant = 0;
  long bitm = 0x80000000000000;

  double
      mantissa,
      number;

  read( fd, &m, 8 );

  if ( m[0] >> 7 )
    sign = -1;
  else
    sign = 1;

  exponent = (m[0] % 128) - 64;

  for ( b = 1; b <8; b++ )
    {
      bitmask   = 0x80;
      for ( bit = 7; bit >= 0; bit-- )
        {
          i = ( m[b] & bitmask ) >> bit;
          if (i)
            mant += bitm ;

          // mantissa = (double)mant/(double)(0x100000000000000);
          // number = sign * mantissa * pow( 16, exponent );
          // printf("%d 0x%16lX: %.24lf %.24lf\n",i,bitm,number,(float)number);

          bitmask = bitmask >> 1 ;
          bitm = bitm >> 1;
        }
    }

  mantissa = (double)mant/(double)(0x100000000000000);

  number = sign * mantissa * pow( 16, exponent );

  return( number );

}  // read_double


/*------------------------------------------------------------------------------------------*/


float
gds_read_float( int fd )

/* Read 8 bytes and interpret the number as a wacky GDS float. The format must be */
/* the legacy of some very ancient General Electric computer, built before the    */
/* establishment of IEEE floating point format.                                   */

{
  BYTE
      e,
      m[8];

  int
      i,
      b,
      bit,
      sign,
      bitmask,
      exponent;

  float
      value,
      mantissa,
      number;


  read( fd, &e, 1 );

  if ( e >> 7 )
    sign = -1;
  else
    sign = 1;

  exponent = (e % 128) - 64;

  read( fd, m, 7 );

  value     = 2.0;
  mantissa  = 0.0;

  for ( b = 0; b <= 6; b++ )
    {
      bitmask   = 0x80;
      for ( bit = 7; bit >= 0; bit-- )
        {
          i = ( m[b] & bitmask ) >> bit ;
          mantissa += i / value ;
          value = value + value ;
          bitmask = bitmask >> 1 ;
        }
    }


  number = sign * mantissa * pow( 16, exponent );

  return( number );

}  // read_float

/*------------------------------------------------------------------------------------------*/

void
gds_bindump( BYTE x )            // dump one byte in binary format
{                            // way too clever hack from a forum post, sorry
  int z;
  static char b[9];

  b[0] = '\0';

  for ( z=128; z>0; z >>=1 )
    strcat( b, ((x & z) == z) ? "1" : "0" );

  printf( "%s ", b );

}

/*------------------------------------------------------------------------------------------*/

void gds_write_float(int fd, float x ){

	int isNegative;
	int i;
	float real;
	unsigned char exponent,b;

	float G_epsilon;

//	gds_write_pos(fd);
	
	b =0;
	G_epsilon = 0.000001;
	
  isNegative = 0;
  x = x;

  if(x < 0.0){
      isNegative = 1;
      x = 0 - x;
    }

  exponent = 0;
  while(x >= 1.0){
      exponent++;
      x = (x/16.0);
    }
  if(x != 0){
      while(x < 0.0625){
          exponent--;
          x = ( x * 16.0);
        }
    }

  if(isNegative){exponent += 192;}
  else {exponent += 64;}
  write( fd, &exponent, 1 );
//  printf("0: ");
//  gds_bindump(exponent);
//  printf("\n");
  for(i=1;i<=7;i++){
      if(x>=0){b = (int) (x*256.0) + G_epsilon;}
      else {b = (int) (x*256.0) - G_epsilon;}
      write(fd,&b,1);
//	  printf("%i: ",i);
//	  gds_bindump(b);
//	    printf("\n");
      x = x*256.0 - (b + 0.0);
    }
}

void
gds_write_float1( int fd, float x )

/* Write 8 bytes after converting back to wacky GDS float format. */

{

  unsigned int
      b,
      sign,
      e64,
      bit[56];

  int            // important
      exponent,
      i;

  float
      fexponent,
      mantissa,
      mantita;

  BYTE
      by,
      stupid[8];


  if ( ! BYTESWAP )
    WARNING( "WRITING OF FLOATING POINT ON LITTLE-ENDIAN MACHINES HAS NOT BEEN TESTED" );

  // printf( "\nencoding %g\n", x ); fflush( stdout );

  for ( i=0; i<8; i++ ) stupid[i] = 0;

  if ( x != 0.0 )
    {

      if ( x < 0.0 )
        sign = 1;
      else
        sign = 0;

      x = fabsf( x );

      exponent = 1 + floor( log( x ) / log( 16 ) );
      if ( exponent < -64 ) BAILOUT( "A NUMBER IS TOO SMALL TO ENCODE AS A GDS FLOAT" );
      fexponent = exponent;
      e64 = exponent + 64;
      mantissa = x / powf( 16.0, fexponent );
      mantita = mantissa;

      for( i=0; i<=56; i++ )
        {
          bit[i] = floor( pow(2,i+1) * mantita );
          mantita = mantita - bit[i] / pow(2,i+1);

        }

      stupid[0] = pow(2,7) * sign + e64;

      for ( i=0;  i<8;  i++ ) stupid[1] = stupid[1] + bit[i] * pow(2, 7-i);
      for ( i=8;  i<16; i++ ) stupid[2] = stupid[2] + bit[i] * pow(2,15-i);
      for ( i=16; i<24; i++ ) stupid[3] = stupid[3] + bit[i] * pow(2,23-i);
      for ( i=24; i<32; i++ ) stupid[4] = stupid[4] + bit[i] * pow(2,31-i);
      for ( i=32; i<40; i++ ) stupid[5] = stupid[5] + bit[i] * pow(2,39-i);
      for ( i=40; i<48; i++ ) stupid[6] = stupid[6] + bit[i] * pow(2,47-i);
      for ( i=48; i<56; i++ ) stupid[7] = stupid[7] + bit[i] * pow(2,55-i);

    }

  //printf( "\n" );
  for ( i=0; i<8; i++ )
    {
      by = stupid[i];
      write( fd, &by, 1 );
	  //             gds_bindump( by );
    }
//   printf( "\n" ); fflush( stdout );


} // write_float




/*------------------------------------------------------------------------------------------*/
//    TOKEN I/O FUNCTIONS
/*------------------------------------------------------------------------------------------*/

/* Contains GDS version number- we don't care */

void gds_read_header( int fd, int count, BOOL verbose )
{
  int i;
  SKIPOVER( fd, count );
}

/*------------------------------------------------------------------------------------------*/

void
gds_write_header( int fd )
{
  unsigned short int
      count,
      si,
      token;

  ssize_t w;

  count = 6;
  gds_swap2bytes( (BYTE *) &count );
  w = write( fd, &count, 2 );

  if ( w <= 0 ) BAILOUT( "UNABLE TO WRITE TO OUTPUT FILE - CHECK OPEN() CALL" );

  token = 0x0002;                // header
  gds_swap2bytes( (BYTE *) &token );
  write( fd, &token, 2 );
  si = 600;                      // GDSII version 6
  gds_swap2bytes( (BYTE *) &si );
  write( fd, &si, 2 );

}  // write_header

/*------------------------------------------------------------------------------------------*/
/* Beginning of library. Allocate the first cell. */
/* Ignore the creation date.                      */
void
gds_read_bgnlib( int  fd,                        // file descriptor
                 int  count,                     // bytes in this record (always 4)
                 int *cell_table_size,           // cell table size
                 struct gds_celltype **lib,      // pointer to library (main anchor point)
                 struct gds_celltype ***c,       // pointer to the cell table
                 struct gds_celltype **cc,       // pointer to the current cell
                 BOOL verbose )

{
  struct gds_celltype *library, **cell, *current_cell;
  int i;

  SKIPOVER( fd, count );
  library = (struct gds_celltype *) malloc( sizeof( struct gds_celltype ) );   /* start of the list */
  current_cell = library;                                              /* top of the list   */
  library->nextcell = NULL;

  if ( ! library ) BAILOUT( "UNABLE TO ALLOCATE LIBRARY POINTER. THAT'S STRANGE." );

  cell = (struct gds_celltype **) malloc( 1025 * sizeof( struct gds_celltype * ) ); /* table of pointers to cells */
  *cell_table_size = 1024;

  if ( ! cell ) BAILOUT( "UNABLE TO ALLOCATE MEMORY FOR CELL TABLE." );

  *lib = library;
  *c   = cell;
  *cc  = current_cell;

}  // read_bgnlib

/*------------------------------------------------------------------------------------------*/

void
gds_write_bgnlib( int fd )
{
  time_t *now;
  struct tm *date;

  short int
      year,
      month,
      day,
      hour,
      minute,
      second,
      token,
      count;

  now  = (time_t *)    malloc( sizeof( time_t ) );
  date = (struct tm *) malloc( sizeof( struct tm ) );

  time( now );
  date = localtime( now );

  count = 0x1C;
  gds_swap2bytes( (BYTE *) &count );
  write( fd, &count, 2 );
  token = 0x0102;                 // BGNLIB
  gds_swap2bytes( (BYTE *) &token );
  write( fd, &token, 2 );
  year = 1900 + date->tm_year;    // last modification time
  gds_swap2bytes( (BYTE *) &year );
  write( fd, &year, 2 );
  month = 1 + date->tm_mon;
  gds_swap2bytes( (BYTE *) &month );
  write( fd, &month, 2 );
  day = date->tm_mday;
  gds_swap2bytes( (BYTE *) &day );
  write( fd, &day, 2 );
  hour = date->tm_hour;
  gds_swap2bytes( (BYTE *) &hour );
  write( fd, &hour, 2 );
  minute = date->tm_min;
  gds_swap2bytes( (BYTE *) &minute );
  write( fd, &minute, 2 );
  second = date->tm_sec;
  gds_swap2bytes( (BYTE *) &second );
  write( fd, &second, 2 );

  write( fd, &year, 2 );          // last access time (same)
  write( fd, &month, 2 );
  write( fd, &day, 2 );
  write( fd, &hour, 2 );
  write( fd, &minute, 2 );
  write( fd, &second, 2 );

}  // write_bgnlib


/*------------------------------------------------------------------------------------------*/
/* Beginning of a cell. If this is after the first cell */
/* then allocate it.  Ignore the creation date.         */
void
gds_read_bgnstr( int fd, int count, 
                 int *cell_table_size,
                 int *num_cells,
                 struct gds_celltype ***c,          // cell table
                 struct gds_celltype **cc,          // current cell
                 struct gds_itemtype **ci,          // current item
                 BOOL verbose )
{
  static BOOL
      first_cell = TRUE;

  int i;

  static struct gds_celltype
      **cell,
      *current_cell;

  static struct gds_itemtype
      *current_item;

  current_item = *ci;
  cell = *c;
  current_cell = *cc;

  SKIPOVER( fd, count );

  if ( first_cell )  /* Then we already created the first cell in bgnlib */
    {
      first_cell = FALSE;
      *num_cells = 0;
    }
  else                       /* allocate a new cell and move up the pointer */
    {
      current_cell->nextcell = (struct gds_celltype *) malloc( sizeof( struct gds_celltype ) );

      if ( ! current_cell->nextcell ) BAILOUT( "UNABLE TO ALLOCATE MEMORY FOR THE NEXT CELL." );

      current_cell = current_cell->nextcell;
      current_cell->nextcell = NULL;

      *num_cells = *num_cells + 1;

      if ( *num_cells > *cell_table_size )
        {
          *cell_table_size *= 2;
          cell = (struct gds_celltype **) realloc( cell, (1 + *cell_table_size) * sizeof( struct gds_celltype * ) );
          if ( ! cell ) BAILOUT( "UNABLE TO REALLOCATE CELL TABLE. SORRY DUDE." );
        }

    }


  current_cell->item = (struct gds_itemtype *) malloc( sizeof( struct gds_itemtype ) );
  current_item = current_cell->item;
  current_item->type = -1;
  current_item->mag  = 1.0;
  current_item->nextitem = NULL;

  // Now let's be careful to not use the first item.
  // Seems like a waste, but it makes the logic simpler.

  *c   = cell;
  *cc  = current_cell;
  *ci  = current_item;

}  // read_bgnstr

/*------------------------------------------------------------------------------------------*/

void
gds_write_bgnstr( int fd )
{
  static time_t *now;
  static struct tm *date;

  static short int
      year,
      month,
      day,
      hour,
      minute,
      second,
      token,
      count;

  now  = (time_t *)    malloc( sizeof( time_t ) );
  date = (struct tm *) malloc( sizeof( struct tm ) );

  time( now );
  date = localtime( now );

  year   = 1900 + date->tm_year;
  month  = 1 + date->tm_mon;
  day    = date->tm_mday;
  hour   = date->tm_hour;
  minute = date->tm_min;
  second = date->tm_sec;

  gds_swap2bytes( (BYTE *) &year   );
  gds_swap2bytes( (BYTE *) &month  );
  gds_swap2bytes( (BYTE *) &day    );
  gds_swap2bytes( (BYTE *) &hour   );
  gds_swap2bytes( (BYTE *) &minute );
  gds_swap2bytes( (BYTE *) &second );

  count = 28;
  gds_swap2bytes( (BYTE *) &count );
  write( fd, &count, 2 );
  token = 0x0502;                 // BGNSTR
  gds_swap2bytes( (BYTE *) &token );
  write( fd, &token, 2 );
  write( fd, &year, 2 );          // creation time
  write( fd, &month, 2 );
  write( fd, &day, 2 );
  write( fd, &hour, 2 );
  write( fd, &minute, 2 );
  write( fd, &second, 2 );
  write( fd, &year, 2 );          // access time
  write( fd, &month, 2 );
  write( fd, &day, 2 );
  write( fd, &hour, 2 );
  write( fd, &minute, 2 );
  write( fd, &second, 2 );

}  // write_bgnstr

/*------------------------------------------------------------------------------------------*/
/* Marks the end of a library */
void
gds_read_endlib( int fd, int count, BOOL verbose )
{
  int i;
  SKIPOVER( fd, count );
}

/*------------------------------------------------------------------------------------------*/

void
gds_write_endlib( int fd )
{
  static short int
      count,
      token;

  count = 4;
  gds_swap2bytes( (BYTE *) &count );
  write( fd, &count, 2 );
  token = 0x0400;                 // ENDLIB
  gds_swap2bytes( (BYTE *) &token );
  write( fd, &token, 2 );

}  // write_endlib

/*------------------------------------------------------------------------------------------*/
/* Marks the end of a structure */
void
gds_read_endstr( int fd, int count, BOOL verbose )
{
  static int i;
  SKIPOVER( fd, count );
}

/*------------------------------------------------------------------------------------------*/

void 
gds_write_endstr( int fd )
{
  static short int
      count,
      token;

  count = 4;
  gds_swap2bytes( (BYTE *) &count );
  write( fd, &count, 2 );
  token = 0x0700;                 // ENDSTR
  gds_swap2bytes( (BYTE *) &token );
  write( fd, &token, 2 );

}  // write_endstr

/*------------------------------------------------------------------------------------------*/
/* Read the library name. */
char *
gds_read_libname( int fd, int count, BOOL verbose )
{
  char *name;
  name = (char *) malloc( count - 3 );

  read( fd, name, count-4 );
  name[count-4] = '\0';

  if ( verbose ) printf( "             library \"%s\"\n", name );
  return( name );

}  // read_libname

/*------------------------------------------------------------------------------------------*/

void
gds_write_libname( int fd, char *s )
{
  short int                       // name should be null-terminated
      count,
      len, i,
      token;

  static BOOL pad;
  static char *ps;

  len = strlen( s );
  pad = (len % 2) != 0;
  if ( pad ) len++;

  ps = (char *) malloc( len );
  if ( pad )
    {
      for ( i=0; i < len-1; i++ ) ps[i] = s[i];
      ps[len-1] = 0;
    }
  else
    {
      for ( i=0; i < len; i++ ) ps[i] = s[i];
    }


  count = 4 + len;
  gds_swap2bytes( (BYTE *) &count );
  write( fd, &count, 2 );
  token = 0x0206;                 // LIBNAME
  gds_swap2bytes( (BYTE *) &token );
  write( fd, &token, 2 );
  write( fd, ps, len );

}  // write_libname


/*------------------------------------------------------------------------------------------*/
/* Contains a structure name.  */
void
gds_read_strname( int fd, 
                  int count,
                  int num_cells,
                  struct gds_celltype ***c,
                  struct gds_celltype **cc,
                  BOOL verbose   )
{
  static struct gds_celltype
      **cell,
      *current_cell;

  cell = *c;
  current_cell = *cc;

  current_cell->name = (char *) malloc( count - 3 );

  if ( ! current_cell->name ) BAILOUT( "UNABLE TO ALLOCATE MEMORY FOR NEXT CELL NAME" );

  read( fd, current_cell->name, count-4 );

  current_cell->name[count-4] = '\0';
  if ( verbose ) printf( "             cell    \"%s\"\n", current_cell->name );

  cell[ num_cells ] = current_cell;    /* save the pointer to this cell */

  *c   = cell;
  *cc  = current_cell;

}  // read_strname

/*------------------------------------------------------------------------------------------*/

void
gds_write_strname( int fd, char *s )
{
  static short int         // name should be null-terminated
      count,
      len, i,
      token;

  static BOOL pad;
  static char *ps;

  len = strlen( s );
  pad = (len % 2) != 0;
  if ( pad ) len++;

  ps = (char *) malloc( len );
  if ( pad )
    {
      for ( i=0; i < len-1; i++ ) ps[i] = s[i];
      ps[len-1] = 0;
    }
  else
    {
      for ( i=0; i < len; i++ ) ps[i] = s[i];
    }

  count = 4 + len;
  gds_swap2bytes( (BYTE *) &count );
  write( fd, &count, 2 );
  token = 0x0606;                 // STRNAME
  gds_swap2bytes( (BYTE *) &token );
  write( fd, &token, 2 );
  write( fd, ps, len );

}  // write_strname


/*------------------------------------------------------------------------------------------*/
/* Contains a string for presentation, not for exposure.  */
void
gds_read_string( int fd, int count, struct gds_itemtype **ci, BOOL verbose )

{                            // A string should be saved as part of a text object
  // So hopefully the current item is TEXT.
  // gds_read_text generates a new text item.
  static struct gds_itemtype
      *current_item;

  static char str[513];

  static int n, m, i;

  current_item = *ci;

  if ( current_item->type != 4 ) BAILOUT( "SYNTAX ERROR: STRING APPEARS OUTSIDE OF TEXT OBJECT" );

  m = count - 4;
  n = m;
  if ( n > 512 ) n = 512;      // that's the spec: maximum 512 characters
  read( fd, str, n );
  if ( m > n )
    {
      WARNING( "STRING HAS MORE THAN 512 CHARACTERS" );
      printf(  "                                 [%s]\n", str  );
      fflush( stdout );
      SKIPOVER( fd, m-n );
    }
  str[n] = '\0';

  if ( verbose ) printf( "             %s\n", str );

  current_item->n = count - 4;
  current_item->text = (char *) malloc( n+1 );
  strcpy( current_item->text, str );

  *ci = current_item;

}  // read_string

/*------------------------------------------------------------------------------------------*/

void 
gds_write_string( int fd, char *s )
{                               // s must by null-terminated
  static short int
      count,
      token,
      i,
      len;
  static BOOL pad;
  static char *ps;

  len = strlen( s );

  if ( len > 512 )
    {
      WARNING( "ATTEMPT TO WRITE A STRING LONGER THAN 512 CHARACTERS. TRUNCATING." );
      len = 512;
      ps[len] = '\0';
    }

  pad = (len % 2) != 0;
  if ( pad ) len++;

  ps = (char *) malloc( len );
  if ( pad )
    {
      for ( i=0; i < len-1; i++ ) ps[i] = s[i];
      ps[len-1] = 0;
    }
  else
    {
      for ( i=0; i < len; i++ ) ps[i] = s[i];
    }

  count = 4 + len;
  gds_swap2bytes( (BYTE *) &count );
  write( fd, &count, 2 );
  token = 0x1906;                 // STRING
  gds_swap2bytes( (BYTE *) &token );
  write( fd, &token, 2 );
  write( fd, ps, len );

}  // write_string


/*------------------------------------------------------------------------------------------*/

// Contains the name of a referenced structure (cell).

// Passes by reference (!) the 2D array of forward references.

void
gds_read_sname( int fd, 
                int count,
                int num_cells,
                int *forward_num,
                char ***f,
                struct gds_celltype ***c,
                struct gds_itemtype **ci,
                BOOL verbose )
{
  static char name[256];
  static int  i;
  static BOOL forward_ref_init = FALSE;
  static int  max_forward_refs = MAX_FORWARD_REFS;
  static char **forward_name;
  static struct gds_celltype **cell;
  static struct gds_itemtype  *current_item;

  current_item = *ci;
  cell = *c;             // cell table, passed in but not out

  if ( count > 254 ) BAILOUT( "CELL NAME IS WAY TOO LONG" );

  read( fd, name, count-4 );
  name[count-4] = '\0';
  if ( verbose ) printf( "             %s\n", name );

  /* look this up in the cell table */

  i = 0;
  while ( (strcmp( name, cell[i]->name ) != 0) && (i < num_cells) ) i++;

  if ( i >= num_cells )
    {
      if ( verbose ) printf( "             forward reference\n" );
      current_item->cell_number = -1;        /* indicates that we must look again on the second pass */

      if ( ! forward_ref_init )
        {
          forward_ref_init = TRUE;
          forward_name = (char **) malloc( max_forward_refs * sizeof( char * ) );
          if ( ! forward_name ) BAILOUT( "UNABLE TO ALLOCATE ARRAY OF FORWARD REFERENCE NAMES" );
          *forward_num = -1;
        }
      else   // we use the array as it was passed in
        {
          forward_name = *f;
        }

      *forward_num = *forward_num + 1;

      if ( *forward_num >= max_forward_refs )
        {
          max_forward_refs *= 2;
          forward_name = (char **) realloc( forward_name, max_forward_refs * sizeof( char * ) );
          if ( ! forward_name ) BAILOUT( "UNABLE TO REALLOCATE ARRAY OF FORWARD REFERENCE NAMES" );
        }

      forward_name[*forward_num] = (char *) malloc( 256 );

      if ( ! forward_name[*forward_num] ) BAILOUT( "UNABLE TO ALLOCATE STRING FOR FORWARD NAME" );

      strcpy( forward_name[*forward_num], name );

    }
  else
    {
      if ( verbose ) printf( "             this is cell [%d]\n", i );
      current_item->cell_number = i;
    }

  *f = forward_name;       // send it back out again. yikes.
  *ci = current_item;

}  // read_sname

/*------------------------------------------------------------------------------------------*/

void 
gds_write_sname( int fd, char *s )
{
  static short int                // s should be null-terminated
      count,
      len, i,
      token;
  static BOOL pad;
  static char *ps;

  len = strlen( s );
  pad = (len % 2) != 0;
  if ( pad ) len++;

  ps = (char *) malloc( len );
  if ( pad )
    {
      for ( i=0; i < len-1; i++ ) ps[i] = s[i];
      ps[len-1] = 0;
    }
  else
    {
      for ( i=0; i < len; i++ ) ps[i] = s[i];
    }

  count = 4 + len;
  gds_swap2bytes( (BYTE *) &count );
  write( fd, &count, 2 );
  token = 0x1206;                 // SNAME
  gds_swap2bytes( (BYTE *) &token );
  write( fd, &token, 2 );
  write( fd, ps, len );

}  // write_sname


/*------------------------------------------------------------------------------------------*/
/* Marks the beginning of a boundary (polygon)  */
void
gds_read_boundary( int fd, int count, struct gds_itemtype **ci, BOOL verbose )
{
  static struct gds_itemtype
      *current_item;
  int i;

  current_item = *ci;
  SKIPOVER( fd, count );
  gds_make_next_item( &current_item );
  current_item->type = 0;  /* meaning, this is a polygon */

  *ci = current_item;

}  // read_boundary

/*------------------------------------------------------------------------------------------*/

void
gds_write_boundary( int fd )
{                             // just the token here. "xy" writes the actual polygon.
  static short int
      count,
      token;

  count = 4;
  gds_swap2bytes( (BYTE *) &count );
  write( fd, &count, 2 );
  token = 0x0800;                 // BOUNDARY
  gds_swap2bytes( (BYTE *) &token );
  write( fd, &token, 2 );

} // write_boundary

/*------------------------------------------------------------------------------------------*/
/* Marks the beginning of a box  */
void
gds_read_box( int fd, int count, struct gds_itemtype **ci, BOOL verbose )
{
  static struct gds_itemtype
      *current_item;
  int i;

  current_item = *ci;
  SKIPOVER( fd, count );
  gds_make_next_item( &current_item );
  current_item->type = 5;  /* meaning, this is a box */

  *ci = current_item;

}  // read_box

/*------------------------------------------------------------------------------------------*/

void
gds_write_box( int fd )
{                             // Boxes were orignally for documentation only,
  // but later, people started using them like polygons.
  // The spec says we need to store five points,
  // just like a boundary, which is so stupid.
  static short int
      count,
      token;

  WARNING( "BOXES ARE STUPID. USE BOUNDARIES INSTEAD." );

  count = 4;
  gds_swap2bytes( (BYTE *) &count );
  write( fd, &count, 2 );
  token = 0x2D00;                 // BOX
  gds_swap2bytes( (BYTE *) &token );
  write( fd, &token, 2 );

} // write_box

/*------------------------------------------------------------------------------------------*/

void
gds_read_boxtype( int fd, int count, struct gds_itemtype **ci, BOOL verbose )
{
  static short num;
  static struct gds_itemtype *current_item;
  current_item = *ci;

  read( fd, &num, 2 );
  gds_swap2bytes( (BYTE *) &num );

  if ( verbose ) printf( "             %d\n", num ) ;

  if ( num < 0 )
    WARNING( "NEGATIVE BOX TYPE NUMBER" );

  if ( num > 255 )
    WARNING( "BOX TYPE > 255 " );

  current_item->dt = num;
  *ci = current_item;

}  // read_boxtype


/*------------------------------------------------------------------------------------------*/

void
gds_write_boxtype( int fd, short int dt )
{
  static short int
      count,
      token;

  if ( dt < 0 )
    WARNING( "NEGATIVE BOXTYPE NUMBER" );

  if ( dt > 255 )
    WARNING( "BOXTYPE > 255 " );

  count = 6;
  gds_swap2bytes( (BYTE *) &count );
  write( fd, &count, 2 );
  token = 0x2E02;                 // BOXTYPE
  gds_swap2bytes( (BYTE *) &token );
  write( fd, &token, 2 );
  gds_swap2bytes( (BYTE *) &dt );
  write( fd, &dt, 2 );

}  // write_boxtype

/*------------------------------------------------------------------------------------------*/
/* Marks the beginning of a path  */
void
gds_read_path( int fd, int count, struct gds_itemtype **ci, BOOL verbose )
{
  int i;
  static struct gds_itemtype *current_item;
  current_item = *ci;
  SKIPOVER( fd, count );
  gds_make_next_item( &current_item );
  current_item->type = 1;           /* meaning, this is a path */
  *ci = current_item;

}  // read_path

/*------------------------------------------------------------------------------------------*/
void
gds_write_path( int fd )
{                             // Just the token here. "xy" writes the actual path.
  static short int
      count,
      token;

  count = 4;
  gds_swap2bytes( (BYTE *) &count );
  write( fd, &count, 2 );
  token = 0x0900;                 // PATH
  gds_swap2bytes( (BYTE *) &token );
  write( fd, &token, 2 );

}  // write_path

/*------------------------------------------------------------------------------------------*/
/* Marks the beginning of a sref (structure reference) */
void
gds_read_sref( int fd, int count, struct gds_itemtype **ci, BOOL verbose )
{
  int i;
  static struct gds_itemtype *current_item;
  current_item = *ci;
  SKIPOVER( fd, count );
  gds_make_next_item( &current_item );
  current_item->type = 3;     /* meaning, this is a sref */
  *ci = current_item;

}  // read_sref

/*------------------------------------------------------------------------------------------*/

void
gds_write_sref( int fd )
{                             // Only the token is written here.
  static short int
      count,
      token;

  count = 4;
  gds_swap2bytes( (BYTE *) &count );
  write( fd, &count, 2 );
  token = 0x0A00;                 // SREF
  gds_swap2bytes( (BYTE *) &token );
  write( fd, &token, 2 );

}  // write_sref

/*------------------------------------------------------------------------------------------*/
/* Marks the beginning of an aref (array reference) */
void
gds_read_aref( int fd, int count, struct gds_itemtype **ci, BOOL verbose )
{
  int i;
  static struct gds_itemtype *current_item;
  current_item = *ci;
  SKIPOVER( fd, count );
  gds_make_next_item( &current_item );
  current_item->type = 2;                  /* meaning, this is an aref */
  current_item->cell_number = -1;          /* indicates not found yet */
  *ci = current_item;

}  // read_aref

/*------------------------------------------------------------------------------------------*/

void
gds_write_aref( int fd )
{
  static short int                   // write the aref token only
      count,
      token;

  count = 4;
  gds_swap2bytes( (BYTE *) &count );
  write( fd, &count, 2 );
  token = 0x0B00;                   // AREF
  gds_swap2bytes( (BYTE *) &token );
  write( fd, &token, 2 );

}  // write_aref


/*------------------------------------------------------------------------------------------*/
/* Marks the beginning of a text element */
void
gds_read_text( int fd, int count, struct gds_itemtype **ci, BOOL verbose )
{
  int i;
  static struct gds_itemtype *current_item;
  current_item = *ci;
  SKIPOVER( fd, count );
  gds_make_next_item( &current_item );
  current_item->type = 4;  /* meaning, this is text */
  *ci = current_item;
}

/*------------------------------------------------------------------------------------------*/

void
gds_write_text( int fd )
{
  static short int                   // write the text token only
      count,
      token;

  count = 4;
  gds_swap2bytes( (BYTE *) &count );
  write( fd, &count, 2 );
  token = 0x0C00;                   // TEXT
  gds_swap2bytes( (BYTE *) &token );
  write( fd, &token, 2 );

}  // write_text


/*------------------------------------------------------------------------------------------*/
/* Marks the end of an element, pointlessly. */
void
gds_read_endel( int fd, int count, BOOL verbose )
{
  int i;
  SKIPOVER( fd, count );
}

/*------------------------------------------------------------------------------------------*/

void
gds_write_endel( int fd )
{
  static short int
      count,
      token;

  count = 4;
  gds_swap2bytes( (BYTE *) &count );
  write( fd, &count, 2 );
  token = 0x1100;                 // ENDEL
  gds_swap2bytes( (BYTE *) &token );
  write( fd, &token, 2 );

}  // write_endel


/*------------------------------------------------------------------------------------------*/
/* Contains the layer number 0..255 */
void
gds_read_layer( int fd, int count, struct gds_itemtype **ci, BOOL verbose )
{
  static short num;
  static struct gds_itemtype *current_item;
  current_item = *ci;
  read( fd, &num, 2 );
  gds_swap2bytes( (BYTE *) &num );
  if ( verbose ) printf( "             %d\n", num );

  if ( num < 0 )
    WARNING( "NEGATIVE LAYER NUMBER" );

  if ( num > 255 )
    WARNING( "LAYER > 255 " );

  current_item->layer = num;
  *ci = current_item;

}  // read_layer

/*------------------------------------------------------------------------------------------*/

void
gds_write_layer( int fd, short int layer )
{
  static short int
      count,
      token;

  // Layers are actually supposed to go up to 64 only, but most programs allow
  // values up to 255, because 64 is stupid. Well 255 is also stupid, but less so.

  if ( layer < 0 || layer > 255 ) BAILOUT( "INVALID LAYER NUMBER" );
  count = 6;
  gds_swap2bytes( (BYTE *) &count );
  write( fd, &count, 2 );
  token = 0x0D02;                 // LAYER
  gds_swap2bytes( (BYTE *) &token );
  write( fd, &token, 2 );
  gds_swap2bytes( (BYTE *) &layer );
  write( fd, &layer, 2 );

}  // write_layer

/*------------------------------------------------------------------------------------------*/
/* Contains the width of a path. Negative means that */
/* the width does not scale.                         */
void
gds_read_width( int fd, int count, struct gds_itemtype **ci, BOOL verbose )
{
  static int num;
  static struct gds_itemtype *current_item;
  current_item = *ci;

  read( fd, &num, 4 );
  gds_swap4bytes( (BYTE *) &num );

  if ( verbose ) printf( "             %d", num );

  if ( num < 0 )
    if ( verbose ) printf( "    does not scale" );

  if ( verbose ) printf( "\n" );

  current_item->width = num;
  *ci = current_item;

}  // read_width

/*------------------------------------------------------------------------------------------*/

void
gds_write_width( int fd, int width )
{
  static short int
      count,
      token;

  count = 8;
  gds_swap2bytes( (BYTE *) &count );
  write( fd, &count, 2 );
  token = 0x0F03;                 // WIDTH
  gds_swap2bytes( (BYTE *) &token );
  write( fd, &token, 2 );
  gds_swap4bytes( (BYTE *) &width );
  write( fd, &width, 4 );

}  // write_width


/*------------------------------------------------------------------------------------------*/
/* Contains the datatype number 0..255 */
void
gds_read_datatype( int fd, int count, struct gds_itemtype **ci, BOOL verbose )
{
  static short num;
  static struct gds_itemtype *current_item;
  current_item = *ci;

  read( fd, &num, 2 );
  gds_swap2bytes( (BYTE *) &num );

  if ( verbose ) printf( "             %d\n", num );

  if ( num < 0 )
    WARNING( "NEGATIVE DATATYPE NUMBER" );

  if ( num > 255 )
    WARNING( "DATATYPE > 255 " );

  current_item->dt = num;
  *ci = current_item;

}  // read_datatype

/*------------------------------------------------------------------------------------------*/

void
gds_write_datatype( int fd, short int dt )
{
  static short int
      count,
      token;

  if ( dt < 0 )
    WARNING( "NEGATIVE DATATYPE NUMBER" );

  if ( dt > 255 )
    WARNING( "DATATYPE > 255 " );

  count = 6;
  gds_swap2bytes( (BYTE *) &count );
  write( fd, &count, 2 );
  token = 0x0E02;                 // DATATYPE
  gds_swap2bytes( (BYTE *) &token );
  write( fd, &token, 2 );
  gds_swap2bytes( (BYTE *) &dt );
  write( fd, &dt, 2 );

}  // write_datatype


/*------------------------------------------------------------------------------------------*/
/* Contains the text type. Should be 0..63, or whatever. */
void
gds_read_texttype( int fd, int count, struct gds_itemtype **ci, BOOL verbose )
{
  static short num;
  static struct gds_itemtype *current_item;
  current_item = *ci;

  read( fd, &num, 2 );
  gds_swap2bytes( (BYTE *) &num );

  if ( verbose ) printf( "             %d\n", num ) ;

  if ( num < 0 )
    WARNING( "NEGATIVE TEXT TYPE NUMBER" );

  if ( num > 255 )
    WARNING( "TEXT TYPE > 255 " );

  current_item->dt = num;
  *ci = current_item;

}  // read_texttype

/*------------------------------------------------------------------------------------------*/

void
gds_write_texttype( int fd, short int dt )
{
  static short int
      count,
      token;

  if ( dt < 0 )
    WARNING( "NEGATIVE TEXT TYPE NUMBER" );

  if ( dt > 255 )
    WARNING( "TEXT TYPE > 255 " );

  count = 6;
  gds_swap2bytes( (BYTE *) &count );
  write( fd, &count, 2 );
  token = 0x1602;                 // TEXTTYPE
  gds_swap2bytes( (BYTE *) &token );
  write( fd, &token, 2 );
  gds_swap2bytes( (BYTE *) &dt );
  write( fd, &dt, 2 );

}  // write_texttype


/*------------------------------------------------------------------------------------------*/
/* Number of generations to retain. Irrelevant. */
void
gds_read_generations( int fd, int count, BOOL verbose )
{
  static short num;

  read( fd, &num, 2 );
  gds_swap2bytes( (BYTE *) &num );

  if ( verbose ) printf( "             %d\n", num ) ;

  if ( num < 0 )
    WARNING( "NEGATIVE NUMBER OF GENERATIONS TO RETAIN" );

  if ( num > 99 )
    WARNING( "NUMBER OF GENERATIONS > 99 " );

} // read_generations

/*------------------------------------------------------------------------------------------*/

void
gds_write_generations( int fd, short int gens )
{                                       // most useless parameter ever
  static short int
      count,
      token;

  if ( gens < 0 )
    WARNING( "NEGATIVE GENERATIONS NUMBER" );

  if ( gens > 99 )
    WARNING( "GENERATIONS > 99 " );

  count = 6;
  gds_swap2bytes( (BYTE *) &count );
  write( fd, &count, 2 );
  token = 0x2202;                 // GENERATIONS
  gds_swap2bytes( (BYTE *) &token );
  write( fd, &token, 2 );
  gds_swap2bytes( (BYTE *) &gens );
  write( fd, &gens, 2 );

}  // write_generations

/*------------------------------------------------------------------------------------------*/
/* Defines what the ends of paths should look like */
void
gds_read_pathtype( int fd, int count, struct gds_itemtype **ci, BOOL verbose )
{
  static short num;
  static struct gds_itemtype *current_item;
  current_item = *ci;

  read( fd, &num, 2 );
  gds_swap2bytes( (BYTE *) &num );

  if ( verbose ) printf( "             [0x%x] ", num ) ;

  if ( num == 0 )
    { if ( verbose ) printf( " flush square ends" ) ; }
  else if ( num == 1 )
    { if ( verbose ) printf( " round ends" ) ; }
  else if ( num == 2 )
    { if ( verbose ) printf( " extended square ends" ) ; }
  else if ( num == 4 )
    { if ( verbose ) printf( " variable square end extensions ARE NOT SUPPORTED HERE" ) ; }
  else
    WARNING( "UNKNOWN PATH END TYPE" );

  if ( verbose ) printf( "\n" ) ;


  if ( (num > 4) || (num < 0) )
    {
      num = 0;
      WARNING( "INVALID PATH TYPE" );
    }

  current_item->path_end = num;
  *ci = current_item;

}  // read_pathtype

/*------------------------------------------------------------------------------------------*/

void
gds_write_pathtype( int fd, short int pt )
{
  static short int
      count,
      token;

  if ( (pt < 0) || (pt > 4) )
    BAILOUT( "INVALID PATH TYPE NUMBER" );

  count = 6;
  gds_swap2bytes( (BYTE *) &count );
  write( fd, &count, 2 );
  token = 0x2102;                 // PATHTYPE
  gds_swap2bytes( (BYTE *) &token );
  write( fd, &token, 2 );
  gds_swap2bytes( (BYTE *) &pt );
  write( fd, &pt, 2 );

}  // write_pathtype


/*------------------------------------------------------------------------------------------*/
/* Contains the font and orientation of text */
void
gds_read_presentation( int fd, int count, struct gds_itemtype **ci, BOOL verbose )
{
  static short
      num,
      vp,
      hp,
      font;

  static struct gds_itemtype *current_item;
  current_item = *ci;

  read( fd, &num, 2 );
  gds_swap2bytes( (BYTE *) &num );

  if ( verbose ) printf( "             [0x%x] ", num ) ;

  /* extract font from bits "10" and "11", meaning actually bits 5 and 4.  */

  font = ( num >> 4 ) % 4;

  /* extract vertical presentation from bits "12" and "13", meaning actually bits 3 and 2. */

  vp = ( num >> 2 ) % 4;

  /* extract horizontal presentation from bits "14" and "15", meaning acutally bits 1 and 0. */

  hp = num % 4;


  if ( verbose ) printf( "font %d ", font ) ;

  if ( vp == 0 )
    { if ( verbose ) printf( " vertical: top " ) ; }
  else if ( vp == 1 )
    { if ( verbose ) printf( " vertical: middle " ) ; }
  else if ( vp == 2 )
    { if ( verbose ) printf( " vertical: bottom " ) ; }
  else
    { if ( verbose ) printf( " ERROR: VERTICAL PRESENTATION BITS BOTH SET " ) ; }

  if ( hp == 0 )
    { if ( verbose ) printf( " horizontal: left \n" ) ; }
  else if ( hp == 1 )
    { if ( verbose ) printf( " horizontal: center \n" ) ; }
  else if ( hp == 2 )
    { if ( verbose ) printf( " horizontal: right \n" ) ; }
  else
    { if ( verbose ) printf( " ERROR: HORIZONTAL PRESENTATION BITS BOTH SET \n" ); }


  current_item->font = font;
  current_item->mag  = 1.0;
  current_item->hor_present = hp;
  current_item->ver_present = vp;

  *ci = current_item;

}  // read_presentation

/*------------------------------------------------------------------------------------------*/
/* Contains the font and orientation of text */
void
gds_write_presentation( int fd,      // file descriptor
                        int font,    // font number              0, 1, 2, 3
                        int vp,      // vertical presentation    0 = top  1 = middle  2 = bottom
                        int hp )     // horizontal presentation  0 = left 1 = center  2 = right
{
  static unsigned short
      token,
      count,
      num;

  if ( (font < 0) || (font > 3) )
    {
      font = 0;
      WARNING( "INVALID FONT NUMBER" );
    }

  if ( (vp < 0) || (vp > 2) )
    {
      vp = 0;
      WARNING( "INVALID VERTICAL PRESENTATION SENT TO WRITE_PRESENTATION" );
    }

  if ( (hp < 0) || (hp > 2) )
    {
      hp = 0;
      WARNING( "INVALID HORIZONTAL PRESENTATION SENT TO WRITE_PRESENTATION" );
    }


  num = hp + 4*vp + 16*font;

  gds_swap2bytes( (BYTE *) &num );
  count = 6;
  gds_swap2bytes( (BYTE *) &count );
  write( fd, &count, 2 );
  token = 0x1701;
  gds_swap2bytes( (BYTE *) &token );
  write( fd, &token, 2 );
  write( fd, &num, 2 );

}  // write_presentation

/*------------------------------------------------------------------------------------------*/
/* Contains information about transformations */
void
gds_read_strans( int fd,                             // input file descriptor
                 int count,                          // number of bytes in this record
                 struct gds_itemtype **ci,
                 BOOL verbose )
{
  static short
      num,
      bit0,
      bit13,
      bit14;


  static struct gds_itemtype *current_item;
  current_item = *ci;

  read( fd, &num, 2 );
  gds_swap2bytes( (BYTE *) &num );

  if ( verbose ) printf( "             [0x%x] ", num ) ;

  /* bit "0" is really bit 15. It specifies reflection. */
  bit0  = (num >> 15);
  /* bit "13" is really bit 2. It specifies absolute magnification. */
  bit13 = (num >> 2) % 2;
  /* bit "14" is really bit 1. It specifies absolute angle. */
  bit14 = (num >> 1) % 2;

  current_item->abs_angle = FALSE;

  if ( bit0 )
    {
      if ( verbose ) printf( " apply reflection about X before rotation," ) ;
      current_item->reflect = TRUE;
    }
  else
    {
      if ( verbose ) printf( " no reflection," ) ;
      current_item->reflect = FALSE;
    }

  if ( bit13 )
    {
      if ( verbose ) printf( " absolute magnification," ) ;
      current_item->abs_mag = TRUE;
    }
  else
    {
      if ( verbose ) printf( " relative magnification," ) ;
      current_item->abs_mag = FALSE;
    }

  if ( bit14 )
    {
      if ( verbose ) printf( " absolute angle" ) ;
      current_item->abs_angle = TRUE;
    }
  else
    {
      if ( verbose ) printf( " relative angle" ) ;
      current_item->abs_angle = FALSE;
    }

  if ( verbose ) printf( "\n" ) ;
  *ci = current_item;

}  // read_strans

/*------------------------------------------------------------------------------------------*/

void
gds_write_strans( int  fd,            // output file descriptor
                  BOOL reflect,       // reflect about X axis before rotation (normally false)
                  BOOL abs_angle,     // angles are absolute (normally false)
                  BOOL abs_mag  )     // magnification (scale) is absolute (normally false)
{
  static unsigned short int
      count,
      token,
      strans;


  strans = 32768 * reflect + 2 * abs_mag + abs_angle;

  count = 6;
  gds_swap2bytes( (BYTE *) &count );
  write( fd, &count, 2 );
  token = 0x1A01;                 // STRANS
  gds_swap2bytes( (BYTE *) &token );
  write( fd, &token, 2 );
  gds_swap2bytes( (BYTE *) &strans );
  write( fd, &strans, 2 );

}  // write_strans


/*------------------------------------------------------------------------------------------*/
/* Contains X,Y coordinate pairs */
void
gds_read_xy( int fd, int count, float dbu_um, struct gds_itemtype **ci, BOOL verbose )
{
  /* type 0 = polygon, 1 = path, 2 = aref, 3 = sref, 4 = text  */
  static int
      i, ii,
      num,
      x, y,
      px, py,
      degenerates;
  static float
      angle;
  static struct gds_itemtype
      *current_item;

  current_item = *ci;

  num = (count - 4) / 8;

  current_item->x = (int *) malloc( (num + 1) * sizeof( int ) );
  current_item->y = (int *) malloc( (num + 1) * sizeof( int ) );
  if ( ! current_item->x || ! current_item->y ) BAILOUT( "UNABLE TO ALLOCATE MEMORY FOR COORDINATES." );

  degenerates = 0;
  ii = 0;

  for ( i=0; i < num; i++ )
    {
      read( fd, &x, 4 );
      gds_swap4bytes( (BYTE *) &x );
      read( fd, &y, 4 );
      gds_swap4bytes( (BYTE *) &y );
      if ( verbose ) printf( "             (%8d, %8d) = (%9.3f, %9.3f) um \n", x, y, x * dbu_um, y * dbu_um ) ;

      if  ( current_item->type == 3 ||          /* sref */
            current_item->type == 4     )       /* text */
        {
          if ( num != 1 ) BAILOUT( "THERE SHOULD BE ONLY ONE COORDINATE FOR SREF OR TEXT." );
          current_item->x[0] = x;
          current_item->y[0] = y;
        }
      else if ( current_item->type == 2 )       /* aref */
        {
          if ( num != 3 ) BAILOUT( "THERE SHOULD BE THREE COORDINATES FOR AN AREF." );
          if ( i == 0 )
            {                                 /* aref reference point */
              current_item->x[0] = x;
              current_item->y[0] = y;
            }
          else if ( i == 1 )                    /* aref column spacing */
            {
              if ( current_item->cols <= 0 ) BAILOUT( "NUMBER OF COLUMNS IS <= 0" );
              angle = current_item->angle;
              current_item->col_pitch  = (x - current_item->x[0]) / current_item->cols;
              current_item->col_pitchy = (y - current_item->y[0]) / current_item->cols; // hopefully zero
              if ( verbose ) printf( "             Column pitch (points): %d, (%d) \n",
                                     current_item->col_pitch, current_item->col_pitchy );
              if ( current_item->col_pitchy != 0 ) WARNING( "DIAGONAL ARRAY - PROBABLY A MISTAKE" );
            }
          else if ( i == 2 )
            {
              if ( current_item->rows <= 0 ) BAILOUT( "NUMBER OF ROWS IS <= 0" );
              angle = current_item->angle;
              current_item->row_pitch  = (y - current_item->y[0]) / current_item->rows;
              current_item->row_pitchx = (x - current_item->x[0]) / current_item->rows;
              if ( verbose ) printf( "             Row pitch (points):    (%d), %d \n",
                                     current_item->row_pitchx, current_item->row_pitch );
              if ( current_item->row_pitchx != 0 ) WARNING( "DIAGONAL ARRAY - PROBABLY A MISTAKE" );
            }
        }
      else if ( current_item->type == 1 ||       // path
                current_item->type == 0 ||       // polygon
                current_item->type == 5    )     // box
        {
          current_item->x[ii] = x;
          current_item->y[ii] = y;
          if ( i == 0 )
            {
              px = x;
              py = y;
            }
          else     // look for degenerates
            {
              if ( x == px && y == py )
                {
                  ii--;
                  degenerates++;
                  printf( "\nWarning: degenerate vertex  " );
                }
              else
                {
                  px = x;
                  py = y;
                }
            }
        }
      else
        BAILOUT( "INVALID ITEM TYPE FOUND IN XY FUNCTION." );

      ii++;
    }

  current_item->n = num - degenerates;

  if ( current_item->n <= 0 ) printf( "\nWARNING: DEGENERATE POLYGON, PATH, OR BOX. THERE IS ONLY ONE VERTEX.\n" );

  *ci = current_item;

}  // read_xy

/*------------------------------------------------------------------------------------------*/

void
gds_write_xy( int fd, int *x, int *y, int n )
{
  static short int            // If this is a polygon, be sure to repeat the first vertex.
      count,                  // If this is a path, do not repeat.
      token;

  static int i, xx, yy;


  if ( n > 8200 ) BAILOUT( "WAY TOO MANY VERTICIES" );

  count = 4 + 8 * n;
  gds_swap2bytes( (BYTE *) &count );
  write( fd, &count, 2 );
  token = 0x1003;                 // XY
  gds_swap2bytes( (BYTE *) &token );
  write( fd, &token, 2 );

  for ( i=0; i<n; i++ )
    {
      xx = x[i];
      yy = y[i];
      gds_swap4bytes( (BYTE *) &xx );
      gds_swap4bytes( (BYTE *) &yy );
      write( fd, &xx, 4 );
      write( fd, &yy, 4 );
    }

}  // write_xy

/*------------------------------------------------------------------------------------------*/
/* Contains the number of columns and rows of an array. */
void
gds_read_colrow( int fd, int count, struct gds_itemtype **ci, BOOL verbose )
{
  static int
      ncols,
      nrows;

  static struct gds_itemtype *current_item;
  current_item = *ci;

  read( fd, &ncols, 2 );
  gds_swap2bytes( (BYTE *) &ncols );
  read( fd, &nrows, 2 );
  gds_swap2bytes( (BYTE *) &nrows );
  if ( verbose ) printf( "             %d  %d\n", ncols, nrows ) ;

  if ( ncols <= 0 ) BAILOUT( "NEGATIVE OR ZERO NUMBER OF COLUMNS" );
  if ( nrows <= 0 ) BAILOUT( "NEGATIVE OR ZERO NUMBER OF ROWS" );

  current_item->cols = ncols;
  current_item->rows = nrows;
  *ci = current_item;

}  // colrow

/*------------------------------------------------------------------------------------------*/

void
gds_write_colrow( int  fd, int ncols, int nrows )
{
  static unsigned short int
      count,
      token,
      sicols,
      sirows;

  if ( (ncols < 0) || (ncols > 32767) )
    BAILOUT( "NUMBER OF COLUMNS IS INVALID" );

  if ( (nrows < 0) || (nrows > 32767) )
    BAILOUT( "NUMBER OF ROWS IS INVALID" );

  sicols = ncols;
  sirows = nrows;

  count = 8;
  gds_swap2bytes( (BYTE *) &count );
  write( fd, &count, 2 );
  token = 0x1302;                 // COLROW
  gds_swap2bytes( (BYTE *) &token );
  write( fd, &token, 2 );
  gds_swap2bytes( (BYTE *) &sicols );
  write( fd, &sicols, 2 );
  gds_swap2bytes( (BYTE *) &sirows );
  write( fd, &sirows, 2 );

}  // write_colrow

/*------------------------------------------------------------------------------------------*/

void
gds_read_units( int fd, float *Pdbu_um, float *Pdbu_uu, float *Pdbu_m, BOOL verbose )

/* Read the database UNIT in "user units" (typically 0.001), then read the database unit  */
/* in units of meters. The first number doesn't really matter, unless you want a CAD      */
/* program to display the data in convenient units (such as microns).                     */
/* The imporant number is the second one: meters per bit.                                 */


{
  double
      dbu_uu,        /* database unit in user units, usually 0.001 */
      dbu_m;         /* database unit in meters, usually 1e-9      */

  dbu_uu = gds_read_double( fd );

  dbu_m  = gds_read_double( fd );

  if ( dbu_uu <= 0.0 ) BAILOUT( "INVALID DATABASE USER UNIT, < 0" );

  if ( dbu_m <= 0.0 )  BAILOUT( "INVALID DATABASE UNIT, < 0" );


  if ( verbose ) printf( "             Database units in microns: %f\n", dbu_m * 1e6 ) ;

  *Pdbu_um = dbu_m * 1e6;     // microns per database unit
  *Pdbu_uu = dbu_uu;          // user units per database unit
  *Pdbu_m  = dbu_m;           // meters per database unit

}  // read_units

/*------------------------------------------------------------------------------------------*/

void
gds_write_units( int fd, float dbu_uu, float dbu_m )
{
  short int
      count,
      token;
  int i;

  BYTE db_units[] = { 0x3e, 0x41, 0x89, 0x37, 0x4b, 0xc6, 0xa7, 0xf0};
  BYTE user_units[] = { 0x39, 0x44, 0xb8, 0x2f, 0xa0, 0x9b, 0x5a, 0x54};

  count = 20;
  gds_swap2bytes( (BYTE *) &count );
  write( fd, &count, 2 );
  token = 0x0305;                 // UNITS
  gds_swap2bytes( (BYTE *) &token );
  write( fd, &token, 2 );

//Hardcoding db units and gds units, float conversion gives arhitmetic error

  for(i=0;i<8;i++){
	  write(fd, &(db_units[i]), 1);
  }
  for(i=0;i<8;i++){
	  write(fd, &(user_units[i]), 1);
  }
//  gds_write_float( fd, dbu_uu );
//  gds_write_float( fd, dbu_m );

}  // write_units

/*------------------------------------------------------------------------------------------*/

void
gds_read_mag( int fd, int count, struct gds_itemtype **ci, BOOL verbose )
{
  double magnification;
  static struct gds_itemtype *current_item;
  current_item = *ci;

  magnification = gds_read_double( fd );

  if ( verbose ) printf( "             %g \n", magnification ) ;

  current_item->mag = magnification;
  *ci = current_item;

}  // read_mag

/*------------------------------------------------------------------------------------------*/

void
gds_write_mag( int fd, float mag )
{
  static int
      count,
      token;

  count = 12;
  gds_swap2bytes( (BYTE *) &count );
  write( fd, &count, 2 );
  token = 0x1B05;   // MAG
  gds_swap2bytes( (BYTE *) &token );
  write( fd, &token, 2 );
  gds_write_float( fd, mag );

}  // write_mag

/*------------------------------------------------------------------------------------------*/

void
gds_read_angle( int fd, int count, struct gds_itemtype **ci, BOOL verbose )
{

  double
      a;

  static struct gds_itemtype *current_item;
  current_item = *ci;

  a = gds_read_double( fd );

  if ( verbose ) printf( "             %g \n", a ) ;

  current_item->angle = a;

  *ci = current_item;

}  // read_angle

/*------------------------------------------------------------------------------------------*/

void
gds_write_pos(int fd){
  long pos = lseek(fd, 0, SEEK_CUR);
  printf("pos is %#08x %i bytes\n", pos,pos);
}

void
gds_write_angle( int fd, float angle )
{
  static int
      count,
      token;

  //long pos;
//  pos = ftell(fd);

  count = 12;
  gds_swap2bytes( (BYTE *) &count );
  write( fd, &count, 2 );
  token = 0x1C05;   // ANGLE
  gds_swap2bytes( (BYTE *) &token );
  write( fd, &token, 2 );
  gds_write_float( fd, angle );


}  // write_angle


/*------------------------------------------------------------------------------------------*/
//  HIGHER LEVEL FUNCTIONS
/*------------------------------------------------------------------------------------------*/


void
gds_create_lib( int fd, char *libname, float dbu_um )
{
  // Write HEADER, BGNLIB, LIBNAME, and UNITS.


  float
      dbu_uu,        // database user units, 1 nm per bit
      dbu_m;         // database unit in meters, usually 1e-9m


  dbu_uu = 0.001;
  dbu_m  =  1.0e-9;

  gds_write_header( fd );
  gds_write_bgnlib( fd );
  gds_write_libname( fd, libname );
  gds_write_units( fd, dbu_uu, dbu_m );

}  // create_lib



/*------------------------------------------------------------------------------------------*/

void
gds_create_text( int fd, char *str, int x, int y, int layer, int size )
{
  static int xx[1], yy[1];

  // generate text centered at x,y

  gds_write_text( fd );
  gds_write_layer( fd, layer );
  gds_write_texttype( fd, 0 );
  gds_write_presentation( fd, 0, 1, 0 );  // fd, font=0, vp=center, hp=left
  gds_write_width( fd, size );
  gds_write_strans( fd, 0, 0, 0 );        // fd, reflect, abs_angle, abs_mag
  xx[0] = x;
  yy[0] = y;
  gds_write_xy( fd, xx, yy, 1 );
  gds_write_string( fd, str );
  gds_write_endel( fd );

} // create_text

/*------------------------------------------------------------------------------------------*/
