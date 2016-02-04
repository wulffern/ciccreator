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
	     // foreach(Rect* r, route->children()){
	//		this->add(r);
	//	}
	 }

    }

	void LayoutCell::addAllPorts(){

		QStringList nodes = _subckt->nodes();

		//QMap<QString,Port*> porthash;
		//QMap<QString,Instance*> instancehash;
		foreach(Rect* child, children()){
		    if(!(child->isInstance())){ continue;}
		    Instance * inst = (Instance *) child;
		    foreach(Port * p, inst->ports()){
			if(p == NULL){continue;}
			if(!nodes.contains(p->name())){continue;}
			if(ports_.contains(p->name())){continue;}
			Port * pi = new Port(p->name());
			pi->setChild(p,inst);

			this->add(pi);
		      }
		}


	//	foreach(QString s,nodes){
	//		if(_ports.contains(s))
				//continue;

	//		Port* p = new Port(s);
	//		if(porthash.contains(s)){
	//		    p->addChild(porthash[s],instancehash[s]);
	//		    this->add(p);
	//		}
	//	}


	}

}
