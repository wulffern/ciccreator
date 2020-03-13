//====================================================================
//        Copyright (c) 2016 Carsten Wulff Software, Norway 
// ===================================================================
// Created       : wulff at 2019-06-02
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

#include "phcapcell.h"

using namespace cIcSpice;
using namespace cIcCore;

namespace cIcCells{

    void PhCapCell::binaryBits(QJsonValue obj){
        _binaryBits = obj.toInt();
    }

    void PhCapCell::unaryBits(QJsonValue obj){
        _unaryBits = obj.toInt();
    }

    Rect PhCapCell::calcBoundingRect()
    {
        Rect r = LayoutCell::calcBoundingRect();
        int mw =  this->rules->get("M2","width");
        r.adjust(0,+mw/2,0,-mw/2);
        return r;
    }

    void PhCapCell::place()
    {
        this->noPowerRoute();
        this->setBoundaryIgnoreRouting(false);
        
        LayoutCell::place();

        int ms =  this->rules->get("M4","capspace");
        Instance* ct = Cut::getInstance("M1","M4",2,1);
        
        int mw = ct->height();
        int mh = ct->width();

        _finger_length = pow(2,(_binaryBits + 2))*mw + mh*2;
        _top_length = _finger_length + 2*ms + 2*mw;

        int x = 0;
        int y = 0;

        int msw = mw + ms;

        QList<Rect*> rects;

        //- Add top plate
        auto first_top = new Rect("M5", x ,y , _top_length, mw);
        this->add(first_top);
        y += msw;

        //Add binary bits
        for(int i=0;i<=_binaryBits;i++){
            y = this->addFinger(x,y,msw,mw,mh,i,"CB" + to_string((long long) i));
        }

        for(int i=0;i<=_unaryBits;i++){
            for(int z=0;z<pow(2,_unaryBits);z++){
                y = this->addFinger(x,y,msw,mw,mh,_binaryBits,"CU" + to_string((long long)i) + "N" + to_string((long long)z));

            }
        }
        y -= ms;
        auto left_top = new Rect("M5",first_top->x1(),first_top->y1(),mw,y - first_top->y2());
        auto right_top = new Rect("M5",first_top->x2()-mw,first_top->y1(),mw,y - first_top->y2());  
        this->add(left_top);this->add(right_top);

        
        this->add(rects);
    }

    int PhCapCell::addFinger(int x, int y, int msw, int mw, int mh, int i, string name){

            Instance *ct1 = Cut::getInstance("M4","M5",2,1);
            ct1->moveTo(x + msw-mw,y);
            //- Plate 1
            auto p1 = new Rect("M5", ct1->x2() ,y , mw*(pow(2,(_binaryBits + 1)) + pow(2,i+1)-1 ) , mw);
            
            auto name_a = name + "_A";

            auto pa = new Port(QString::fromLocal8Bit(name_a.c_str()));
            pa->moveTo(ct1->x1(),ct1->y1());
            //- Plate 2
            auto p2 = new Rect("M5", p1->x2() +  mw*2  ,y , (pow(2,(_binaryBits + 1)) - pow(2,i+1)+1 )*mw  , mw);
            auto ct2 = Cut::getInstance("M4","M5",2,1);
            ct2->moveTo(p2->x2(),y);  
            auto name_b = name + "_B";  
            auto pb = new Port(QString::fromLocal8Bit(name_b.c_str()));
            pb->moveTo(ct2->x1(),ct2->y1());

            
            y += msw;   
            this->add(ct1);
            this->add(ct2);
            this->add(p1);
            this->add(p2);
            this->add(pa);this->add(pb);
            this->add(new Rect("M5", x ,y , _top_length, mw));
            y += msw;
            return y;
    }

};
