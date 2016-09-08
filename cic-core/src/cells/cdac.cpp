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

        int prev_width = 0;
        int x = 0;
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

         //Find how many nets called CP
        QStringList nodes= subckt()->nodes();
        int i=0;
        foreach(QString s,nodes){
            if(s.contains("CP")){
                i += 1;
            }
        }

        //Add the route rings

        for(auto x=0;x<i;x++){
            QString name = QString("CP<%1>").arg(x);
            this->addRouteRing("M2",name,"l",1,1);
        }
        
        this->updateBoundingRect();
 

        

    }


    void CDAC::route()
    {

       
        

        this->addRouteConnection("CP","","M1","left","");

        if(firstinst){
            //Add ports
            QList<Rect*> rects1 = firstinst->findRectanglesByNode("AVSS","");
            if(rects1.count() > 1 ){
                this->addPort("AVSS",rects1[0]);
            }
        }
        
        if(inst){
            QList<Rect*> rects2 = inst->findRectanglesByNode("CTOP","");
            if(rects2.count() > 1){
                this->addPort("CTOP",rects2[0]);
                
            }
        }
        
        
        LayoutCell::route();
        this->trimRouteRing("CP<","left","b");
        this->updateBoundingRect();
    }
    
    
    

}

