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
#ifndef CIC_CORE_INSTANCE_H
#define CIC_CORE_INSTANCE_H

#include <QObject>
#include "cell.h"

namespace cIcCore{

    class Instance : public Cell
    {

        Q_OBJECT

    public:
        Instance();
        Instance(const Instance&);
        ~Instance();
        Cell * cell(){return _cell;}
        virtual Rect calcBoundingRect();
        // Cell * addInstance(QString cell);
        static Instance * getInstance(QString cell);

    private:
        Cell * _cell;

    };



}

Q_DECLARE_METATYPE(cIcCore::Instance)

#endif // INSTANCE_H
