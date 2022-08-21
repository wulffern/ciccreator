//====================================================================
//        Copyright (c) 2016 Carsten Wulff Software, Norway 
// ===================================================================
// Created       : wulff at 2016-6-13
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

#include "routering.h"

namespace cIcCore{


	RouteRing::RouteRing(){
        this->setBoundaryIgnoreRouting(false);
	}
	
	RouteRing::RouteRing(QString layer,QString name, Rect * size,QString location, int xgrid, int ygrid,int metalwidth ){

		//Adjust rectangle to get correct size;
		if(location.contains("b")){
			size->adjust(0,-ygrid,0,0);
		}
		if(location.contains("t")){
			size->adjust(0,0,0,ygrid);
		}
		if(location.contains("r")){
			size->adjust(0,0,xgrid,0);
		}
		if(location.contains("l")){
			size->adjust(-xgrid,0,0,0);
		}

        this->setName(name);

		int x1 = size->x1();
		int y1 = size->y1();
		int x2 = size->x2();
		int y2 = size->y2();


		bottom = new Rect(layer,x1,y1,x2-x1,metalwidth);
		left   = new Rect(layer,x1,y1,metalwidth,y2-y1);
		right  = new Rect(layer,x2-metalwidth,y1,metalwidth,y2-y1);
		top    = new Rect(layer,x1,y2-metalwidth,x2-x1,metalwidth);

		if(location.contains("b")){
			this->add(bottom);
            default_rectangle = bottom;
		}

		if(location.contains("t")){
			this->add(top);
            default_rectangle = top;
		}

		if(location.contains("l")){
			this->add(left);
            default_rectangle = left;
		}

		if(location.contains("r")){
			this->add(right);
            default_rectangle = right;
		}
		
	}

	RouteRing::~RouteRing(){

	}

    Rect* RouteRing::getDefault()
    {
        return default_rectangle;
    }


    void RouteRing::translate(int dx, int dy) {
//        bottom->translate(dx,dy);
//        left->translate(dx,dy);
        //       top->translate(dx,dy);
//        right->translate(dx,dy);
        Cell::translate(dx,dy);
//        emit updated();
    }

    void RouteRing::moveTo(int ax, int ay) {
        int x1 = this->x1();
        int y1 = this->y1();

        ax = ax - x1;
        ay = ay - y1;
//        bottom->translate(ax,ay);
//        left->translate(ax,ay);
//        top->translate(ax,ay);
//        right->translate(ax,ay);
        Cell::moveTo(ax,ay);        
//        emit updated();
    }



	Rect* RouteRing::get(QString location){
		if(location == "bottom"){
			return bottom->getCopy();
		}else if(location == "top"){
			return top->getCopy();
		}else if(location == "right"){
			return right->getCopy();
		}else if(location == "left"){
			return left->getCopy();
		}
        return NULL;
	}

    Rect* RouteRing::getPointer(QString location){
		if(location == "bottom"){
			return bottom;
		}else if(location == "top"){
			return top;
		}else if(location == "right"){
			return right;
		}else if(location == "left"){
			return left;
		}else{
            qDebug() << "Could not find location =" << location << "on" << this->name() << ". Use top,bottom,left,right";
            return 0;
        }
	}

    void RouteRing::trimRouteRing(QString location,QString whichEndToTrim)
    {

        QList<Rect*> cuts = this->getChildren("cIcCore::Route");
        Rect bounds = Cell::calcBoundingRect(cuts);



        Rect* r = this->getPointer(location);
        if(r == 0){
            return;
        }
        if(whichEndToTrim.contains("l")){
            r->setLeft(bounds.x1());
        }

        if(whichEndToTrim.contains("t")){
            r->setTop(bounds.y2());
        }

        if(whichEndToTrim.contains("r")){
            r->setRight(bounds.x2());
        }

        if(whichEndToTrim.contains("b")){
            r->setBottom(bounds.y1());
        }

    }
    



}
