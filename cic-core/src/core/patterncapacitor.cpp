//====================================================================
//        Copyright (c) 2015 Carsten Wulff Software, Norway
// ===================================================================
// Created       : wulff at 2015-04-03
// ===================================================================
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.

//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//====================================================================

#include "core/patterncapacitor.h"

namespace cIcCore{

    PatternCapacitor::PatternCapacitor()
    {
        //res1 = new Resistor(QStringList() << "A" << "NC1");
        //res2 = new Resistor(QStringList() << "B" << "NC2");
        //res1->setName("R1");
        //res2->setName("R2");

        Subckt * ckt = new Subckt();

        //QStringList n;
//        n << "A" << "B" ;
        //      ckt->setNodes(n);
        //ckt->add(res1);
        //ckt->add(res2);
        rindex = 0;
        rcounter = 0;

        this->setSubckt(ckt);

    }

    void PatternCapacitor::onFillCoordinate(QChar c, QString layer, int x, int y, QMap<QString,QVariant> &data){

        switch(c.unicode()){
            //Create ports
        case 'B':
        case 'A':
        case 'P':
        case 'N':
        case 'D':
        case 'S':
        case 'G':
            Resistor *res = new Resistor(QStringList() << c << QString("NC%1").arg(rindex));
            rindex++;
            res->setName(QString("%1%2").arg("R").arg(rindex));
            nodes << c;
            this->subckt()->setNodes(nodes);
            this->subckt()->add(res);
            resistors.append(res);
            res->setProperty("layer",layer);
            break;

        }
    }





    PatternCapacitor::~PatternCapacitor()
    {

    }


    void PatternCapacitor::onPaintEnd()
    {
        Subckt * ckt = this->subckt();

        ckt->setProperty("width",this->rules->toMicron(this->width()));
        ckt->setProperty("height",this->rules->toMicron(this->height()));
        
    }
    


    void PatternCapacitor::paintRect(Rect* r, QChar c  ,int x , int y ){

        if(c != 'r' || r == 0) return;

        Layer* l = this->rules->getLayer(r->layer());
        QString reslayer = l->res;
        if(reslayer == "") return; //Return if resistor layer is
                                   //undefined

        Rect * rc = new Rect(reslayer,translateX(x),translateY(y),xspace_,currentHeight_);
        
        if(c =='r'){
            if(resistors.count() > rcounter){

                resistors[rcounter]->setProperty("width",this->rules->toMicron( currentHeight_ ) );
                resistors[rcounter]->setProperty("length",this->rules->toMicron( xspace_ ) );
                resistors[rcounter]->setProperty("layer",r->layer());
                
                rcounter +=1;
            }
        }




        this->add(rc);
    }

     QJsonObject PatternCapacitor::toJson(){
        QJsonObject o = PatternTile::toJson();
        return o;
    }

    void PatternCapacitor::fromJson(QJsonObject o){
        PatternTile::fromJson(o);
    }


}
