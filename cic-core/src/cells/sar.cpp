//====================================================================
//        Copyright (c) 2016 Carsten Wulff Software, Norway
// ===================================================================
// Created       : wulff at 2016-8-2
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

#include "sar.h"

using namespace cIcSpice;
using namespace cIcCore;

namespace cIcCells{


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
        Instance* inst;

        if(groups.contains(group)){
            foreach(SubcktInstance* ckt, groups[group]){
                if(i % 2 == 0){
                    inst = this->addInstance(ckt,x,y);
                }else{
                    inst = this->addInstance(ckt,x,y);
                    inst->setAngle("MY");
                }
                i++;
                x += inst->width() + xoffset;
            }
        }
        return inst;
        
    }
    
    


    void SAR::place()
    {
        noPowerRoute();        

        SARgroup groups;
        
        //Collect groups
        foreach(cIcSpice::SubcktInstance * ckt_inst,_subckt->instances()){
            QString group = ckt_inst->groupName();
            groups[group].append(ckt_inst);
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
        
        int centerx = (cdacwidth*2 + ms*5)/2;    
        int i = 1;
        int x = centerx - switchwidth;
        int y = 0;
        
        //Place switches
        inst = this->placeAlternateMirror(groups,"XB",i,x,y,0);
        if(inst) y += inst->height() + msw*4;

        //Place CDAC
        x = 0; i = 1;
        inst = this->placeAlternateMirror(groups,"XDAC",i,x,y,ms);
        if(inst) y = inst->y2();

        //Get routing height
        QList<Graph*> graphs = this->getNodeGraphs("CP<|CN<|D<");
        int yc = y + msw*2;
        y += msw*graphs.count() + msw*10;
        
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

        this->addSarRouting(yc,msw,mw);
        
        
        
    }

    void SAR::addSarRouting(int y,int msw,int mw)
    {
        
        y += msw + mw;
        sarn = new Rect("M4",this->x1() + msw,y,this->width() - 2*msw,mw);
        this->add(sarn);
        y += 2*msw + mw;
        sarp = new Rect("M4",this->x1() + msw,y,this->width() - 2*msw,mw);                   
        this->add(sarp);
        y += msw + mw;
        
        if(ports_.contains("SARP")){
            ports_["SARP"]->set(sarp);
            ports_["SARN"]->set(sarn);
        }

        QList<Rect*> sarn_cdac = this->findRectanglesByNode("SARN","","CDAC");
        QList<Rect*> sarn_start;
        sarn_start.append(sarn);
        Route* r = new Route("SARN","M3",sarn_start,sarn_cdac,"","-|--");
        this->add(r);

        QList<Rect*> sarp_cdac = this->findRectanglesByNode("SARP","","CDAC");
        QList<Rect*> sarp_start;
        sarp_start.append(sarp);
        Route* r1 = new Route("SARP","M3",sarp_start,sarp_cdac,"","-|--");
        this->add(r1);

        
    }

    void SAR::route()
    {


        LayoutCell::route();
        
    }
    



    }
