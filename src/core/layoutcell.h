//====================================================================
//        Copyright (c) 2015 Carsten Wulff Software, Norway
// ===================================================================
// Created       : wulff at 2015-8-20
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

#ifndef CIC_CORE_LAYOUTCELL_H
#define CIC_CORE_LAYOUTCELL_H

#include <QObject>
#include "cell.h"
#include "instance.h"
#include "route.h"

namespace cIcCore{

    class LayoutCell : public Cell
    {
        Q_OBJECT

    public:
        LayoutCell();
        LayoutCell(const LayoutCell&);
        ~LayoutCell() ;

        Q_INVOKABLE void setYoffsetHalf(QJsonValue obj);

        Q_INVOKABLE void addDirectedRoute(QJsonArray obj);

        virtual void place();
        virtual void route();

        virtual  void addAllPorts();

    private:
        bool useHalfHeight;
        QList<Route*> routes_;

    };

}

Q_DECLARE_METATYPE(cIcCore::LayoutCell)

#endif // LAYOUTCELL_H
