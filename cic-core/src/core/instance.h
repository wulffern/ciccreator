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
#ifndef CIC_CORE_INSTANCE_H
#define CIC_CORE_INSTANCE_H

#include <QObject>
#include "cell.h"
#include "port.h"
#include "spice/spiceobject.h"
#include <iostream>

namespace cIcCore{

        class Instance : public Cell
        {

                Q_OBJECT

                public:
                        Instance();
                        Instance(const Instance&);
                        ~Instance();
                        Cell * cell(){return _cell;}
                        QString instanceName(){return instanceName_;}
                        QString id()
                        {
                                return name()  + instanceName_;
                        }

                        Rect calcBoundingRect() override;
                        QString angle(){return angle_;}
                        void setAngle(QString angle);

                        // Cell * addInstance(QString cell);
                        static Instance * getInstance(QString cell);
                        void setCell(Cell*cell){_cell = cell;}
                        cIcSpice::SubcktInstance *subcktInstance(){return ckt_inst_;}
                        void setSubcktInstance(cIcSpice::SubcktInstance *inst);

                        QList<Rect*> findRectanglesByRegex(QString regex,QString layer) override;
                        QList<Rect*> findRectanglesByNode(QString node, QString filterChild);
                        QString toString() override;

                        void transform(Rect* r);

                        Rect* getRect(QString layer);
                        Point* getCellPoint();

                        QJsonObject toJson() override;
                        void fromJson(QJsonObject o) override;
                        void setCell(QString cell);
                        void updateUsedChildren() override;

                private:
                        Cell * _cell;
                        cIcSpice::SubcktInstance * ckt_inst_;
                        QMap<QString,Port*> instancePorts_;
                        QString angle_;
                        int xcell;
                        int ycell;
                        QList<InstancePort*> instance_ports_;


        };

}

Q_DECLARE_METATYPE(cIcCore::Instance)

#endif // INSTANCE_H
