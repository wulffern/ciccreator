//====================================================================
//        Copyright (c) 2015 Carsten Wulff Software, Norway
// ===================================================================
// Created       : wulff at 2015-7-6
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


#ifndef CIC_SPICE_SUBCKT_H
#define CIC_SPICE_SUBCKT_H

#include <QtCore>
#include <QObject>
#include "spiceobject.h"
#include "subcktinstance.h"
#include "spicedevice.h"
#include "mosfet.h"
#include "resistor.h"
#include <QString>
#include <QMap>
#include <iostream>

namespace cIcSpice{
    using namespace std;
    class Subckt: public SpiceObject
    {
        Q_OBJECT
        Q_PROPERTY(QString name READ name WRITE setName)

        public:
            Subckt();
            Subckt(QList<QString> buffer);
            Subckt(const Subckt&);
            ~Subckt();

            virtual QJsonObject toJson();
            virtual void fromJson(QJsonObject o);


            static Subckt* getInstanceSubckt(SubcktInstance*);
            void parse(QList<QString> buffer, int line);
            QList<SubcktInstance*> instances(){ return _instances;}
            QList<SpiceDevice*> devices(){ return _devices;}
            void setLibPath(QString path){_lib_path = path; }
            QString libPath(){return _lib_path;}

            void add(SubcktInstance* s)
            {
                _instances.append(s);
            }

            void add(SpiceDevice* s)
            {
                _devices.append(s);
            }

            SubcktInstance* getInstance(QString name);


            void addSubckt()
            {
                _allsubckt[this->name()] = this;
            }








        protected:
            static QMap<QString,Subckt*> _allsubckt;

        private:
            QHash<QString,int> _inst_index;
            QList<SubcktInstance*> _instances;
            QList<SpiceDevice*> _devices;
            QString _lib_path;


    };


}


#endif
