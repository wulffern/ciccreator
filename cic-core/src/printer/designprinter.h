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
#include "core/text.h"

namespace cIcPrinter{
    using namespace cIcCore;

  class DesignPrinter : public QObject{

  public:

    protected:
        Cell * current_cell;
        QString filename;
        QFile  file;
	QString stopcell;

    public:
        explicit DesignPrinter(QString filename);
        ~DesignPrinter();
        char * toChar(QString str);
        bool isEmpty(Cell * c);
        void print(Design * o);
	void print(Design * o,QString stopcell);

        virtual void openFile(QString file);
        virtual void closeFile();
        virtual void printCell(Cell * c);

        virtual void startCell(Cell * ){};
        virtual void endCell() {};
        virtual void printPort(Port *) {} ;
	    virtual void printText(Text *) {} ;
        virtual void printRect(Rect * ) {};
        virtual void printReference(Cell *) {};
        virtual void printChildren(QList<Rect*> children);

        virtual void startLib(QString name);
        virtual void endLib();
        //   virtual void printText(Text & text) = 0;




    };
};

#endif
