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


#ifndef CIC_SPICE_SUBCKTINSTANCE_H
#define CIC_SPICE_SUBCKTINSTANCE_H

#include <QtCore>
#include "spiceobject.h"
#include <iostream>
#include <QJsonObject>

namespace cIcSpice{
    using namespace std;
    class SubcktInstance: public SpiceObject
    {
        Q_OBJECT
        Q_PROPERTY(QString subcktName READ subcktName WRITE setSubcktName)


    public:
        SubcktInstance();
        SubcktInstance(QString buffer);
        SubcktInstance(const SubcktInstance&);
        ~SubcktInstance();
        virtual QString setName(QString val);
        QString subcktName(){return _subckt_name;}
        QString setSubcktName(QString val){ _subckt_name = val;
            return _subckt_name;}
        QString groupName(){return _group_name;}
        QString groupTag(){return _group_tag;}
        void parse(QString buffer,int line_number);
        virtual QJsonObject toJson();
        virtual void fromJson(QJsonObject o);
        
    private:

        QString _subckt_name;
        QString _group_name;
        QString _group_tag;


    };


}


#endif
