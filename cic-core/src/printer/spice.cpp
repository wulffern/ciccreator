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

#include "spice.h"

namespace cIcPrinter{

  Spice::Spice(QString name):DesignPrinter(name + ".spi")
  {
    subcktInPrint = false;

  }

  Spice::~Spice()
  {
  } 

  void Spice::startCell(Cell * cell){

    if(!cell){return;}
    if(cell->isCut()){return;};
    cIcSpice::Subckt * ckt = cell->subckt();

    if(!ckt){return;}

    //I think these are devices
    //    if(ckt->name() == ""){return;}
    QTextStream ts(&file);
    ts << "\n";
    ts << "*-------------------------------------------------------------\n";
    ts << "* " << cell->name() << " (" << cell->metaObject()->className() <<")\n";
    ts << "*-------------------------------------------------------------\n";
    if(ckt->name() != ""){
        ts << ".SUBCKT " << ckt->name() << " " << ckt->nodes().join(' ')  << "\n";
        foreach(SpiceDevice* sd,ckt->devices()){
            ts << sd->toSpice();
        }
        
      }
    this->subcktInPrint = true;

  }

  void Spice::endCell(){
    if(subcktInPrint){
        QTextStream ts(&file);
        ts << ".ENDS \n";
      }
    subcktInPrint = false;

  }

  void Spice::printRect(Rect *rect){

  }

  void Spice::printPort(Port *){

  }

  void Spice::printReference(Cell *c){

    Instance * i = (Instance *) c;
    SubcktInstance *si = i->subcktInstance();
    Cell * cell =   i->cell();

    if(!cell){return;}

    QTextStream ts(&file);

    if(si){
//        ts << so->toSpice(si->name(), si->nodes()) << "\n";
//      }else{
		QString instname = si->name();
		if(instname.startsWith("M")){
			instname = "X" + instname;
		}
        ts << instname << " " << si->nodes().join(' ') << " " << si->subcktName() << "\n";

      }
  }



}

