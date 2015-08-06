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

#ifndef CIC_PRINTER_DESIGNPRINTER
#define CIC_PRINTER_DESIGNPRINTER

#include <QObject>
#include <QFile>
#include <QTextStream>
#include "core/design.h"
#include <string.h>

namespace cIcPrinter{
    using namespace cIcCore;
    class DesignPrinter : public QObject{
    private:


    protected:
        Cell * current_cell;
        QString filename;
        QFile  file;

//        ofstream  stream;

    public:

        DesignPrinter(QString filename){
            this->filename = filename;

        }

        char * toChar(QString str);


        virtual ~DesignPrinter(){

        }


        virtual void add(QString & str){
            QTextStream s(&file);
            s << str;
        }


        virtual void add(int i){
            QTextStream s(&file);
            s << i;
//            if(this->stream.is_open()){
//                this->stream << i;
//            }
        }


        void print(Design * o);

        virtual void openFile(QString file);
        virtual void closeFile();

        bool isEmpty(Cell * c);

        virtual void startCell(Cell * cell);
        virtual void endCell() = 0;
        virtual void startLib(QString name);
        virtual void endLib();
//      virtual void endFile()  = 0;
        //     virtual void printInstance(Instance & inst)  = 0;
        //     virtual void printInitLib(string name) = 0;
        //    virtual void printEndLib() = 0;
        //   virtual void printInitCell(Cell & cell) = 0;
        //   virtual void printEndCell() = 0;
        //   virtual void printText(Text & text) = 0;
        //  virtual void printPin(Pin & pin) = 0;
        virtual void printRect(Rect * rect) = 0;
        virtual void printCell(Cell * c);
        virtual void printReference(Cell *c);



    };
};

#endif
