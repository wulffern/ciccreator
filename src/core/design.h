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
//#include <QMetaType>
#include "core/consoleoutput.h"
#include "core/patterntransistor.h"
#include "core/patterncapacitor.h"
#include "core/layoutcell.h"
#include "spice/spiceparser.h"
namespace cIcCore{

    class Design: public Cell
    {
        Q_OBJECT
		
    public:
        Design();
        void read(QString filename);
		QList<QString> cellNames(){return _cell_names;}

    private:
        void runIfObjectCanMethods(Cell * c, QJsonObject jobj, QString theme = "");
        void runAllMethods(QString jname, Cell *c, QJsonObject jobj);
        void runAllParentMethods(QString jname, Cell *c, QList<QJsonObject> * parents);
        void runParentsIfObjectCanMethods(Cell * c, QList<QJsonObject> * parents, QString theme = "");
        void comment(QString str);

        QHash<QString,QString> cellTranslator;
		QHash<QString,QString> nameTranslator;
		void findAllParents(QList<QJsonObject > *reverse_parents,QString inh);
		void createCell(QJsonObject jobj);
		cIcSpice::SpiceParser _spice_parser;
		QMap<QString,QJsonObject>  _cells;
		QList<QString> _cell_names;
		ConsoleOutput * console;

    signals:

    };
}
#endif
