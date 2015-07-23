
#ifdef __cplusplus
extern "C" {
#endif

#ifndef EXT_LIBGDS
#define EXT_LIBGDS



#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

                            // Setting BYTESWAP to 1 is appropriate for big-endian Intel processors. 
                            // GDS format was originally used on little-endian, older computers.
#define BYTESWAP 1
#define BYTE unsigned char

#define BOOL  int
#define TRUE  1
#define FALSE 0



#define SKIPOVER( fd, count )  { for ( i=0; i < ((count)-4); i+=2 ) read( (fd), &gdsword, 2 ); }
#define BAILOUT( message )     { printf( "\n\nERROR: %s\n\n", (message) ); fflush(stdout); exit(-1); }
#define WARNING( message )     { printf( "\n                            WARNING: %s\n\n", (message) ); fflush(stdout); }
 

#define MAX_FORWARD_REFS 1000


struct gds_itemtype
    {                                /* an item might be a polygon, path, aref, sref or text                      */
    int   type;                      /* 0 = polygon, 1 = path, 2 = aref, 3 = sref, 4 = text, 5 = box              */
    int   n;                         /* in the case of polygons or paths, this is the number of verticies         */
                                     /*     for text this is the number of characters in the string               */
    int   layer;                     /* layer                                                                     */
    int   dt;                        /* datatype                                                                  */ 
    int   cell_number;               /* index into the table of cells- relevant for sref and aref                 */
    float mag;                       /* magnification- relevant to sref, aref and text                            */
    float angle;                     /* the angle - relevant to sref and aref                                     */
    BOOL  abs_angle;                 /* from strans - normally false                                              */
    BOOL  abs_mag;                   /* from strans - normally false                                              */
    BOOL  reflect;                   /* from strans (reflect over x axis before rotating)                         */
    int   cols;			     /* Yes, many of these items are relevant to only one type of item, so        */
    int   rows;			     /* perhaps we should invent a different item type for each item, then        */
    int   col_pitch;		     /* string them together in a linked list of items.  Why not?                 */
    int   row_pitch;		     /* Because the "library" has to be a linked list of one "thing". What we     */
    int   col_pitchy;                /* An array's column pitch in y, which would create a diagonal array.        */
    int   row_pitchx;                /* An array's row pitch in x. Diagonal arrays are strange and useless.       */
    int   path_end;		     /* 0 = flush, 1 = round, 2 = extended. Default 0.                            */
    int   hor_present;		     /* The horizontal presentation for text.                                     */
    int   ver_present;		     /* The vertical presentation for text.                                       */
    int   font;			     /* Also relevant only for text.                                              */
    int   width;                     /* Relevant only to paths.                                                   */
    int  *x;                         /* array of x coordinates or possibly just the reference point X             */
    int  *y;                         /* array of y coordinates or possibly just the reference point Y             */
    char *text;                      /* Used only for strings. Such a waste.                                      */
    struct gds_itemtype *nextitem;       
    };                               
                                       

struct gds_celltype                  /* A GDS library is a linked list of cells.                                  */
    {                                /* A cell is a linked list of items.                                         */
    char *name;                      /* name of the cell                                                          */
    struct gds_itemtype *item;       /* one element of the cell                                                   */
    struct gds_celltype *nextcell;   /* pointer to the next cell, forming a linked list                           */  
    };


char *gds_read_libname( int fd, int count, BOOL verbose );
void  gds_read_header( int fd, int count, BOOL verbose );
void  gds_swap4bytes( BYTE *four  );
void  gds_swap2bytes( BYTE *two );
void  gds_read_header( int fd, int count, BOOL verbose );
void  gds_write_header( int fd );
void  gds_read_bgnlib( int  fd, int count, int *cell_table_size, struct gds_celltype **lib, 
                       struct gds_celltype ***c, struct gds_celltype **cc,  BOOL verbose );
void  gds_write_bgnlib( int fd );
void  gds_read_bgnstr( int fd, int count, int *cell_table_size, int *num_cells, struct gds_celltype ***c, 
                       struct gds_celltype **cc, struct gds_itemtype **ci, BOOL verbose );
void  gds_write_bgnstr( int fd );
void  gds_read_endlib( int fd, int count, BOOL verbose );
void  gds_write_endlib( int fd );
void  gds_read_endstr( int fd, int count, BOOL verbose );
void  gds_write_endstr( int fd );
void  gds_write_libname( int fd, char *name );
void  gds_read_strname( int fd, int count, int num_cells, struct gds_celltype ***c, 
                        struct gds_celltype **cc, BOOL verbose );
void  gds_write_strname( int fd, char *name );
void  gds_read_string( int fd, int count, struct gds_itemtype **ci, BOOL verbose );
void  gds_write_string( int fd, char *s );
void  gds_read_sname( int fd, int count, int num_cells, int *forward_num, char ***f, 
                      struct gds_celltype ***c, struct gds_itemtype **ci, BOOL verbose );
void  gds_write_sname( int fd, char *s );
void  gds_read_boundary( int fd, int count, struct gds_itemtype **ci, BOOL verbose );
void  gds_write_boundary( int fd );
void  gds_read_box( int fd, int count, struct gds_itemtype **ci, BOOL verbose );
void  gds_write_box( int fd );
void  gds_read_boxtype( int fd, int count, struct gds_itemtype **ci, BOOL verbose );
void  gds_write_boxtype( int fd, short int dt );
void  gds_read_path( int fd, int count, struct gds_itemtype **ci, BOOL verbose );
void  gds_write_path( int fd );
void  gds_read_sref( int fd, int count, struct gds_itemtype **ci, BOOL verbose );
void  gds_write_sref( int fd );
void  gds_read_aref( int fd, int count, struct gds_itemtype **ci, BOOL verbose );
void  gds_write_aref( int fd );
void  gds_read_text( int fd, int count, struct gds_itemtype **ci, BOOL verbose );
void  gds_write_text( int fd );
void  gds_read_endel( int fd, int count, BOOL verbose );
void  gds_write_endel( int fd );
void  gds_read_layer( int fd, int count, struct gds_itemtype **ci, BOOL verbose );
void  gds_write_layer( int fd, short int layer );
void  gds_read_width( int fd, int count, struct gds_itemtype **ci, BOOL verbose );
void  gds_write_width( int fd, int width );
void  gds_read_datatype( int fd, int count, struct gds_itemtype **ci, BOOL verbose );
void  gds_write_datatype( int fd, short int dt );
void  gds_read_texttype( int fd, int count, struct gds_itemtype **ci, BOOL verbose );
void  gds_write_texttype( int fd, short int dt );
void  gds_read_generations( int fd, int count, BOOL verbose );
void  gds_write_generations( int fd, short int gens );
void  gds_read_pathtype( int fd, int count, struct gds_itemtype **ci, BOOL verbose );
void  gds_write_pathtype( int fd, short int pt );
void  gds_read_presentation( int fd, int count, struct gds_itemtype **ci, BOOL verbose );
void  gds_write_presentation( int fd, int font, int vp, int hp );
void  gds_read_strans( int fd, int count, struct gds_itemtype **ci, BOOL verbose );
void  gds_write_strans( int  fd, BOOL reflect, BOOL abs_angle, BOOL abs_mag  );
void  gds_read_xy( int fd, int count, float dbu_um, struct gds_itemtype **ci, BOOL verbose );
void  gds_write_xy( int fd, int *x, int *y, int n );
void  gds_read_colrow( int fd, int count, struct gds_itemtype **ci, BOOL verbose );
void  gds_write_colrow( int  fd, int ncols, int nrows );
void  gds_read_units( int fd, float *Pdbu_um, float *Pdbu_uu, float *Pdbu_m, BOOL verbose );
void  gds_write_units( int fd, float dbu_uu, float dbu_m );
void  gds_read_mag( int fd, int count, struct gds_itemtype **ci, BOOL verbose );
void  gds_write_mag( int fd, float mag );
void  gds_read_angle( int fd, int count, struct gds_itemtype **ci, BOOL verbose );
void  gds_write_angle( int fd, float angle );
void  gds_create_lib( int fd, char *libname, float dbu_um );    
void  gds_create_text( int fd, char *str, int x, int y, int layer, int size );



#endif

#ifdef __cplusplus
}

#endif
