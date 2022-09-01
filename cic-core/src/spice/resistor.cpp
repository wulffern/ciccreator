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
        init(n);
    }

    Resistor::Resistor(){
        init(QStringList() << "A" << "B");
    }


    Resistor::Resistor(const Resistor& cap){
        
    } 

    void Resistor::init(QStringList n)
    {
        this->deviceName_ = "mres";
        this->spiceType_ = "X";
        this->setNodes(n);
        this->setName("R1");

    }
    

    Resistor::~Resistor(){

    }

    QString Resistor::toSpice( ){
        return toSpice(this->name(),nodes());
   } 

    QString Resistor::toSpice( QString instance, QStringList nodes){
        QString s;
        QTextStream ts(&s);

        cIcCore::Rules * rules = cIcCore::Rules::getRules();

        QString deviceName = this->deviceName();
        if(this->hasProperty("layer")){
            deviceName += this->getPropertyString("layer");
        }

        cIcCore::Device * mtype = rules->getDevice(deviceName);

        QString type  = this->deviceName();
        QString dtype = "X";
        if(!mtype){
            qWarning() << "ERROR: Could not find resistor type for " << this->deviceName() << " in rule file";
    }else{
            dtype = mtype->devicetype;
            type = mtype->name;
        }



        ts << dtype << instance << " " << nodes.join(" ") << " "<< type << " w=" << this->getPropertyString("width") << " l=" << this->getPropertyString("length")<< "\n";

        return s;
    }

    QJsonObject Resistor::toJson()
    {

        QList<SubcktInstance*> instances;
        SubcktInstance* ins = new SubcktInstance();
        QStringList sn;
        sn.append("A");
        sn.append("B");
        ins->setNodes(sn);
        ins->setName("R1");
        ins->setDeviceName("resistor");
        instances.append(ins);

        QJsonObject o = SpiceDevice::toJson();        

        return o;
    }

    void Resistor::fromJson(QJsonObject o){
        SpiceDevice::fromJson(o);
    }
    

}
