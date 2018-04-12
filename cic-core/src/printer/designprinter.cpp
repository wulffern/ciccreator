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


    DesignPrinter::DesignPrinter(QString filename){
        this->filename = filename;
    }

    DesignPrinter::~DesignPrinter(){

    }

    char *  DesignPrinter::toChar(QString str){
        char* cstr;
        std::string fname = str.toStdString();
        cstr = new char [fname.size()+1];
        strcpy( cstr, fname.c_str() );
        return cstr;
    }



    void DesignPrinter::openFile(QString filename){
        file.setFileName(filename);
        file.open(QIODevice::WriteOnly );

    }

    void DesignPrinter::closeFile(){
        file.close();
    }

    

    void DesignPrinter::printChildren(QList<Rect*> children){
        foreach(Rect * child,children){
            if(!child){continue;}
            if(child->isInstance()){
                Instance * inst = (Instance*)child;
                if(inst->name() == ""){continue;}
                this->printReference((Instance*)child);
            }else if(child->isPort()){
                Port * p = (Port *) child;
                if(p->spicePort){
                    this->printPort(p);
                }
            }else if(child->isText()){
                Text * p = (Text *) child;
                this->printText(p);
            }else if (child->isCell()){
                Cell * c = (Cell * ) child;
                this->printChildren(c->children());

            }else{
                this->printRect(child);
            }

        }

    }

    void DesignPrinter::printCell(Cell * c){

        if(Cell::isEmpty(c)){return ;}

        this->startCell(c);

        this->printChildren(c->children());


        this->endCell();
    }

    void DesignPrinter::startLib(QString name){
        this->openFile(name);
    }

    void DesignPrinter::endLib(){
        this->closeFile();
    }





    void DesignPrinter::print(Design * d ){
        this->print(d,"");
    }

    void DesignPrinter::print(Design * d, QString stopcell ){

        ConsoleOutput* console = new ConsoleOutput();
        this->stopcell = stopcell;
        this->startLib(this->filename);
        bool skip = false;
        QList<QString> cells = d->cellNames();

        for(int i=0;i<cells.count();i++){
            if(skip) {

                continue;
            }


            Cell * c = Cell::getCell(cells[i]);
            if(c ){
                this->printCell(c);
            }
            if("" != stopcell && cells[i] == stopcell){

                skip = true;
            }

        }

        console->comment("Written " + stopcell + " to " + this->filename,ConsoleOutput::green);
        this->endLib();

    }

    //}


};
