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

#include "cells/cdac.h"

using namespace cIcCore;
using namespace cIcSpice;


namespace cIcCells{
    
    void CDAC::place()
    {
        this->noPowerRoute();
        this->setBoundaryIgnoreRouting(false);     

        int xs = this->rules->get("M1","space");
        int xw = this->rules->get("M1","width");
        int xg = this->rules->get("ROUTE","horizontalgrid");

                 //Find how many nets called CP
        QStringList nodes= subckt()->nodes();
        int i=-1;
        foreach(QString s,nodes){
            if(s.contains("CP")){
                i += 1;
            }
        }

        
        int prev_width = 0;
        int x =  (xw + xs*2)*(i+2);
        int y = 0;
        bool first = true;
        
        foreach(cIcSpice::SubcktInstance * ckt_inst,_subckt->instances()){
            inst= this->addInstance(ckt_inst,x,y);
            y += inst->height();
            if(first){
                firstinst = inst;
                first = false;
            }
        }


        this->adjust(-xs*2+xw,0,0,0);
        

        //Add the route rings
        for(auto k=i;k>=0;k--){
            QString name = QString("CP<%1>").arg(k);
            this->addRouteRing("M2",name,"l",1,2,false);
            
        }

        //- Add AVSS connections
        QList<Graph*> graphs = this->getNodeGraphs("AVSS");
        foreach(Graph* graph,graphs){
            foreach(Port* p, graph->ports){
                if(!p->isInstancePort()) continue;
                InstancePort* ip = static_cast<InstancePort*>(p);
                if(!ip->childName().contains("C")) continue;
                Rect * r = p->parent();
                if(!r->isInstance()) continue;
                Instance* inst = static_cast<Instance *>(r);
                Cell* c = static_cast<Cell*>(inst->cell());
                if(c && strcmp(c->metaObject()->className(),"cIcCells::CapCell") == 0){
                    CapCell* cap = static_cast<CapCell*>(c);

                    //- Not really a rect, it's a cut
                    Rect* rect = cap->getAvssConnectRect(p);
                    rect->translate(inst->x1(),0);
                    this->add(rect);
                }
                        
            }
        }
        
        
        this->updateBoundingRect();
    }


    void CDAC::route()
    {
        this->addRouteConnection("CP","","M3","left","");

        if(firstinst){
            //Add ports
            QList<Rect*> rects1 = firstinst->findRectanglesByNode("AVSS","");
            if(rects1.count() > 0 ){
                
                this->addPort("AVSS",rects1[0]);
            }
        }
        
        
        if(inst){
            QList<Rect*> rects2 = inst->findRectanglesByNode("CTOP","");
            if(rects2.count() > 0){
                this->addPort("CTOP",rects2[0]);
                
            }
        }        
        
        LayoutCell::route();
        this->trimRouteRing("CP<","left","b");
        this->updateBoundingRect();
    }
    
    void CDAC::paint()
    {
        LayoutCell::paint();
  
        
//        moveTo(width,0);
    }
    
    

}

