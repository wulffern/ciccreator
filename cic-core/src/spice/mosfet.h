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

#ifndef CIC_SPICE_MOSFET
#define CIC_SPICE_MOSFET

#include <QString>
#include "core/rules.h"
#include "spice/spicedevice.h"


namespace cIcSpice{

    class Mosfet : public SpiceDevice
    {
        Q_OBJECT

    public:
        Mosfet();
        Mosfet(const Mosfet& mos);
        ~Mosfet();
        virtual QString toSpice( QString instance, QStringList nodes);
        virtual QJsonObject toJson();
            virtual void fromJson(QJsonObject o);
        virtual QString toSpice();
        
    };



}

Q_DECLARE_METATYPE(cIcSpice::Mosfet)

#endif // MOSFET
