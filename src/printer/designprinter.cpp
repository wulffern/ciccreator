//====================================================================
//        Copyright (c) 2015 Carsten Wulff Software, Norway 
// ===================================================================
// Created       : wulff at 2015-7-14
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

#include "printer/designprinter.h"


namespace cIcPrinter{


    void DesignPrinter::startCell(Cell * o){
    }

    void DesignPrinter::printReference(Cell * o){
    }

    void DesignPrinter::openFile(QString filename){
        file.setFileName(filename);
        file.open(QIODevice::WriteOnly );

    }

    void DesignPrinter::closeFile(){
        file.close();
        //if(!stream.is_open()){
        //    stream.close();
        // }

    }

    void DesignPrinter::printCell(Cell * c){

        this->startCell(c);

        foreach(Rect * child, c->children()){
            if(strcmp(child->metaObject()->className(),"cIcCore::Instance") == 0){
                this->printReference((Cell*)child);
            }else{
                this->printRect(child);
            }

        }

        this->endCell();
    }

    void DesignPrinter::print(Design * d ){

		
        foreach(Cell * r, Cell::getAllCells() ){
            this->printCell(r);
        }


    }

    //}


};
