//====================================================================
//        Copyright (c) 2016 Carsten Wulff Software, Norway
// ===================================================================
// Created       : wulff at 2016-5-19
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


#include "spice/capacitor.h"
namespace cIcSpice{


        

    Capacitor::Capacitor()
    {
        init(QStringList()<< "A" << "B");
    }
    
    Capacitor::Capacitor(QStringList nodes){
        init(nodes);
        
    } 

    void Capacitor::init(QStringList nodes)
    {
        
        this->deviceName_ = "mres";
        this->spiceType_ = "X";
        this->setNodes(nodes);


        for(int i=0;i< nodes.count();i++){
            QString node = _nodes[i];
            SubcktInstance* ins = new SubcktInstance();
            QStringList sn;
            sn.append(node);
            sn.append(QString("%1%2").arg("NC").arg(i));
            ins->setNodes(sn);
            ins->setName(QString("%1%2").arg("R").arg(i));
            ins->setDeviceName("resistor");
            instances.append(ins);
        }

    }


    Capacitor::Capacitor(const Capacitor& cap){

    }

    Capacitor::~Capacitor(){

    }


    QString Capacitor::toSpice( QString instance, QStringList nodes){
        QString s;
        QTextStream ts(&s);

        cIcCore::Rules * rules = cIcCore::Rules::getRules();
        cIcCore::Device * mtype = rules->getDevice(this->deviceName());


        for(int i=0;i< nodes.count();i++){
            QString node = nodes[i];

            ts << "R" << instance << i << " " << node << " NC" << i << " " << mtype->name << "\n";


        }
        return s;
    }

    QJsonObject Capacitor::toJson()
    {


        cIcCore::Rules * rules = cIcCore::Rules::getRules();
        cIcCore::Device * mtype = rules->getDevice(this->deviceName());


        foreach(SubcktInstance* ins,instances){
            ins->setSubcktName(mtype->name);
        }
        
        
        QJsonObject o = SpiceDevice::toJson();


        return o;
    }


}
