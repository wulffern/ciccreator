//====================================================================
//        Copyright (c) 2016 Carsten Wulff Software, Norway 
// ===================================================================
// Created       : wulff at 2016-9-3
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




#ifndef CIC_CELLS_CAPCELLV2_H
#define CIC_CELLS_CAPCELLV2_H

#include "core/layoutcell.h"

namespace cIcCells{

    class CapCellV2: public cIcCore::LayoutCell
    {
        Q_OBJECT

    public:
        virtual void place();
        virtual Rect calcBoundingRect();
        void addContacts(QString name, QString node,int y,QList<int> array);
        Rect* getAvssConnectRect(Rect* rect);
        
        
    private:
        int msw = 0;
        int xorg = 0;
        int yMax = 0;
        

    };


};
    Q_DECLARE_METATYPE(cIcCells::CapCellV2)

#endif 
