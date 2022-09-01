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
        res = new Resistor();
        res->setDeviceName("rppo");
        res->setNodes(QStringList() << "N" << "P" << "B") ;
        Subckt * ckt = new Subckt();
        ckt->setNodes(res->nodes());
        ckt->add(res);
        this->setSubckt(ckt);


    }

    PatternHighResistor::PatternHighResistor(const PatternHighResistor& mos)
    {
    }


    PatternHighResistor::~PatternHighResistor()
    {

    }

    void PatternHighResistor::onFillCoordinate(QChar c, QString layer, int x, int y, QMap<QString,QVariant> &data){



        if(layer.startsWith("PO")){

            int pofinger = data["pofinger"].toInt();
            if(pofinger < x){
                data["nf"] = data["nf"].toInt() +  1;
                data["pofinger"] = x;
            }


            res->setProperty("width",this->rules->toMicron(xspace_));
        }
    }


    void PatternHighResistor::onPaintEnclosure(Rect* r)
    {
        if(r->layer() == "OP"){
            res->setProperty("length",this->rules->toMicron( r->height() ) );
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
