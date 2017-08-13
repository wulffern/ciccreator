//====================================================================
//        Copyright (c) 2015 Carsten Wulff Software, Norway 
// ===================================================================
// Created       : wulff at 2015-8-20
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

#ifndef CIC_PRINTER_GEDA_H
#define CIC_PRINTER_GEDA_H

#include "printer/designprinter.h"
#include "core/cell.h"
#include "geda/gobject.h"
#include "geda/gEdaFileWriter.h"

namespace cIcPrinter{

class gEda : public DesignPrinter
{
public:
  gEda(QString name);
  ~gEda();

    virtual void openFile(QString filename);
    virtual void closeFile();
    
    
  virtual void startCell(Cell * cell);
  virtual void endCell();
  virtual void printRect(Rect *rect);
  virtual void printReference(Cell *c);
  virtual void printPort(Port *);


protected:
  bool subcktInPrint;
    QFile currentFile;

    int xref;
    int yref;
    QMap<QString,int> symbolWidth;
    QMap<QString,int> symbolHeight;
    GSchematic * sch;
    cIcSpice::Subckt * ckt;
    gEdaFileWriter * gwriter;
    
    
    
    

};

}

#endif // SPICE_H
