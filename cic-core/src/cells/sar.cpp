//====================================================================
//        Copyright (c) 2016 Carsten Wulff Software, Norway
// ===================================================================
// Created       : wulff at 2016-8-2
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




#include "sar.h"

using namespace cIcSpice;
using namespace cIcCore;

namespace cIcCells{


    void SAR::usem5(QJsonValue obj){
        if(obj.toInt()){
            usem5_ = true;
        }else{
            usem5_ = false;
        }

    }

    int SAR::getCellWidth(SARgroup groups, QString group)
    {
        int width = 0;

        if(groups.contains(group))
        {
            SubcktInstance* ckt = groups[group][0];
            if(ckt && Cell::_allcells.contains(ckt->subcktName())){
                Cell* c = Cell::_allcells[ckt->subcktName()];
                if(c) width = c->width();
            }
        }

        return width;
    }

    Instance* SAR::placeAlternateMirror(SARgroup groups,QString group, int i, int x ,int y, int xoffset)
    {
        Instance* inst = NULL;

        int ind = i;
        int xc = x;

        if(groups.contains(group)){
            foreach(SubcktInstance* ckt, groups[group]){
                if(ind % 2 == 0){
                    inst = this->addInstance(ckt,xc,y);
                }else{
                    inst = this->addInstance(ckt,xc,y);
                    inst->setAngle("MY");
                }
                ind++;
                xc += inst->width() + xoffset;
            }
        }
        return inst;

    }




    void SAR::place()
    {
        noPowerRoute();
        this->setBoundaryIgnoreRouting(false);


        
        SARgroup groups;

        //Collect groups
        if(_subckt){
            
        foreach(cIcSpice::SubcktInstance * ckt_inst,_subckt->instances()){
            if(ckt_inst){                
                QString group = ckt_inst->groupName();            
                groups[group].append(ckt_inst);
            }
            
        }
        }
        

        //Metal space
        int ms = this->rules->get("M3","space");
        int mw = this->rules->get("M3","width");
        Instance* cut = Cut::getInstance("M1","M2",2,1);
        int msw = ms + mw + (cut->width()-mw)/2;

        Instance* inst;

        //Get CDAC width
        int cdacwidth = this->getCellWidth(groups,"XDAC");
        int switchwidth = this->getCellWidth(groups,"XB");

        int centerx = (cdacwidth*2 + ms*5 + mw*4)/2;
        int i = 1;
        int x = centerx - switchwidth;
        int y = 0;

        //Place switches
        inst = this->placeAlternateMirror(groups,"XB",i,x,y,0);
        if(inst) y += inst->height() + msw;

        //Place CDAC
        x = 0; i = 1;
        inst = this->placeAlternateMirror(groups,"XDAC",i,x,y,ms*5 + mw*2);
        if(inst) y = inst->y2();

        //Get routing height
        QList<Graph*> graphs = this->getNodeGraphs("CN<|D<|CP<");

        

        int yc = y + msw*2;

        y += msw*graphs.count() + msw*10- mw;

        //Get control width
        int ctrwidth = this->getCellWidth(groups,"XA");
        int all_ctrwidth = ctrwidth*groups["XA"].count();
        x = centerx - all_ctrwidth/2;

        //Place control blocks
        i = 0;
        inst = this->placeAlternateMirror(groups,"XA",i,x,y,0);

        int maxy = y;

        if(inst) maxy = inst->y2();

        y = maxy + msw*4;

        this->updateBoundingRect();

        yc = this->addSarRouting(yc,msw,mw);



        //Scramble routing
        std::sort(graphs.begin(), graphs.end(), sortGraph);

        Instance* c;
        foreach(Graph* graph, graphs){

            auto r0 = new Rect("M3",this->x1(),yc,this->width(),mw);
            yc += msw;
            double xfmin = 1e32;
            double xfmax = -1e32;

            QList<Rect*> rects = graph->getRectangles("","SARDIG","");
            foreach(Rect* r,rects){
                auto r1 = new Rect("M4",r->x1(),r0->y2(),mw,r->y1()  - r0->y2());
                c = Cut::getInstance("M3","M4",1,2);
                c->moveCenter(r1->centerX(),r0->centerY());
                this->add(c);
                this->add(r1);
                this->addPort(graph->name,r1);
                xfmin =  (xfmin > r1->x1()) ? r1->x1(): xfmin;
                xfmax=  (xfmax < r1->x2()) ? r1->x2(): xfmax;
            }

            QList<Rect*> rects_cdac = graph->getRectangles("","CDAC","");
            foreach(Rect* r,rects_cdac){
                c = Cut::getInstance("M2","M3",1,2);
                c->moveCenter(r->centerX(),r0->centerY());
                auto r1 = new Rect("M2",r->x1(),r->y2(),mw,r0->y1()  - r->y2());
                this->add(c);
                this->add(r1);
                xfmin =  (xfmin > r1->x1()) ? r1->x1() : xfmin;
                xfmax=  (xfmax < r1->x2()) ? r1->x2() : xfmax;
            }

            r0->setLeft(xfmin);
            r0->setRight(xfmax);
            this->add(r0);
        }

        this->updateBoundingRect();

    }

