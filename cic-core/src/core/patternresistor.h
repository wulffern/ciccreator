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



#ifndef CIC_CORE_PATTERNRESISTOR_H
#define CIC_CORE_PATTERNRESISTOR_H

#include <QObject>
#include "cell.h"
#include "patterntile.h"
#include "spice/resistor.h"



namespace cIcCore{

    using namespace cIcSpice;

    class PatternResistor : public PatternTile
    {
        Q_OBJECT

    public:
        virtual void paintRect(Rect*, QChar ,int , int );
        PatternResistor();
        PatternResistor(const PatternResistor& mos);
        ~PatternResistor();
        virtual QMap<QString,QVariant>  initFillCoordinates();
        virtual void onFillCoordinate(QChar c, QString layer, int x, int y, QMap<QString,QVariant> &data);
        virtual void endFillCoordinate(QMap<QString, QVariant> &data);


    protected:
        Resistor * res;
    };

}

Q_DECLARE_METATYPE(cIcCore::PatternResistor)

#endif
