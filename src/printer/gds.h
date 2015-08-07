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


#ifndef CIC_PRINTER_GDS
#define CIC_PRINTER_GDS



#include <QObject>
#include <QString>
#include "printer/designprinter.h"
#include "libgds.h"

namespace cIcPrinter{
    using namespace cIcCore;

    class Gds : public DesignPrinter{

    public:

        Gds(QString filename);
        ~Gds(){

        }


    private:
        virtual void startCell(Cell * cell);
        virtual void openFile(QString file);
        virtual void closeFile();
        virtual void endCell();
        virtual void startLib(QString name) ;
        virtual void endLib() ;
//      virtual void printText(Text & text);

        //TODO: add port printing
//         virtual void printPin(Pin & pin);
        virtual void printRect(Rect * rect);
        virtual void printReference(Cell * o);
        int fd,        x[5],        y[5];
    };
};

#endif
