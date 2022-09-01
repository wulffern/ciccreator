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
#ifndef CIC_SPICE_DEVICE_H
#define CIC_SPICE_DEVICE_H

#include <QObject>
#include <QTextStream>
#include <QRegularExpression>
#include <QJsonObject>
#include <QJsonArray>
#include "spice/spiceobject.h"
#include "spice/subcktinstance.h"

namespace cIcSpice{

    class SpiceDevice: public SpiceObject
    {

        Q_OBJECT

    public:
        SpiceDevice();
        SpiceDevice(const SpiceDevice&);
        ~SpiceDevice();

        virtual QJsonObject toJson();
        virtual QString toSpice( QString instance, QStringList nodes);
        virtual QString toSpice();
            virtual void fromJson(QJsonObject o);
        

    protected:
        QList<SubcktInstance*> instances;

    private:

    };


}

Q_DECLARE_METATYPE(cIcSpice::SpiceDevice)


#endif
