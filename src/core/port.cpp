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

#include "core/port.h"


namespace cIcCore{
	Port::Port(){

    }

	Port::~Port(){

    }

	Port::Port(QString name){
	  name_ = name;
	  childport_ = 0;
	}

    QString Port::name(){return name_;}

	void Port::setName(QString name){name_ = name;}
	
	void Port::setChild(Port * p,Rect * parent){
		childport_ = p;
		parent_ = parent;
		this->set(p);

	}

    void Port::set(Rect * r ){
          Layer * l = rules->getLayer(r->layer());
          this->setRect(l->pin,r->x1(),r->y1(),r->width(),r->height());
    }
}


