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


#ifndef CIC_PRINTER_TIKZ_H
#define CIC_PRINTER_TIKZ_H

#include <QObject>
#include <QString>
#include "printer/designprinter.h"
#include <cmath>

namespace cIcPrinter{
  using namespace cIcCore;
  using namespace std;
  class Tikz : public DesignPrinter{
    public:
        Tikz(QString filename):DesignPrinter(filename){
        }
        ~Tikz(){
        }

        virtual void startCell(Cell * cell);
        virtual void endCell();
        virtual void printPort(Port *);
        virtual void printRect(Rect * rect);
        virtual void printReference(Cell * o);
	
	virtual void startLib(QString name);
//        virtual void endLib();
	
	double toTikz(int angstrom);
	QString getCellName(QString name);
	QHash<QString,QString> block_map;
	QList<QString> height_map;

	QString cellname;

  };
};

#endif
