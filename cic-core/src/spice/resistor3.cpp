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



#include "spice/resistor3.h"
namespace cIcSpice{

    Resistor3::Resistor3(){
        this->deviceName_ = "rppo";
        this->spiceType_ = "X";
        QStringList n;
        n << "N" << "P" << "B" ;
        this->setNodes(n);
    }

    Resistor3::Resistor3(const Resistor3& cap){

    }

    Resistor3::~Resistor3(){

    }


    QString Resistor3::toSpice( QString instance, QStringList nodes){
        QString s;
        QTextStream ts(&s);

        cIcCore::Rules * rules = cIcCore::Rules::getRules();
        cIcCore::Device * mtype = rules->getDevice(this->deviceName());

        ts << "X" << instance << " N P B " << mtype->name << "\n";

        return s;
    }

}
