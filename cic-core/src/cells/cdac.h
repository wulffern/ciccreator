//====================================================================
//        Copyright (c) 2016 Carsten Wulff Software, Norway 
// ===================================================================
// Created       : wulff at 2016-9-4
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


#ifndef CIC_CELLS_CDAC_H
#define CIC_CELLS_CDAC_H

#include "core/layoutcell.h"
#include "cells/capcell.h"

namespace cIcCells{

    class CDAC: public cIcCore::LayoutCell
    {
        Q_OBJECT

    public:
        virtual void place();
        virtual void route();
        virtual void paint();
        
        
    private:
        cIcCore::Instance* firstinst;
        cIcCore::Instance* inst;

        

    };


};
Q_DECLARE_METATYPE(cIcCells::CDAC)

#endif 
