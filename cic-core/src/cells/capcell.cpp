//====================================================================
//        Copyright (c) 2016 Carsten Wulff Software, Norway
// ===================================================================
// Created       : wulff at 2016-9-3
// ===================================================================
// The MIT License (MIT)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
//====================================================================

#include "capcell.h"

using namespace cIcSpice;
using namespace cIcCore;

namespace cIcCells{


    void CapCell::place()
    {
        this->noPowerRoute();

        LayoutCell::place();

        int ms =  this->rules->get("M4","capspace");
        Instance* ct = Cut::getInstance("M1","M4",2,1);
        int mw = ct->height();

        int y = 0;
        int count = 32;
        msw = ms + mw;
        int mswy = ms + mw + ct->width()/2 - mw/2;

        int msc = ct->width()/2 + ms*2;
        int b = 5;

        int height = mswy*2 + msc*b + mw;
        int xm1 = 0;
        xorg = ct->width() + ms*2;
        int x= xorg;

        int first = 1;

        //- First dummy
        QList<Rect*> rects;
        rects.append(new Rect("M5",x-  msw ,y , mw, height));
        for (auto i =0; i<count;i +=1) {
            auto r = new Rect("M4",x,y, mw, height);
            rects.append(r);
            if (first) {
                Port* p = new Port("CTOP"); p->set(r);this->add(p);
                first = 0;
            }
            rects.append(new Rect("M2",x,y, mw, height - mw));
            rects.append(new Rect("M4",x + msw ,y + msw, mw, height - msw*2));
            rects.append(new Rect("M3",x + msw ,y + msw, mw, height - msw*2));
            rects.append(new Rect("M5",x + msw ,y , mw, height));
            x = x + (msw)*2;
        }


        auto xdm = x;
        //- Add dumauto
        auto ctdumauto  = Cut::getInstance("M2","M5",1,2);
        ctdumauto->moveTo(x + msw,y + height/2 - ctdumauto->height());
        this->add(ctdumauto);

        for (auto i =0; i<count;i +=1) {
            rects.append(new Rect("M2",x + msw ,y , mw, height));
            rects.append(new Rect("M3",x + msw ,y , mw, height));
            rects.append(new Rect("M4",x + msw ,y , mw, height));
            rects.append(new Rect("M5",x + msw ,y, mw, height));
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

        rects.append(new Rect("M5",xorg - msw, y ,x - msw, mw));
        rects.append(new Rect("M5",xorg - msw, height - mw ,x - msw, mw));

        auto vss1 =  new Rect("M2",xorg, y ,x - msw*2, mw);
        auto ctop1 =  new Rect("M4",xorg, y ,x - xorg, mw);
        auto ctop2 =  new Rect("M4",xorg, height - mw ,x - xorg, mw);
        auto vss2 =  new Rect("M2",xorg, height -mw ,x - msw*2, mw);
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

        this->addContacts("XRES1A","C1A",y1a ,QList<int>({15}));
        this->addContacts("XRES1B","C1B",y1b ,QList<int>({49}));
        this->addContacts("XRES2","C2",y2,QList<int>({17,47}));
        this->addContacts("XRES4","C4",y4,QList<int>({13,19,45,51}));
        this->addContacts("XRES8","C8",y8,QList<int>({9,11,21,23,41,43,53,55}));
        this->addContacts("XRES16","C16",y16,QList<int>({1,3,5,7,25,27,29,31,33,35,37,39,57,59,61,63}));
        this->add(rects);
    }

    void CapCell::addContacts(QString name, QString node,int y,QList<int> array){

        auto ms =  this->getRules()->get("M2","space");

        auto ctres =  Cut::getInstance("M1","M4",2,1);
        auto inst = this->getInstanceFromInstanceName(name);
        if(!inst){return;}
        inst->moveTo(ctres->width(),y);
        foreach(int x,array) {

            auto ct = Cut::getInstance("M1","M4",1,2);
            auto mw = ct->width();
            ct->moveTo(xorg + msw*x ,y+mw/2- ct->height()/2);
            auto ra = new Rect("M2",ct->x1(),0,ct->width(),ct->y1()- ms);
            
            auto rb = new Rect("M2",ct->x1(),ct->y2() + ms,ct->width(),yMax - ct->y2() - ms );
            this->add(QList<Rect*>({ct,ra,rb}));

        }
    }

};
