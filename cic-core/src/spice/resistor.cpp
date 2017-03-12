//====================================================================
//        Copyright (c) 2016 Carsten Wulff Software, Norway 
// ===================================================================
// Created       : wulff at 2016-9-4
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



#include "spice/resistor.h"
namespace cIcSpice{

    Resistor::Resistor(QStringList n){
        this->deviceName_ = "mres";
        this->spiceType_ = "X";
        this->setNodes(n);
    }

    Resistor::Resistor(const Resistor& cap){

    }

    Resistor::~Resistor(){

    }

    QString Resistor::toSpice( ){
        return toSpice("1",nodes());
   } 

    QString Resistor::toSpice( QString instance, QStringList nodes){
        QString s;
        QTextStream ts(&s);

        cIcCore::Rules * rules = cIcCore::Rules::getRules();
        cIcCore::Device * mtype = rules->getDevice(this->deviceName());

        ts << "R" << instance << " " << nodes.join(" ") << " "<< mtype->name << "\n";

        return s;
    }

    QJsonObject Resistor::toJson()
    {


        cIcCore::Rules * rules = cIcCore::Rules::getRules();
        cIcCore::Device * mtype = rules->getDevice(this->deviceName());

        QList<SubcktInstance*> instances;
        SubcktInstance* ins = new SubcktInstance();
        QStringList sn;
        sn.append("A");
        sn.append("B");
        ins->setNodes(sn);
        ins->setName("R1");
        ins->setDeviceName("resistor");
        ins->setSubcktName(mtype->name);
        instances.append(ins);


        QJsonObject o = SpiceDevice::toJson();        

        return o;



    }
    

}
