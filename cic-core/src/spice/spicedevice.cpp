//====================================================================
//        Copyright (c) 2017 Carsten Wulff Software, Norway
// ===================================================================
// Created       : wulff at 2017-3-11
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

#include "spice/spicedevice.h"
namespace cIcSpice{

    SpiceDevice::SpiceDevice()
    {
        
    }

    SpiceDevice::SpiceDevice(const SpiceDevice &d)
    {
        
    }
    
    SpiceDevice::~SpiceDevice()
    {
        
    }

    QString SpiceDevice::toSpice()
    {
    }
    

    QString SpiceDevice::toSpice( QString instance, QStringList nodes)
    {
        
    }
    
    
QJsonObject SpiceDevice::toJson()
{

    QJsonObject o = SpiceObject::toJson();

    QJsonArray ar;
//    foreach(SubcktInstance* i, instances){
    //        QJsonObject oi = i->toJson();
    //ar.append(oi);
//    }

    //  o["instances"] = ar;
//    o.remove("deviceName");

    return o;


}

    void SpiceDevice::fromJson(QJsonObject o){
        SpiceObject::fromJson(o);
    }

}
