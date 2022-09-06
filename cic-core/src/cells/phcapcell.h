//====================================================================
//        Copyright (c) 2016 Carsten Wulff Software, Norway 
// ===================================================================
// Created       : wulff at 2019-06-02
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

#ifndef CIC_CELLS_PHCAPCELL_H
#define CIC_CELLS_PHCAPCELL_H

#include "core/layoutcell.h"

namespace cIcCells{

    class PhCapCell: public cIcCore::LayoutCell
    {
        Q_OBJECT

    public:
        
        Q_INVOKABLE void binaryBits(QJsonValue obj);
        Q_INVOKABLE void unaryBits(QJsonValue obj);
        
        virtual void place();
        virtual Rect calcBoundingRect();
        int addFinger(int x, int y, int msw, int mw, int mh,int i,string name);

        
    private:
        int _binaryBits  = 1;
        int _unaryBits  = 1;
        int _top_length = 1;
        int _finger_length = 1;
    };


};
    Q_DECLARE_METATYPE(cIcCells::PhCapCell)

#endif 
