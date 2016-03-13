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


#ifndef CIC_PRINTER_SVG
#define CIC_PRINTER_SVG

#include <QObject>
#include <QString>
#include "printer/designprinter.h"
namespace cIcPrinter{
  using namespace cIcCore;

  class Svg : public DesignPrinter{

  public:

    Svg(QString filename):DesignPrinter(filename){

    }
    ~Svg(){

    }

    virtual void startCell(Cell * cell);
    virtual void endCell();
    virtual void printPort(Port *);
    virtual void printRect(Rect * rect);
    virtual void printReference(Cell * o);



    
  };
};

#endif
