//====================================================================
//        Copyright (c) 2016 Carsten Wulff Software, Norway
// ===================================================================
// Created       : wulff at 2016-11-04
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

#include "printer/eps.h"

namespace cIcPrinter{

    void Eps::printPort(Port * port){
        QTextStream ts(&file);
    }

    double Eps::toEps(int angstrom){
        return angstrom/10000.0;
        ;}

    void Eps::printReference(Cell * o){
        QTextStream ts(&file);
        Instance * i = (Instance *) o;
        QString name = this->getCellName(i->name());

        double ix = i->x1();
        double iy = i->y1();

        QString angle;
        if(i->angle() == "R90"){
            angle = "90 rotate\n";
//            ix += -i->height() - i->cell()->y1();
            ix += i->cell()->y2();
            

        }else if(i->angle() == "MX"){
            angle = "";
        }else if(i->angle() == "MY"){
            angle = "-1 1 scale\n";
            ix += i->width() + i->cell()->x1();
        }
        

        ix = toEps(ix);
        iy = toEps(iy);
        ts << "gsave\n";
        ts << ix << " " << iy << " translate\n";
        ts << angle;
        ts << name << "\n";
        ts << "grestore\n";
    }

    void Eps::printRect(Rect * o){
        QTextStream ts(&file);
        double x1 = toEps(o->x1());
        double y1 = toEps(o->y1());
        double x2= toEps(o->x2());
        double y2 = toEps(o->y2());

        Layer *l = Rules::getRules()->getLayer(o->layer());

        if( l->color == "" ){return;}
        QColor color(l->color);
        double blue = color.blue();
        double red = color.red();
        double green = color.green();

        ts << "newpath\n";
        ts << red/256.0 <<" " << green/256.0 << " "  << blue/256.0 << " setrgbcolor\n";

        ts << x1 << " " << y1 << " " << "moveto\n";
        ts << x2 << " " << y1 << " " << "lineto\n";
        ts << x2 << " " << y2 << " " << "lineto\n";
        ts << x1 << " " << y2 << " " << "lineto\n";
        ts << "closepath\n";
        if(!l->nofill)
            ts << "gsave fill grestore\n";
    };

    void Eps::startCell(Cell *cell){
        DesignPrinter::startCell(cell);
        if(cell == NULL) return;
        QTextStream ts(&file);

        ts << "/" << this->getCellName(cell->name()) << "{\n";
        ts << "gsave\n";
    }

    void Eps::endCell(){
        QTextStream ts(&file);
        ts << "grestore\n";
        ts << "} def\n";


    }



    void Eps::startLib(QString name){
        DesignPrinter::startLib(name + ".eps");
        QTextStream ts(&file);
        ts << "%!PS-Adobe-3.0 EPSF-3.0\n";
        ts << "%%Creator: cic2eps \n";
        Cell * c = Cell::getCell(name);
        if(c){
        c->setBoundaryIgnoreRouting(false);
        c->updateBoundingRect();
        
        double xmargin = c->width()*0.01;
        double ymargin = c->height()*0.01;

        double margin = ymargin < xmargin ? ymargin: xmargin;
        
        
            ts << "%%BoundingBox: " << int(toEps(c->x1() -margin)) << " "
               << int(toEps(c->y1()-margin)) << " "
               << int(toEps(c->x2()+margin)) << " "
               << int(toEps(c->y2()+margin)) << "\n";
            ts << "%%HiResBoundingBox: " << toEps(c->x1()-margin) << " "
               << toEps(c->y1()-margin) << " "
               << toEps(c->x2()+margin) << " "
               << toEps(c->y2()+margin) << "\n";
        }

        ts << "%%BeginProlog\n";
    }

    void Eps::endLib(){
        QTextStream ts(&file);
        ts << "%%EndProlog\n";
        ts << " "<< this->getCellName(this->stopcell) <<" \n";
        ts << "showpage \n";
    }


    QString Eps::getCellName(QString name){
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




};
