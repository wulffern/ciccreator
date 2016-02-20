
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
	  name_ = "";
	  routeLayer_ = 0;
    }

	Port::~Port(){
    }

	Port::Port(QString name){
	  name_ = name;
	  routeLayer_ = 0;
	}

    QString Port::name(){return name_;}

	void Port::setName(QString name){name_ = name;}

	QString Port::pinLayer(){if(routeLayer_){return routeLayer_->pin;}else{return "";}}
	
//	void Port::setChild(Port * p){
//		childport_ = p;
//		parent_ = parent;
//		Rect * r = p->getCopy();
//		r->translate(parent->x1(),parent->y1());
//		this->set(r);

//        }



    void Port::set(Rect * r ){
      if(!r){return;};
          Layer * l = rules->getLayer(r->layer());
          routeLayer_ = l;
          alternates_rectangles_.append(r);
          this->setLayer(l->name);
          rect_ = r;
          connect(r,SIGNAL(updated()),this, SLOT(updateRect()));
          this->setRect(r->layer(),r->x1(),r->y1(),r->width(),r->height());

//           qDebug() << r->toString();
    }

    void Port::updateRect(){
        this->setRect(rect_->x1(),rect_->y1(),rect_->width(),rect_->height());
    }

	Rect * Port::get(){
		Rect* r = 0;

		if(routeLayer_){
		    QString layer = routeLayer_->name;
		  r = this->getCopy(layer);
		}
		return r;
	}

	Rect * Port::get(QString layer){
	  foreach(Rect* r,alternates_rectangles_){
	      if(r->layer() == layer){
		  return r->getCopy();
		}
	    }
	  return NULL;
	}

	void Port::add(Rect *r){
	  alternates_rectangles_.append(r);

	}


}


