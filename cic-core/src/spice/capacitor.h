//====================================================================
//        Copyright (c) 2016 Carsten Wulff Software, Norway 
// ===================================================================
// Created       : wulff at 2016-5-19
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


#ifndef CIC_SPICE_CAPACITOR
#define CIC_SPICE_CAPACITOR

#include <QString>
#include <QStringList>
#include "spice/subcktinstance.h"
#include "core/rules.h"
#include "spice/spicedevice.h"

namespace cIcSpice{

    class Capacitor : public SpiceDevice
    {

        Q_OBJECT

    public:
        Capacitor(QStringList n);
        Capacitor();
        
        void init(QStringList n);
         
        Capacitor(const Capacitor& mos);
        ~Capacitor();
        virtual QString toSpice( QString instance, QStringList nodes);
        virtual QJsonObject toJson();
    };

}

Q_DECLARE_METATYPE(cIcSpice::Capacitor)

#endif // MOSFET
