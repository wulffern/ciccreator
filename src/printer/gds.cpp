//====================================================================
//        Copyright (c) 2015 Carsten Wulff Software, Norway
// ===================================================================
// Created       : wulff at 2015-7-15
// ===================================================================
//   This program is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation, either version 3 of the License, or
//   (at your option) any later version.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with this program.  If not, see <http://www.gnu.org/licenses/>.
//====================================================================

#include "printer/gds.h"

namespace cIcPrinter{

  Gds::Gds(QString filename):DesignPrinter(filename){

  }

  void Gds::startLib(QString name){
    openFile(name + ".gds");
    gds_create_lib( fd, this->toChar(name), 0.001 /* um per bit */ );
  }

  void Gds::endLib(){
    gds_write_endlib( fd );
    closeFile();
  }


  void Gds::printPort(Port * p){

    gds_write_text( fd );
    gds_write_layer( fd, Rules::getRules()->layerToNumber(p->layer()) );
    gds_write_texttype( fd, Rules::getRules()->layerToDataType(p->layer()) );
    gds_write_presentation( fd, 0, 0, 0 );  // fd, font, hp, vp
    gds_write_width( fd, 1 );
    gds_write_strans( fd, 0, 0, 0.1 );        // fd, reflect, abs_angle, abs_mag
    x[0] = toNano(p->x1());
    y[0] = toNano(p->y1());
    gds_write_xy( fd, x, y, 1 );
    gds_write_string( fd, this->toChar(p->name()) );
    gds_write_endel( fd );

    qWarning() << "printing port " << p->name();

  }

  void Gds::printReference(Cell * o){
    if(this->isEmpty(o)){return ;}
    char * name = this->toChar(o->name());
    gds_write_sref( fd );                    // contains an instance of...
    gds_write_sname( fd, name );

    ///TODO: Implement rotations
    //              gds_write_angle( fd, 0.0 );             // and tilted at some weird angle
    x[0] =  toNano(o->x());
				   y[0] = toNano(o->y());
    gds_write_xy( fd, x, y, 1 );             // at these coordinates (database units)
    gds_write_endel( fd );                   // end of element
  }

  void Gds::printRect(Rect * o){

    gds_write_boundary( fd );       // write just the token
    gds_write_layer( fd, Rules::getRules()->layerToNumber(o->layer()) );       // layer 0, for example
    gds_write_datatype( fd, Rules::getRules()->layerToDataType(o->layer()) );    // datatype 1, for example

    x[0] = toNano(o->x1());  y[0] = toNano(o->y1());       // signed four-byte integers
	x[1] = toNano(o->x2());  y[1] = toNano(o->y1());
    x[2] = toNano(o->x2());  y[2] = toNano(o->y2());       // in this example 1 integer unit = 1 nm
	x[3] = toNano(o->x1());  y[3] = toNano(o->y2());
																				  x[4] = toNano(o->x1());  y[4] = toNano(o->y1());       // required repetition of first point (yup, that's stupid)

    gds_write_xy( fd, x, y, 5 );    // polygon, four vertices, first vertex repeated => 5 points
    gds_write_endel( fd );          // end of element
  };


  void Gds::startCell(Cell *cell){

    gds_write_bgnstr( fd );
    gds_write_strname( fd, this->toChar(cell->name()));
    //- draw PR boundary
    this->printRect(cell);

  }

  void Gds::endCell(){
    gds_write_endstr( fd );                  // end of structure (cell)
  }


  void Gds::openFile(QString file){
    fd = open( this->toChar(file), O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH );
  }

  void Gds::closeFile(){

    close( fd );
  }

};
