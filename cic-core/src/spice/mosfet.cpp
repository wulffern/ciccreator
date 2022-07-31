//====================================================================
//        Copyright (c) 2015 Carsten Wulff Software, Norway
// ===================================================================
// Created       : wulff at 2015-8-20
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

#include "spice/mosfet.h"

namespace cIcSpice{

    Mosfet::Mosfet(){
        this->setDeviceName("nch");
        this->setSpiceType("M");
        this->setName("M1");
        QStringList n;
        n << "D" << "G" << "S" << "B";
        this->setNodes(n);
        setProperty("width",1);
        setProperty("length",1);
        setProperty("nf",1);
        setProperty("multiplier",1);
    }

    Mosfet::Mosfet(const Mosfet& mos){

    }

    Mosfet::~Mosfet(){

    }


    QString Mosfet::toSpice(){
        return this->toSpice("1",nodes());
    }


    QString Mosfet::toSpice( QString instance, QStringList nodes){
        QString s;
        QTextStream ts(&s);

        cIcCore::Rules * rules = cIcCore::Rules::getRules();
        cIcCore::Device * mtype = rules->getDevice(this->deviceName());


        if(mtype){
            ts << mtype->devicetype << instance << " " << nodes.join(' ') <<  " " << mtype->name ;
        }else{
            ts << "M" << instance << " " << nodes.join(' ') <<  " " << this->deviceName() ;

        }

        double w = _properties["width"].toDouble()*rules->spiceUnit();
        double l = _properties["length"].toDouble()*rules->spiceUnit();

        ts << " w=" << w << " l=" << l << " nf=" << getPropertyString("nf") << " M=" << getPropertyString("multiplier") << "\n";

        return s;
    }

    QJsonObject Mosfet::toJson()
    {

//        cIcCore::Rules * rules = cIcCore::Rules::getRules();
//        cIcCore::Device * mtype = rules->getDevice(this->deviceName());
        QJsonObject o = SpiceDevice::toJson();
        return o;
    }

    void Mosfet::fromJson(QJsonObject o){
        SpiceDevice::fromJson(o);
    }

}
