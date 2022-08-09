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



#ifndef CIC_SPICE_RESISTOR
#define CIC_SPICE_RESISTOR

#include <QString>
#include "core/rules.h"
#include "spice/spicedevice.h"
#include "spice/subcktinstance.h"
 
namespace cIcSpice{

    class Resistor : public SpiceDevice
    {

        Q_OBJECT

    public:

        Resistor(QStringList n );
        Resistor();
        
        void init(QStringList n);
        
        Resistor(const Resistor& mos);
        ~Resistor();
        virtual QString toSpice( QString instance, QStringList nodes);
        virtual QJsonObject toJson();
            virtual void fromJson(QJsonObject o);
        virtual QString toSpice( );

        double width;
        
    };

}

Q_DECLARE_METATYPE(cIcSpice::Resistor)

#endif 
