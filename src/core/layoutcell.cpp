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
#include "core/layoutcell.h"

namespace cIcCore{

    LayoutCell::LayoutCell(): Cell()
    {
		useHalfHeight = false;
    }

    LayoutCell::LayoutCell(const LayoutCell& cell): Cell(cell){
		useHalfHeight = false;
    }

    LayoutCell::~LayoutCell()
    {
		useHalfHeight = false;
    }

    void LayoutCell::setYoffsetHalf(QJsonValue obj){
          useHalfHeight = true;
    }

    void LayoutCell::noPowerRoute(QJsonValue obj){
      noPowerRoute_ = true;
    }

	void LayoutCell::addDirectedRoute(QJsonArray obj){

		if(obj.size() < 3){
			qDebug() << "Error: addDirectedRoute must contain at least three elements\n";
			return;
		}

		QString layer = obj[0].toString();
		QString net = obj[1].toString();
		QString route = obj[2].toString();
		QString startRegex;
		QString routeType;
		QString stopRegex;
		QList<Rect*> start;
		QList<Rect*> stop;

		//Decode route
		QRegularExpression re("^([^-\\|<>]*)([-\\|<>]+)([^-\\|<>]*)$");
		QRegularExpressionMatch m_route = re.match(route);
		if(m_route.hasMatch()){
			startRegex = m_route.captured(1);
			routeType = m_route.captured(2);
			stopRegex = m_route.captured(3);
			
		start = this->findRectanglesByRegex(startRegex,layer);
		stop = this->findRectanglesByRegex(stopRegex,layer);
			
		}else{
			qDebug() << "Error: Could not parse route command '" << route << "'\n";
			return;			
		}
		
		QString options = "";
		if(obj.size() > 3){
			options = obj[3].toString();
		}

		if(start.count() > 0 && stop.count() > 0){

		    //qDebug() << layer << net << route << options;
			Route * r = new Route(net,layer,start,stop,options,routeType);
			r->setPoint1(this->x1(),this->y1());
			routes_.append(r);
		      this->add(r);
		}
	}

    void LayoutCell::place(){

        QString prev_group = "";
        int prev_width = 0;
        int x = 0;
        int y = 0;
        foreach(cIcSpice::SubcktInstance * ckt_inst,_subckt->instances()){
            QString group = ckt_inst->groupName();

            if(prev_group.compare(group) != 0  && prev_group.compare("")  != 0){
                y = 0;
                x = x + prev_width;
            }
            prev_group = group;

			//The chain of events is important here, ports get defined in the setSubckInstance
            Instance * inst = Instance::getInstance(ckt_inst->subcktName());
            inst->setSubcktInstance(ckt_inst);
            this->add(inst);
            inst->moveTo(x,y);
            prev_width = inst->width();
            if(useHalfHeight){
                y += inst->height()/2;
              }else{
                y += inst->height();
              }

        }

        this->updateBoundingRect();

    }

    void LayoutCell::route(){

	foreach(Route *route, routes_){
	    route->route();
	 }



    }

    void LayoutCell::paint(){
      if(!noPowerRoute_){
        this->routePower();
        }

    }

    void LayoutCell::routePower(){

     // qWarning() << "searching power nets";
      QList<Rect*> avss = this->findRectanglesByRegex("AVSS","M1");
      QList<Rect*> avdd = this->findRectanglesByRegex("AVDD","M1");
      QList<Rect*> empty;
//      qWarning() << "Found AVSS" << avss.length() << " Found AVDD" << avdd.length();

      Route * ravdd = new Route("AVDD","M4",avdd,empty,"","-|-");
      ravdd->addStartCuts();
      this->add(ravdd);
      Rect r_avdd =  this->calcBoundingRect(avdd);
      r_avdd.setTop(this->top());
      r_avdd.setBottom(this->bottom());
      r_avdd.setWidth(ravdd->width());
      r_avdd.setLayer("M4");

      Route * ravss = new Route("AVDD","M4",avss,empty,"","-|-");
      ravss->addStartCuts();
      this->add(ravss);

      Rect r_avss =  this->calcBoundingRect(avss);
      r_avss.setTop(this->top());
      r_avss.setBottom(this->bottom());
      r_avss.setWidth(ravss->width());
      r_avss.setLayer("M4");


      this->add(new Rect(r_avdd));
      this->add(new Rect(r_avss));


    }

	void LayoutCell::addAllPorts(){

		QStringList nodes = _subckt->nodes();

		foreach(Rect* child, children()){
		    if(!(child->isInstance())){ continue;}
		    Instance * inst = (Instance *) child;
		    foreach(Port * p, inst->ports()){
			if(p == NULL){continue;}
			if(!nodes.contains(p->name())){continue;}
			if(ports_.contains(p->name())){continue;}
			if(p->name().contains(QRegularExpression("AVSS|AVDD")) && p->childName() == "B"){continue;}
			Port * pi = new Port(p->name());
			pi->setChild(p,inst);
			this->add(pi);
		      }
		}





	}

}
