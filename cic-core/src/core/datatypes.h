//====================================================================
//        Copyright (c) 2016 Carsten Wulff Software, Norway
// ===================================================================
// Created       : wulff at 2016-5-26
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


#ifndef CIC_CORE_DATATYPES_H
#define CIC_CORE_DATATYPES_H

#include <QObject>
namespace cIcCore{

    class DataType{

    public:
        DataType();
        ~DataType();

		QJsonObject getFormat(){
			QJsonParseError err;
            QJsonDocument d = QJsonDocument::fromJson(format.toUtf8(),&err);
            if(QJsonParseError::NoError != err.error ){
                

                qWarning() << err.errorString()  ;
            }

			QJsonObject jformat = d.object();
		}

        void fromJson(QJsonValue val){
            QJsonObject jformat = getFormat();

//			if(jformat.

        }

    protected:

        QString format;
    };

	class DirectedRoute: public DataType{

	public:
            DirectedRoute():DataType(){
                format = '["layer:string","net:string","route:string","options:string:"]';
            }

            //! Layer to route in
            QString layer;
            //! Net to use for routing
            QString net;
            //! Route description, i.e M1:D-|--M2:S
            QString route;
            //! Options, see router.cpp
            QString options;

        };


};

Q_DECLARE_METATYPE(cIcCore::DataType)
Q_DECLARE_METATYPE(cIcCore::DirectedRoute)

#endif
