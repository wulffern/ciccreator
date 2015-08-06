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
     mos_ = new Mosfet();
     this->spiceObject_ = mos_;
  }

  PatternTransistor::~PatternTransistor()
  {

  }

  QHash<QString,QVariant> PatternTransistor::initFillCoordinates(){
      QHash<QString,QVariant> data;
      data["isTransistor"] = false;
      data["wmin"] = std::numeric_limits<int>::max();
      data["wmax"] = -std::numeric_limits<int>::max();
      data["pofinger"] = 0;
      data["nf"] = 0;
      data["useMinLength"] = false;
      return data;
  }

  void PatternTransistor::onFillCoordinate(QChar c, QString layer, int x, int y, QHash<QString,QVariant> &data){

    if(layer == "PO" && rectangle_strings_.contains("OD") && rectangle_strings_["OD"].contains(x)
       && rectangle_strings_["OD"][x].contains(y) ){

        data["isTransistor"] = true;


        int pofinger = data["pofinger"].toInt();
        if(pofinger != y){
            data["nf"] = data["nf"].toInt() +  1;
          }
        data["pofinger"] = y;

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


  void PatternTransistor::endFillCoordinate(QHash<QString,QVariant> &data){
    if(data["isTransistor"].toBool()){

        int width = (data["wmax"].toInt() - data["wmin"].toInt() + 1)*this->xspace_;
        mos_->width = this->rules->toMicron(width);
        if(data["useMinLength"].toBool()){
            int minlength = this->minPolyLength_;
            if(minlength == 0){
              minlength  = this->rules->get("PO","mingatelength");
              }
            mos_->length = this->rules->toMicron(minlength);
          }else{
            mos_->length = this->rules->toMicron(rules->get("PO","mingatelength"));
          }
        mos_->numberOfFingers = data["nf"].toInt();
        mos_->drainWidth = this->rules->toMicron(this->yspace_);
        mos_->sourceWidth = this->rules->toMicron(this->yspace_);
      }
  }
}

