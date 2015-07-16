//====================================================================
//        Copyright (c) 2015 Carsten Wulff Software, Norway
// ===================================================================
// Created       : wulff at 2015-7-14
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
#include "core/instance.h"

namespace cIcCore{

    Instance::Instance()
    {

    }

    Instance::Instance(const Instance&)
    {

    }


    Instance::~Instance()
    {

    }

	Rect Instance::calcBoundingRect(){
		Rect r = this->_cell->calcBoundingRect();
		r.moveTo(this->x1()+r.x1(), this->y1() + r.y1());
		return r;
	}

	Instance * Instance::getInstance(QString cell){

    Instance * c = new Instance();

    if(Cell::_allcells.contains(cell)){
        c->_cell  = _allcells[cell];
        c->setName(c->_cell->name());
        Rect r = c->_cell->calcBoundingRect();
        //	qWarning() << r.toString();
		c->setLayer("PR");
        c->updateBoundingRect();
//        c->setRect(r);
        //qWarning() << c->toString();
        //qWarning() << "Adding instance " << c->name();
     }
     return c;
  }

//Cell * Instance::addInstance(QString cell){
//     Cell * c = Cell::getInstance(cell);
//     this->_instances.append(c);
//     connect(c,SIGNAL(updated()),this,SLOT(updateBoundingRect()));
//     return c;
//}

}
