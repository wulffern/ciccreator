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

#include "spice/spiceobject.h"

namespace cIcSpice{

    SpiceObject::SpiceObject(){

    }

    SpiceObject::~SpiceObject(){

    }

    SpiceObject::SpiceObject(const SpiceObject&){

    }

    QString SpiceObject::name(){return _name;}
    QString SpiceObject::setName(QString val){ _name = val; return _name;}

    QList<QString> SpiceObject::spiceStr(){return _spice_str;}
    QList<QString> SpiceObject::setSpiceStr(QList<QString> val){ _spice_str = val; return _spice_str;}

    QStringList SpiceObject::nodes(){return _nodes;}
    QStringList SpiceObject::setNodes(QStringList val){ _nodes = val; return _nodes;}

    QMap<QString,QString> SpiceObject::properties(){
        return _properties;
    }

    QString SpiceObject::spiceType(){return spiceType_;}
    QString SpiceObject::deviceName(){return deviceName_;}
    void SpiceObject::setDeviceName(QString name){deviceName_ = name;}


    QString SpiceObject::toSpice( QString instance, QStringList nodes){ QString s; QTextStream ts(&s); ts << instance << " " << nodes.join(' ') ; return s;}

    int SpiceObject::lineNumber(){return _line_number;}
    int SpiceObject::setLineNumber(int val){ _line_number = val; return _line_number;}

}
