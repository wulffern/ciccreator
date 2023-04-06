//====================================================================
//        Copyright (c) 2017 Carsten Wulff Software, Norway 
// ===================================================================
// Created       : wulff at 2017-1-8
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

#include "guard.h"

namespace cIcCore{


    Guard::Guard(Rect* r,QList<QString> layers)
    {
        //this->setBoundaryIgnoreRouting(false);

        
        auto d1 = new Cut("OD","M1",2,1);
        if(!d1) return;
        
        auto od = d1->getRect("OD");

        if(!od){
            qDebug() << "Could not find OD rectangle in Cut";
            qDebug() << d1->toString();
            return;
            
        }
        
        
        int xod = od->x1() - d1->x1();
        int yod = od->y1() - d1->y1();
        int odw = od->height();
        
        d1->addEnclosingLayers(layers);

        r->adjust(d1->height());


        //this->add(d1);

        
        QList<Cell*> rects;

        auto * c_bot = new Cell();
        c_bot->add(new Rect("OD",r->x1() + xod, r->y1() + yod, r->width() - xod*2,odw));
        c_bot->add(new Rect("M1",r->x1() + xod, r->y1() + yod, r->width() - xod*2,odw));
        auto * c_left = new Cell();
        c_left->add(new Rect("OD",r->x1() + xod, r->y1() + yod,odw,r->height() - yod*2));
        c_left->add(new Rect("M1",r->x1() + xod, r->y1() + yod,odw,r->height() - yod*2));
        auto * c_top = new Cell();
        c_top->add(new Rect("OD",r->x1() + xod, r->y1() + r->height() - yod - odw, r->width() - xod*2,odw));
        c_top->add(new Rect("M1",r->x1() + xod, r->y1() + r->height() - yod - odw, r->width() - xod*2,odw));
        auto * c_right = new Cell();
        c_right->add(new Rect("OD",r->x1() + r->width()  - xod - odw, r->y1() + yod, odw, r->height() - yod*2));
        c_right->add(new Rect("M1",r->x1() + r->width()  - xod - odw, r->y1() + yod, odw, r->height() - yod*2));

        rects.append(c_bot);
        rects.append(c_top);
        rects.append(c_left);
        rects.append(c_right);

        Rect * b = new Rect("M1",r->x1() + xod, r->y1() + yod, r->width() - xod*2,odw);
        this->add(b);
            
        foreach( Cell* c2, rects){

            //TODO  Need to write a fill-cut function to add cuts

            //Adjust size
            auto* rcp = c2->getCopy();
            if(rcp->isVertical()){
                rcp->adjust(0,odw,0,-odw);
            }else{
                rcp->adjust(odw,0,-odw,0);
            }

            Cut* c= new Cut("OD","M1",rcp);
            this->add(c);

            c2->addEnclosingLayers(layers);
            this->add(c2);

        }
        
        
        
    }
    
    Guard::~Guard()
    {

    }
    



}
