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

#include "core/patternresistor.h"

namespace cIcCore{

    PatternResistor::PatternResistor()
    {
        res = new Resistor(QStringList() << "A" << "B");
        Subckt * ckt = new Subckt();
        ckt->setNodes(res->nodes());
        ckt->add(res);
        this->setSubckt(ckt);


    }

    QMap<QString,QVariant> PatternResistor::initFillCoordinates(){
        QMap<QString,QVariant> data;
        data["wmin"] = std::numeric_limits<int>::max();
        data["wmax"] = -std::numeric_limits<int>::max();
        data["nf"] = 0;
        data["pofinger"] = 0;
        return data;
    }

    void PatternResistor::onFillCoordinate(QChar c, QString layer, int x, int y, QMap<QString,QVariant> &data){

        
        int pofinger = data["pofinger"].toInt();
        if(pofinger < x){
            data["nf"] = data["nf"].toInt() +  1;
            data["pofinger"] = x;
        }

        if(layer == "PO"){
            res->setProperty("width",this->rules->toMicron(xspace_));
        }


    }

    void PatternResistor::endFillCoordinate(QMap<QString,QVariant> &data){

    }




    PatternResistor::PatternResistor(const PatternResistor& mos)
    {
    }


    PatternResistor::~PatternResistor()
    {

    }

    void PatternResistor::paintRect(Rect* r, QChar c  ,int x , int y ){

        if(c != 'r' || r == 0) return;

        Layer* l = this->rules->getLayer(r->layer());
        QString res_ss = l->res;
        if(res_ss == "") return; //Return if resistor layer is undefined

        Rect * rc = new Rect(res_ss,translateX(x),translateY(y),xspace_,currentHeight_);
        this->add(rc);

        
        if(c =='r'){
                this->res->setProperty("width",this->rules->toMicron( currentHeight_ ) );
                this->res->setProperty("length",this->rules->toMicron( xspace_ ) );
                this->res->setProperty("layer",r->layer());
        }

    }
}
