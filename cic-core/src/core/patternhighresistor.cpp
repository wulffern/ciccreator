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

#include "core/patternhighresistor.h"

namespace cIcCore{

    PatternHighResistor::PatternHighResistor()
    {
        transposed_ = false;
        res = new Resistor();
        res->setDeviceName("rppo");
        res->setNodes(QStringList() << "N" << "P" << "B") ;
        Subckt * ckt = new Subckt();
        ckt->setNodes(res->nodes());
        ckt->add(res);
        this->setSubckt(ckt);


    }

    PatternHighResistor::~PatternHighResistor()
    {

    }

    void PatternHighResistor::onFillCoordinate(QChar, QString layer, int x, int y, QMap<QString,QVariant> &data){

        if(layer.startsWith("PO")){

            if(transposed_){
                //- Stripes run across the cell, so a finger is a ROW.
                //- The fill loop walks rows outer and columns inner,
                //- and y DECREASES as it goes (patterntile.cpp: y =
                //- rstrs.count() - i - 1), so the "highest so far"
                //- test the vertical path uses would count exactly
                //- one. Count a finger whenever the row changes
                //- instead, which does not care about direction.
                if(!data.contains("porow") || data["porow"].toInt() != y){
                    data["nf"] = data["nf"].toInt() + 1;
                    data["porow"] = y;
                }
                //- the stripe width is the row pitch, not the column
                res->setProperty("width",this->rules->toMicron(yspace_));
            }else{
                int pofinger = data["pofinger"].toInt();
                if(pofinger < x){
                    data["nf"] = data["nf"].toInt() +  1;
                    data["pofinger"] = x;
                }

                res->setProperty("width",this->rules->toMicron(xspace_));
            }
        }
    }


    void PatternHighResistor::onPaintEnclosure(Rect* r)
    {
        if(r->layer() == "OP"){
            //- the OP rectangle spans the resistor; its long side is
            //- the length, and which side that is depends on the
            //- orientation
            res->setProperty("length",this->rules->toMicron(
                                 transposed_ ? r->width() : r->height() ) );
        }

    }


    void PatternHighResistor::paintRect(Rect* r, QChar c  ,int x , int y ){

        if(c != 'r' || r == 0) return;

        Layer* l = this->rules->getLayer(r->layer());
        QString res = l->res;
        if(res == "") return; //Return if resistor layer is undefined

        Rect * rc = new Rect(res,translateX(x),translateY(y),xspace_,currentHeight_);
        this->add(rc);
    }

    void PatternHighResistor::endFillCoordinate(QMap<QString,QVariant> &data)
    {

        if(data.contains("nf")){
            int nf = data["nf"].toInt();
            res->setProperty("nf",nf);
        }

    }

}
