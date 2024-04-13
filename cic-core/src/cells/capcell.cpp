//====================================================================
//        Copyright (c) 2016 Carsten Wulff Software, Norway
// ===================================================================
// Created       : wulff at 2016-9-3
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




#include "capcell.h"

using namespace cIcSpice;
using namespace cIcCore;

namespace cIcCells{


    void CapCell::usem3(QJsonValue obj){
        if(obj.toInt()){
            usem3_ = true;
        }else{
            usem3_ = false;
        }

    }

    void CapCell::usem5(QJsonValue obj){
        if(obj.toInt()){
            usem5_ = true;
        }else{
            usem5_ = false;
        }

    }

    void CapCell::heightIncreaseMult(QJsonValue obj){
        heightIncreaseMult_ = obj.toInt();

    }


    Rect CapCell::calcBoundingRect()
    {
        Rect r = LayoutCell::calcBoundingRect();
        int mw =  this->rules->get("M2","width");
        r.adjust(0,+mw/2,0,-mw/2);
        return r;
    }

    Rect* CapCell::getAvssConnectRect(Rect* rect)
    {
        //- Return a "Cell" that can connect a port to AVSS
        Cell * cell = new Cell();
        Cut * c = new Cut("M1","M2",1,2);
        int y = c->height()/2.0;
        c->moveTo(0,-y);
        Rect * r = new Rect("M2",0,-rect->height()/2.0,ports_["AVSS"]->x1(),c->width());
        cell->add(c);
        cell->add(r);
        cell->updateBoundingRect();
        cell->moveTo(0,rect->centerY() - y);
        return cell;

    }


