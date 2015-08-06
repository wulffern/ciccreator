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


  char *  DesignPrinter::toChar(QString str){
    char* cstr;
    std::string fname = str.toStdString();
    cstr = new char [fname.size()+1];
    strcpy( cstr, fname.c_str() );
    return cstr;
  }



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
  }

  bool DesignPrinter::isEmpty(Cell * c){
    if(c->name() == ""){
        Cell * r = (Cell * ) c->parent();
        if(r != 0){
            qWarning() << "Error: Empty cell-name encountered in " <<  r->name() ;
          }
        return true;
      }
    return false;
  }

  void DesignPrinter::printCell(Cell * c){

    if(this->isEmpty(c)){return ;}

    this->startCell(c);

    foreach(Rect * child, c->children()){
        if(!child){continue;}

        if(child->isInstance()){
            Instance * inst = (Instance*)child;
            if(inst->name() == ""){continue;}
            this->printReference((Instance*)child);
          }else{
            this->printRect(child);
          }

      }

    this->endCell();
  }

  void DesignPrinter::startLib(QString name){
       this->openFile(name);
  }

  void DesignPrinter::endLib(){
       this->closeFile();
  }

  void DesignPrinter::print(Design * d ){

  this->startLib(this->filename);

    QList<QString> cells = d->cellNames();
    for(int i=0;i<cells.count();i++){
        Cell * c = Cell::getCell(cells[i]);
        if(c){
            this->printCell(c);
          }
      }

    this->endLib();

  }

  //}


};
