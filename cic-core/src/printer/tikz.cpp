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

    void Tikz::printPort(Port * port){
      QTextStream ts(&file);

      QString name = port->name();
      name = name.replace("_","\\_");
      ts << "\\ifthenelse{\\boolean{cicaddtext}}{\\draw ("<< toTikz(port->x1()) << ","<< toTikz(port->y1()) << ") node [anchor=north west,cicfill] {\\textbf{$"<< name <<"$}};}{}\n";
    }

    double Tikz::toTikz(int angstrom){
      return angstrom/10000.0;
      ;}
  
    void Tikz::printReference(Cell * o){
        QTextStream ts(&file);
        if(!o->isInstance()) return;
        Instance * i = (Instance *) o;


	double ix = i->x1();
    double icenterx = i->centerX();
    double icentery = i->centerY();
	double iy = i->y1();
	
	QString angle;
	if(i->angle() == "R90"){
	  
	}else if(i->angle() == "MX"){
	  angle = ", yscale=-1";
	}else if(i->angle() == "MY"){
	  angle = ", xscale=-1";
	  ix += i->width();
      icenterx += i->width();
	}
	ts << "\\begin{scope}[shift={("<< toTikz(ix) << "," << toTikz(iy) << ")} " << angle << "]\n";
	ts << "\\setboolean{cicaddtext}{false}\n";
        double x = toTikz(o->x1());
        double y = toTikz(o->y1());
	ts << "\\fig" << this->getCellName(o->name()) << "\n";
	ts << "\\setboolean{cicaddtext}{true}\n";
	ts << "\\end{scope}\n";

    ts << "\\ifthenelse{\\boolean{cicaddref}}{\\draw ("<< toTikz(icenterx) << ","<< toTikz(icentery) << ") node [anchor=center,cicfill] {\\textbf{$"<< i->instanceName() <<"$}};}{}\n";
    

    }



    void Tikz::printRect(Rect * o){
        QTextStream ts(&file);
        double x = toTikz(o->x1());
        double y = toTikz(o->y1());
	double x2= toTikz(o->x2());
	double y2 = toTikz(o->y2());

	Layer *l = Rules::getRules()->getLayer(o->layer());


	if( l->color == "" ){return;}
	
	if(height_map.contains(o->layer()))
	  ts << "\\begin{pgfonlayer}{" << o->layer() << "}\n";

	
	if( l->nofill){
	  ts << " \\draw [" << l->color <<"] ";
	}else{
	  ts << " \\filldraw [" << l->color <<"] ";
	}
        ts << " (" << x << "," << y << ") rectangle (" << x2 << "," << y2 << ");\n";

	if(height_map.contains(o->layer()))
	  ts << "\\end{pgfonlayer}\n";
    };

  QString Tikz::getCellName(QString name){
    QString n = name;
    n.replace("_","");
    n.replace(".","");
    n.replace("0","ten");
    n.replace("1","one");
    n.replace("2","two");
    n.replace("3","three");
    n.replace("4","four");
    n.replace("5","five");
    n.replace("6","six");
    n.replace("7","seven");
    n.replace("8","eight");
    n.replace("9","nine");
    return n;
  }



  
  void Tikz::startCell(Cell *cell){
        DesignPrinter::startCell(cell);
        if(cell == NULL) return;
	cellname = this->getCellName(cell->name());
        QTextStream ts(&file);
        ts << "\n\n";
	ts << "\\newcommand{\\fig"<<cellname <<"}{\n";

    }


  
  void Tikz::startLib(QString name){
    DesignPrinter::startLib(name);
    QTextStream ts(&file);

    
    
    ts << "\\newcommand{\\lib"<<this->getCellName(name) <<"}{\n";

    ts << " \\newboolean{cicaddtext} \n \\setboolean{cicaddtext}{true}  \n";
    ts << " \\newboolean{cicaddref} \n \\setboolean{cicaddref}{false}  \n";
//    ts << "\\tikzstyle{stuff_fill}=[rectangle,fill=white,minimum size=1.4em,opacity=0.7]\n";
    

    
    foreach(QString s,height_map){
      ts << "\\pgfdeclarelayer{" << s << "}\n";
    }

    ts << "\\pgfsetlayers{";
    for(int i=0;i < height_map.count();i++){
      QString s = height_map[i];
      ts  << s << ",";
    }
    ts << "main" << "}\n";
    ts << "}\n";
    
  }

  
  void Tikz::endCell(){
        QTextStream ts(&file);
        ts << "}\n";
  }


};
