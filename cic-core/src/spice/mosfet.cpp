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
      numberOfFingers = 1;
      drainWidth = 0;
      sourceWidth = 0;
      width = 0;
      length = 0;
      multiplier = 1;
      this->deviceName_ = "nch";
      this->spiceType_ = "M";
      QStringList n;
      n << "D" << "G" << "S" << "B";
      this->setNodes(n);
    }

	Mosfet::Mosfet(const Mosfet& mos){

    }

	Mosfet::~Mosfet(){

    }


     QString Mosfet::toSpice( QString instance, QStringList nodes){
      QString s;
      QTextStream ts(&s);

      cIcCore::Rules * rules = cIcCore::Rules::getRules();
      cIcCore::Device * mtype = rules->getDevice(this->deviceName());

	  

      if(mtype){
            ts << "M" << instance << " " << nodes.join(' ') <<  " " << mtype->name << " w=" << width << "u l=" << length << "u nf=" << numberOfFingers << " M=" << multiplier;
        }else{
                ts << "M" << instance << " " << nodes.join(' ') <<  " " << this->deviceName() << " w=" << width << "u l=" << length << "u nf=" << numberOfFingers << " M=" << multiplier;
        }

    return s;
    }

}



