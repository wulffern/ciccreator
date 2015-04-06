/*********************************************************************
 *        Copyright (c) 2015 Carsten Wulff Software, Norway
 * *******************************************************************
 * Created       : wulff at 2015-1-26
 * *******************************************************************
 * Licensed under the Apache License, Version 2.0 (the "License")
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ********************************************************************/



#ifndef CIC_CORE_DESIGN_H
#define CIC_CORE_DESIGN_H

#include <QtCore>
#include <QObject>
#include <QString>
#include <QJsonObject>
//#include <QMetaType>
#include "core/patterntransistor.h"
namespace cIcCore{

    class Design: public QObject
    {
        Q_OBJECT

		//typedef Cell * (*fp)();
		
    public:
        Design();
        void read(QString filename);
        void runIfObjectCanMethods(Cell * c, QJsonObject jobj);
        void runAllMethods(QString jname, Cell *c, QJsonObject jobj);
		

    private:
        QHash<QString,QString> cellTranslator;
      QHash<QString,QString> nameTranslator;
		QHash<QString,Cell> _designs;
		void findAllParents(QList<Cell *> reverse_parents,QString inh);
		void createCell(QString cl, QJsonObject jobj);

    signals:

    };
}
#endif
