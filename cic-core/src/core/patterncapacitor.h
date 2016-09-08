//====================================================================
//        Copyright (c) 2015 Carsten Wulff Software, Norway
// ===================================================================
// Created       : wulff at 2015-04-03
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


#ifndef CIC_CORE_PATTERNCAPACITOR_H
#define CIC_CORE_PATTERNCAPACITOR_H

#include <QObject>
#include "cell.h"
#include "patterntile.h"
#include "spice/capacitor.h"



namespace cIcCore{

    using namespace cIcSpice;

    class PatternCapacitor : public PatternTile
    {
        Q_OBJECT

    public:
        virtual void paintRect(Rect*, QChar ,int , int );
        PatternCapacitor();
        ~PatternCapacitor();

    protected:
        Capacitor * cap_;
    };

	class PatternCapacitorGnd : public PatternCapacitor
    {
        Q_OBJECT

    public:

        PatternCapacitorGnd();
        ~PatternCapacitorGnd();

    };
}

Q_DECLARE_METATYPE(cIcCore::PatternCapacitor)
Q_DECLARE_METATYPE(cIcCore::PatternCapacitorGnd)

#endif // PATTERNCAPACITOR_H