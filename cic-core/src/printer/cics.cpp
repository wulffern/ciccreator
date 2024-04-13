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

#include "cics.h"

namespace cIcPrinter{

    Cics::Cics(QString name):DesignPrinter(name + ".cics")
    {
        subcktInPrint = false;
    }

    Cics::~Cics()
    {
    }


    void Cics::startLib(QString name)
    {
        DesignPrinter::startLib(name);
    }

    void Cics::endLib()
    {

        QTextStream ts(&file);
        QJsonDocument  d(ar);
        ts << d.toJson();
        DesignPrinter::endLib();        

    }
    
    
    void Cics::startCell(Cell * cell){

        if(!cell){return;}
        if(cell->isCut()){return;};
        cIcSpice::Subckt * ckt = cell->subckt();

        if(!ckt){return;}

        QTextStream ts(&file);
        QJsonObject o = ckt->toJson();
        o["class"] = cell->metaObject()->className();
        ar.append(o);
        this->subcktInPrint = true;
            
    }

    void Cics::endCell(){
        subcktInPrint = false;

    }

    void Cics::printRect(Rect *rect){
    }

    void Cics::printPort(Port *){
    }

    void Cics::printReference(Cell *c){
    }



}
