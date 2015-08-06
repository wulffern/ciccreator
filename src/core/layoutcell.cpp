//====================================================================
//        Copyright (c) 2015 Carsten Wulff Software, Norway 
// ===================================================================
// Created       : wulff at 2015-7-14
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
#include "core/layoutcell.h"

namespace cIcCore{

    LayoutCell::LayoutCell()
    {

    }

    LayoutCell::LayoutCell(const LayoutCell&){

    }

    LayoutCell::~LayoutCell()
    {

    }

    void LayoutCell::setYoffsetHalf(QJsonValue obj){
          useHalfHeight = true;
    }

    void LayoutCell::place(){

        QString prev_group = "";
        int prev_width = 0;
        int x = 0;
        int y = 0;
        foreach(cIcSpice::SubcktInstance * ckt_inst,_subckt->instances()){
            QString group = ckt_inst->groupName();

            if(prev_group.compare(group) != 0  && prev_group.compare("")  != 0){
                y = 0;
                x = x + prev_width;
            }
            prev_group = group;

            Instance * inst = Instance::getInstance(ckt_inst->subcktName());
            inst->setSubcktInstance(ckt_inst);

            this->add(inst);
            inst->moveTo(x,y);
            prev_width = inst->width();
            if(useHalfHeight){
            y += inst->height()/2;
              }else{
                  y += inst->height();
              }

        }

        this->updateBoundingRect();

    }

}
