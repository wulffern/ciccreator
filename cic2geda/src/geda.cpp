//====================================================================
//        Copyright (c) 2015 Carsten Wulff Software, Norway
// ===================================================================
// Created       : wulff at 2015-8-20
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

#include "geda.h"

namespace cIcPrinter{

    gEda::gEda(QString name):DesignPrinter(name + ".lib")
    {
        subcktInPrint = false;
        gwriter = new gEdaFileWriter();
        
    }

    gEda::~gEda()
    {
    }

    void gEda::openFile(QString aFilename)
    {
        DesignPrinter::openFile(aFilename);
        QTextStream ts(&file);

    }

    void gEda::closeFile()
    {


    }



    void gEda::startCell(Cell * cell){

        //- Make sure everything is there
        if(!cell){return;}
        if(cell->isCut()){return;};
        ckt = cell->subckt();
        if(!ckt){return;}


        //Make a new schematic
        sch = new GSchematic();
        
        //Make a new symbol
        GSymbol * sym = new GSymbol();
        
        if(ckt->name() != "")
        {
            int i =1;
            int x = 0;
            int y = 10;
            int length = 20;
            QString name = ckt->name();
            
            GText *  tname  = GText::commentText(length, -10, name);
            sym->addChild(tname);

            int slength = 0;
            
            foreach(QString  n, ckt->nodes())
            {

                GPin * p = GPin::getPin(n,i,x,y);
                sym->addChild(p);
                i += 1;
                y += 20;
                if(n.length()  > slength) slength = n.length();
            }

            GBox * b = GBox::commentBox(20, 0);
            b->width = slength*5;
            b->height = y;
            sym->addChild(b);
            QString s(ckt->name() + ".sym");
            
            gwriter->writeFile(s,sym);
            
        }
        this->subcktInPrint = true;

    }

    void gEda::endCell(){

        if(subcktInPrint)
        {

            subcktInPrint = false;
            QString s(ckt->name() + ".sch");
            gwriter->writeFile(s,sch);
            
        }


    }

    void gEda::printRect(Rect *rect){

    }

    void gEda::printPort(Port *){

    }

    void gEda::printReference(Cell *c){

        Instance * i = (Instance *) c;
        SubcktInstance *si = i->subcktInstance();
        Cell * cell =   i->cell();

        if(!cell){return;}

        


        if(si && sch){
            QString name = si->subcktName() + ".sym";
            QString instname = si->name();
            

            GComponent * gc = GComponent::newInstance(name,instname,xref,yref);
             sch->addChild(gc);
             
             
             
             if (xref > 800)
             {
                 xref = 0;
                 if(symbolHeight.contains(si->subcktName()))
                 {
                     yref -= symbolHeight[si->subcktName()]*1.5;

                 }else
                 {
                     yref -= 100;
                 }




             }

            
             if(symbolWidth.contains(si->subcktName()))
             {
                 xref += symbolWidth[si->subcktName()]*1.5;
             }else{
                      xref += 100;
             }


        }
    }



}
