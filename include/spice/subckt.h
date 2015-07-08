//====================================================================
//        Copyright (c) 2015 Carsten Wulff Software, Norway 
// ===================================================================
// Created       : wulff at 2015-7-6
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


#ifndef CIC_SPICE_SUBCKT_H
#define CIC_SPICE_SUBCKT_H

#include <QtCore>
#include <QObject>
#include "spiceobject.h"
#include "subcktinstance.h"


namespace cIcSpice{

	class Subckt: public SpiceObject
	{
		Q_OBJECT
		Q_PROPERTY(QString name READ name WRITE setName)

	public:
		Subckt();
		Subckt(QList<QString> buffer);
		Subckt(const Subckt&);
		~Subckt();
		QString name(){return _name;}
		QString setName(QString val){ _name = val; return _name;}
		 void parse(QList<QString> buffer, int line);


	private:
		QString _name;
		QMap<QString,SubcktInstance*> _instances;
	};


}


#endif