    void CapCell::place()
    {
        this->noPowerRoute();
        this->setBoundaryIgnoreRouting(false);


        LayoutCell::place();

        int ms =  this->rules->get("M4","capspace");
        Instance* ct = Cut::getInstance("M1","M4",2,1);
        int mw = ct->height();

        int y = 0;
        int count = 32;
        msw = ms + mw;
        int mswy = ms + mw + ct->width()/2 - mw/2;

        int msc = ct->width()/2 + ms*2*heightIncreaseMult_;
        int b = 5;

        int height = mswy*2 + msc*b + mw;
        int xm1 = 0;
        xorg = ct->width() + ms*2;
        int x= xorg;

        int first = 1;

        //- First dummy
        QList<Rect*> rects;
        if(usem5_){
            rects.append(new Rect("M5",x-  msw ,y , mw, height));
        }
        for (auto i =0; i<count;i +=1) {
            auto r = new Rect("M4",x,y, mw, height);
            rects.append(r);
            if (first) {
                Port* p = new Port("CTOP"); p->set(r);this->add(p);
                first = 0;
            }
            rects.append(new Rect("M2",x,y, mw, height - mw));
            rects.append(new Rect("M4",x + msw ,y + msw, mw, height - msw*2));
            if(usem3_){
                rects.append(new Rect("M3",x + msw ,y + msw, mw, height - msw*2));
            }
            if(usem5_){
            rects.append(new Rect("M5",x + msw ,y , mw, height));
            }
            x = x + (msw)*2;
        }


        auto xdm = x;
        //- Add dumauto
        Instance * ctdumauto = nullptr;
        if(usem5_){
            ctdumauto  = Cut::getInstance("M2","M5",1,2);
        }else{
            ctdumauto  = Cut::getInstance("M2","M4",1,2);
        }
        ctdumauto->moveTo(x + msw,y + height/2 - ctdumauto->height());
        this->add(ctdumauto);

        for (auto i =0; i<count;i +=1) {
            rects.append(new Rect("M2",x + msw ,y , mw, height));
            rects.append(new Rect("M3",x + msw ,y , mw, height));
            rects.append(new Rect("M4",x + msw ,y , mw, height));
            if(usem5_){
                rects.append(new Rect("M5",x + msw ,y, mw, height));
            }
            xdm = xdm + (msw)*2;
        }

        auto ctopport = new Rect("M4", x,y, mw, height);
        rects.append(ctopport);
        rects.append(new Rect("M2", x,y, mw, height));

        auto y1a = mswy;
        auto y1b = y1a + msc*b;
        auto y2 = y1a + msc*(b - 3);
        auto y4 = y1a + msc*(b - 1);
        auto y8 = y1a + msc*(b - 2);
        auto y16 = y1a + msc*(b - 4);
        yMax = height;

        if(usem5_){
            rects.append(new Rect("M5",xorg - msw, y ,x - xorg + msw*2, mw));
            rects.append(new Rect("M5",xorg - msw, height - mw ,x - xorg +  msw*2 , mw));
        }


        auto vss1 =  new Rect("M2",xorg, y ,x - xorg +msw, mw);
        auto ctop1 =  new Rect("M4",xorg, y ,x - xorg, mw);
        auto ctop2 =  new Rect("M4",xorg, height - mw ,x - xorg, mw);
        auto vss2 =  new Rect("M2",xorg, height -mw ,x-xorg + msw , mw);
        Port* p =new Port("AVSS"); p->set(vss1);this->add(p);

        auto c1a =  new Rect("M1",xm1 , y1a ,x - xm1, mw);
        auto c1ap =  new Rect("M1",xm1 , y1a ,ct->width(), mw);
        p = new Port("C1A"); p->set(c1ap);this->add(p);

        auto c1b =  new Rect("M1",xm1 , y1b,x - xm1, mw);
        auto c1bp =  new Rect("M1",xm1, y1b ,ct->width(), mw);
        p = new Port("C1B"); p->set(c1bp);this->add(p);

        auto c2 =  new Rect("M1",xm1, y2,x - xm1, mw);
        auto c2p =  new Rect("M1",xm1 , y2 ,ct->width(), mw);
        p = new Port("C2"); p->set(c2p);this->add(p);

        auto c4 =  new Rect("M1",xm1 , y4,x - xm1, mw);
        auto c4p =  new Rect("M1",xm1, y4 ,ct->width(), mw);
        p = new Port("C4"); p->set(c4p);this->add(p);

        auto c8 =  new Rect("M1",xm1, y8,x - xm1, mw);
        auto c8p =  new Rect("M1",xm1, y8 ,ct->width(), mw);
        p = new Port("C8"); p->set(c8p);this->add(p);

        auto c16 =  new Rect("M1",xm1, y16,  x - xm1, mw);
        auto c16p =  new Rect("M1",xm1, y16 ,ct->width(), mw);
        p = new Port("C16"); p->set(c16p);this->add(p);

        rects.append(c16);
        rects.append(c1a);
        rects.append(c1b);
        rects.append(c2);
        rects.append(c4);
        rects.append(c8);
        rects.append(vss1);
        rects.append(vss2);
        rects.append(ctop1);
        rects.append(ctop2);

        //- 1  3  5  7  9  11 13 15 17 19 21 23 25 27 29 31 33 35 37 39 41 43 45 47 49 51 53 55 57 59 61 63
        //-                      1A                                                 1B
        //-                         2                                             2
        //-                   4        4                                      4         4
        //-             8  8              8  8                          8  8              8  8
        //  16 16 16 16                         16 16 16 16 16 16 16 16                         16 16 16 16

        QList<int> cint1a = QList<int>() << 15;
        QList<int> cint1b = QList<int>() << 49;
        QList<int> cint2 = QList<int>() << 17 << 47;
        QList<int> cint4 = QList<int>() <<13 << 19 <<45 <<51;
        QList<int> cint8 = QList<int>() <<9 <<11 <<21 <<23 <<41<<43<<53<<55;
        QList<int> cint16 = QList<int>() <<1<<3<<5<<7<<25<<27<<29<<31<<33<<35<<37<<39<<57<<59<<61<<63;



        this->addContacts("XRES1A","C1A",y1a ,cint1a,c1a);
        this->addContacts("XRES1B","C1B",y1b ,cint1b,c1b);
        this->addContacts("XRES2","C2",y2,cint2,c2);
        this->addContacts("XRES4","C4",y4,cint4,c4);
        this->addContacts("XRES8","C8",y8,cint8,c8);
        this->addContacts("XRES16","C16",y16,cint16,c16);
        this->add(rects);
    }

    void CapCell::addContacts(QString name, QString node,int y,QList<int> array, Rect * cp){

        auto ms =  this->getRules()->get("M2","space");

        auto ctres =  Cut::getInstance("M1","M4",2,1);
        auto inst = this->getInstanceFromInstanceName(name);
        if(!inst){return;}
        inst->moveTo(ctres->width(),y);

        //Move the metal1 rectangle so it starts at after the metal resistor. Not all
        //technologies can overlap res and metal
        cp->setLeft(inst->x2());


        foreach(int x,array) {

            auto ct = Cut::getInstance("M1","M4",1,2);
            auto mw = ct->width();
            ct->moveTo(xorg + msw*x ,y+mw/2- ct->height()/2);
            auto ra = new Rect("M2",ct->x1(),0,ct->width(),ct->y1()- ms);
            auto rb = new Rect("M2",ct->x1(),ct->y2() + ms,ct->width(),yMax - ct->y2() - ms );
            this->add(ct);
            if(ra->height() - mw > mw){
                this->add(ra);
            }
            if(rb->height() - mw > mw){
                this->add(rb);
            }


        }
    }

};
