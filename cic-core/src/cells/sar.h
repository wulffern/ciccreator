//====================================================================
//        Copyright (c) 2016 Carsten Wulff Software, Norway 
// ===================================================================
// Created       : wulff at 2016-8-2
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





#ifndef CIC_CELLS_SAR_H
#define CIC_CELLS_SAR_H

#include "core/layoutcell.h"
 

typedef QMap<QString,QList<cIcSpice::SubcktInstance*>>  SARgroup;



namespace cIcCells{
    
    class SAR : public cIcCore::LayoutCell
    {
        Q_OBJECT

    public:
        
        virtual void place();
        virtual void route();

        Q_INVOKABLE
        /**
        * @brief Use Metal 5 shield
        */
        void usem5(QJsonValue obj);
        
        int getCellWidth(SARgroup groups,QString group);
        cIcCore::Instance* placeAlternateMirror(SARgroup groups,QString group, int i, int x ,int y, int xoffset);
        int addSarRouting(int y,int msw,int mw);

        static bool sortGraph(cIcCore::Graph* a, cIcCore:: Graph *b);
        
        
    private:

        Rect* sarn = NULL;
        Rect* sarp = NULL;
        bool usem5_ = true;

    };
    
        
}

Q_DECLARE_METATYPE(cIcCells::SAR)

#endif