    bool SAR::sortGraph(Graph * a, Graph* b)
    {

        QList<Rect*> rect_a = a->getRectangles("","SARDIG","");
        QList<Rect*> rect_b = b->getRectangles("","SARDIG","");
//Return if they are the same
        if(rect_a.count() == 0 || rect_b.count() == 0) return false;

        QList<Rect*> rect_sorted_a = Rect::sortLeftOnTop(rect_a);
        QList<Rect*> rect_sorted_b = Rect::sortLeftOnTop(rect_b);
        Rect* ra = rect_sorted_a.first();
        Rect* rb = rect_sorted_b.first();
        int xa = ra->y2();
        int xb = rb->y2();

        return xa < xb;

    }



    int SAR::addSarRouting(int y,int msw,int mw)
    {


        int ms = this->rules->get("M3","space");

        y += msw - mw;
        sarn = new Rect("M4",this->x1() + msw,y,this->width() - 2*msw,mw);
        this->add(sarn);
        y += 2*msw ;
        sarp = new Rect("M4",this->x1() + msw,y,this->width() - 2*msw,mw);
        this->add(sarp);
        y += msw;


        if(ports_.contains("SARP")){
            ports_["SARP"]->set(sarp);
            ports_["SARN"]->set(sarn);
        }


        
        
        //Add CDAC SARN routing
        QList<Rect*> sarn_cdac = this->findRectanglesByNode("SARN","","CDAC");
        if(sarn_cdac.count() > 0){
            Rect* r_sarn = sarn_cdac[0];
            auto ra_sarn = new Rect("M3",r_sarn->x1(),r_sarn->y2(),mw,sarn->y1()-r_sarn->y2());
            auto ct_sarn = Cut::getInstance("M3","M4",1,2);
            ct_sarn->moveTo(ra_sarn->x1(),ra_sarn->y1());
            auto ct1_sarn = Cut::getInstance("M3","M4",2,1);
            ct1_sarn->moveTo(r_sarn->x1(),sarn->y1());
            ra_sarn->setTop(ct1_sarn->y2());
            this->add(ra_sarn);
            this->add(ct_sarn);
            this->add(ct1_sarn);
        }


        //Add CDAC SARP routing
        QList<Rect*> sarp_cdac = this->findRectanglesByNode("SARP","","CDAC");
        if(sarp_cdac.count() > 0){
            Rect* r_sarp = sarp_cdac[0];
            auto ra_sarp = new Rect("M3",r_sarp->x1(),r_sarp->y2(),mw,sarp->y1()-r_sarp->y2());
            auto ct_sarp = Cut::getInstance("M3","M4",1,2);
            ct_sarp->moveTo(ra_sarp->x1(),ra_sarp->y1());
            auto ct1_sarp = Cut::getInstance("M3","M4",2,1);
            ct1_sarp->moveTo(r_sarp->x1(),sarp->y1());
            ra_sarp->setTop(ct1_sarp->y2());
            int ctx1 = ct_sarp->x1();
            this->add(ra_sarp);
            this->add(ct_sarp);
            this->add(ct1_sarp);
            sarp->setLeft(ctx1);
            sarn->setLeft(ctx1);
        }


        //Add CMP SARP routing
        QList<Rect*> sarp_cmp = this->findRectanglesByNode("SARP","","SARCMP");

        if(sarp_cmp.count() > 0){

            Rect* r = sarp_cmp[0];
            if(r){


                auto ct_cmp =Cut::getInstance("M2","M4",2,1);
                Instance * ct2 = nullptr;
                Instance * ct1 = nullptr;
                if(usem5_){
                    ct2 = Cut::getInstance("M4","M5",2,1);
                    ct1 =Cut::getInstance("M4","M5",1,2);
                }else{
                    ct2 = Cut::getInstance("M4","M3",2,1);
                    ct1 =Cut::getInstance("M4","M3",1,2);
                }

                int ycc = sarp->y2() +ct2->height()*4;
                auto ra = new Rect("M4",r->x1(),ycc,mw,r->y1()-ycc);
                ct_cmp->moveTo(r->x1(),r->y1());

                Rect * rb_cmp = nullptr;
                if(usem5_){
                    rb_cmp = new Rect("M5",ra->x1(),sarp->y1(),mw,ycc-sarp->y1());
                }else{
                    rb_cmp = new Rect("M3",ra->x1(),sarp->y1(),mw,ycc-sarp->y1());
                }
                ct2->moveTo(ra->x1(),sarp->y1());
                ct1->moveTo(ra->x1(),ra->y1());
                QList<Rect*> rects = QList<Rect*>() << ct_cmp << rb_cmp << ra << ct2 << ct1<< rb_cmp;
                this->add(rects);
            }
        }


        //Add CMP SARP routing
        QList<Rect*> sarn_cmp = this->findRectanglesByNode("SARN","","SARCMP");
        if(sarn_cmp.count() > 0){

            Rect* r = sarn_cmp[0];
            if(r){

                auto ct_cmp =Cut::getInstance("M2","M3",2,1);
                auto cta_cmp =Cut::getInstance("M3","M4",2,1);

                Instance * ct2 = nullptr;
                Instance * ct1 = nullptr;
                if(usem5_){
                    ct2 = Cut::getInstance("M4","M5",2,1);
                    ct1 =Cut::getInstance("M4","M5",1,2);
                }else{
                    ct2 = Cut::getInstance("M4","M3",2,1);
                    ct1 =Cut::getInstance("M4","M3",1,2);
                }


                int ycc = sarp->y2() +ct2->height()*4;
                auto ra = new Rect("M4",r->x2()+ms,ycc,mw,r->y1()-ycc);
                cta_cmp->moveTo(r->x2() + ms + mw - ct_cmp->width(),r->y1());
                ct_cmp->moveTo(r->x1(),r->y1());
                auto rmet = new Rect("M3",ct_cmp->x1(),ct_cmp->y1(),cta_cmp->x2() - ct_cmp->x1(),ct_cmp->height());
                Rect * rb_cmp = nullptr;
                if(usem5_){
                     rb_cmp = new Rect("M5",ra->x1(),sarn->y1(),mw,ycc-sarn->y1());
                }else{
                     rb_cmp = new Rect("M3",ra->x1(),sarn->y1(),mw,ycc-sarn->y1());
                }
                ct2->moveTo(ra->x1(),sarn->y1());
                ct1->moveTo(ra->x1(),ra->y1());
                QList<Rect*> rects = QList<Rect*>() << cta_cmp <<  ct_cmp << rb_cmp << ra << ct2 << ct1<< rb_cmp;
                this->add(rects);
            }
        }

        y += msw ;
        return y;

    }

    void SAR::route()
    {
        LayoutCell::route();

    }




}
