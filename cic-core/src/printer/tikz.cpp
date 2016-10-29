//====================================================================
//        Copyright (c) 2016 Carsten Wulff Software, Norway
// ===================================================================
// Created       : wulff at 2016-10-29
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

#include "printer/tikz.h"

namespace cIcPrinter{

    void Tikz::printPort(Port *){

    }

    double Tikz::toTikz(int angstrom){
      return angstrom/10000.0;
      ;}
  
    void Tikz::printReference(Cell * o){
        QTextStream ts(&file);
        if(!o->isInstance()) return;
        Instance * i = (Instance *) o;
	
	int lcount = count;
	count ++;

	ts << " % TBD :  ";
        double x = toTikz(o->x1());
        double y = toTikz(o->y1());
	ts << " (" <<  x << ", " << y << ")";
	ts << " node[ " << o->name() << "]\n";
    }



    void Tikz::printRect(Rect * o){
        QTextStream ts(&file);
        double x = toTikz(o->x1());
        double y = toTikz(o->y1());
	double x2= toTikz(o->x2());
	double y2 = toTikz(o->y2());

	Layer *l = Rules::getRules()->getLayer(o->layer());
	
	if( l->color == "" ){return;}
	ts << " \\filldraw [" << l->color <<",opacity=0.5 ] ";

	
        ts << " (" << x << "," << y << ") rectangle (" << x2 << "," << y2 << ");\n";
	//        QString map = "blocks.oak";
        //if(block_map.contains(o->layer())){
        //    map = block_map[o->layer()];

        //    int height = 0;
        //    if(height_map.contains(o->layer())){
        //        height = height_map.indexOf(o->layer());
        //    }
        //    ts << ".up(" << height << ")";
        //    ts <<  ".box(" << map << "," << toMine(o->width())  << ",1," << toMine(o->height()) << ")";
        //}

        //ts << ".move('r" << count << "');\n";
	//	count++;
    };


    void Tikz::startCell(Cell *cell){
        DesignPrinter::startCell(cell);
        if(cell == NULL) return;
        QTextStream ts(&file);
        cellname = cell->name();
	cellname.replace("_","");
	cellname.replace("0","ten");
	cellname.replace("1","one");
	cellname.replace("2","two");
	cellname.replace("3","three");
	cellname.replace("4","four");
	cellname.replace("5","five");
	cellname.replace("6","six");
	cellname.replace("7","seven");
	cellname.replace("8","eight");
	cellname.replace("9","nine");
        ts << "\n\n";
	ts << "\\newcommand{\\fig"<<cellname <<"}{\n";

    }
  
  void Tikz::startLib(QString name){
    DesignPrinter::startLib(name);
    QTextStream ts(&file);
    
  }

  
  void Tikz::endCell(){
        QTextStream ts(&file);
        ts << "}\n";
  }


};
