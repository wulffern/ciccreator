/*********************************************************************
 *        Copyright (c) 2015 Carsten Wulff Software, Norway
 * *******************************************************************
 * Created       : wulff at 2015-1-26
 * *******************************************************************
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ********************************************************************/



#ifndef CIC_CORE_DESIGN_H
#define CIC_CORE_DESIGN_H

#include <QtCore>
#include <QObject>
#include <QMap>
#include <QString>
#include <QJsonObject>
#include "core/consoleoutput.h"
#include "core/patterntransistor.h"
#include "core/patterncapacitor.h"
#include "core/layoutcell.h"
#include "core/layoutrotatecell.h"
#include "spice/spiceparser.h"
#include <exception>
#include <iostream>

namespace cIcCore{

    class Design: public Cell
    {
        Q_OBJECT

    public:
        Design();

		//Read a JSON file and compainon SPICE file
        void read(QString filename);

		//List of cells in the design
        QList<QString> cellNames(){return _cell_names;}

        cIcSpice::Subckt * getSpiceSubckt(QJsonObject jobj, QList<QJsonObject>* reverse_parents, QString name);
        void runMethod(QJsonValue v, QMetaMethod m, Cell* c);
    private:

		//Check if an object (c) can run methods defined in object (jobj)
        void runIfObjectCanMethods(Cell * c, QJsonObject jobj, QString theme = "");

		//Check if an object (c) can run methods defined in parents
        void runParentsIfObjectCanMethods(Cell * c, QList<QJsonObject> * parents, QString theme = "");
		
		//Run all methods defined under name (jname) in object (jobj)
        void runAllMethods(QString jname, Cell *c, QJsonObject jobj);

		//Check all parents (parents) for methods matching name (jname)
        void runAllParentMethods(QString jname, Cell *c, QList<QJsonObject> * parents);

		//Guess, it's quite obvious
        void comment(QString str);

		//List of conversions between ciccreator object names and cnano (Perl version) object names
	QMap<QString,QString> cellTranslator;

		//List of conversion between ciccreator method names and cnano method names
	QMap<QString,QString> nameTranslator;

		//Search through JSON and find all parents
        void findAllParents(QList<QJsonObject > *reverse_parents,QString inh);

		//Make a cell, main entry point after reading JSON file
        void createCell(QJsonObject jobj);

		//Yes, it's the parser object for spice
        cIcSpice::SpiceParser _spice_parser;

		//List of all the cells, QString = cellname
        QMap<QString,QJsonObject>  _cells;

		//List of all cellnames
        QList<QString> _cell_names;

		//Output object for console
        ConsoleOutput * console;

    signals:

    };
}
#endif
