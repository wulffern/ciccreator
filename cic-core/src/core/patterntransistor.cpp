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

#include "core/patterntransistor.h"

namespace cIcCore{


    PatternTransistor::PatternTransistor():PatternTile()
    {
        
        mos = new Mosfet();
        Subckt * ckt = new Subckt();
        ckt->setNodes(mos->nodes());
        ckt->add(mos);
        this->setSubckt(ckt);
    }

    PatternTransistor::~PatternTransistor()
    {

    }

    QMap<QString,QVariant> PatternTransistor::initFillCoordinates(){
        QMap<QString,QVariant> data;
        data["isTransistor"] = false;
        data["wmin"] = std::numeric_limits<int>::max();
        data["wmax"] = -std::numeric_limits<int>::max();
        data["pofinger"] = 0;
        data["nf"] = 0;
        data["useMinLength"] = false;
        return data;
    }

	void PatternTransistor::paintRect(Rect *r, QChar c, int x, int y){
		if(r->layer() == "PO" && rectangle_strings_.contains("OD") && rectangle_strings_["OD"].contains(x)  && rectangle_strings_["OD"][x].contains(y)){

            //Update length of transistor
            mos->setProperty("length",this->rules->toMicron(r->height()));

		}
	}

    void PatternTransistor::onFillCoordinate(QChar c, QString layer, int x, int y, QMap<QString,QVariant> &data){


        // Is this a gate?
        if(layer == "PO" && c == 'G'){
            int pofinger = data["pofinger"].toInt();
            if(pofinger != y){
                data["nf"] = data["nf"].toInt() +  1;
            }
            data["pofinger"] = y;
        }
        
        
        if(layer == "PO" && rectangle_strings_.contains("OD") && rectangle_strings_["OD"].contains(x)
           && rectangle_strings_["OD"][x].contains(y) ){

            data["isTransistor"] = true;            


            if(x < data["wmin"].toInt()){
                data["wmin"] = x;
            }

            if(x > data["wmax"].toInt()){
                data["wmax"] = x;
            }


            if(c == 'm'){
                data["useMinLength"] = true;
            }

        }
    }


    void PatternTransistor::endFillCoordinate(QMap<QString,QVariant> &data){
        if(data["isTransistor"].toBool()){


            int width = (data["wmax"].toInt() - data["wmin"].toInt() + 1)*this->xspace_;

            int minlength = this->minPolyLength_;
            if(minlength == 0){
                minlength  = this->rules->get("PO","mingatelength");
            }

            mos->setProperty("width",this->rules->toMicron(width));
            mos->setProperty("length",this->rules->toMicron(minlength));
            mos->setProperty("nf",data["nf"].toInt());
            mos->setProperty("drainWidth",this->rules->toMicron(this->yspace_));
            mos->setProperty("sourceWidth",this->rules->toMicron(this->yspace_));
        }
    }

    QJsonObject PatternTransistor::toJson(){
        QJsonObject o = PatternTile::toJson();
        return o;
    }

    void PatternTransistor::fromJson(QJsonObject o){
        PatternTile::fromJson(o);
    }
}
