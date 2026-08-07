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



#ifndef CIC_CORE_PATTERNHIGHRESISTOR_H
#define CIC_CORE_PATTERNHIGHRESISTOR_H

#include <QObject>
#include "cell.h"
#include "patterntile.h"
#include "spice/resistor.h"



namespace cIcCore{

    using namespace cIcSpice;

    class PatternHighResistor : public PatternTile
    {
        Q_OBJECT

        /*!
         * \brief Stripes run horizontally rather than vertically.
         *
         * The measurements this class takes are axis dependent: a
         * finger is a column, the stripe width is the column pitch and
         * the length is the height of the OP rectangle. All three hold
         * only while the poly runs up the cell. Turn the resistor on
         * its side and every one of them is on the wrong axis -- a four
         * stripe cell reports 34 fingers of the column width, which is
         * the drawn length mistaken for a count.
         *
         * With this set the three swap together. It changes no
         * geometry: the cell is drawn by PatternTile either way, and
         * this only decides how the device is measured for the netlist.
         *
         * \accessors transposed(), setTransposed()
         */
        Q_PROPERTY(bool transposed READ transposed WRITE setTransposed)

    public:
        virtual void paintRect(Rect*, QChar ,int , int );
        PatternHighResistor();
        ~PatternHighResistor();
        virtual void onFillCoordinate(QChar c, QString layer, int x, int y, QMap<QString,QVariant> &data);
        void onPaintEnclosure(Rect* r);
        void endFillCoordinate(QMap<QString,QVariant> &data);

        bool transposed(){return transposed_;}
        bool setTransposed(bool val){ transposed_ = val; return transposed_; }

    protected:
        Resistor * res;
        bool transposed_;
    };

}

Q_DECLARE_METATYPE(cIcCore::PatternHighResistor)

#endif
