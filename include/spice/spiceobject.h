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


#ifndef CIC_CORE_SPICEOBJECT_H
#define CIC_CORE_SPICEOBJECT_H

#include <QObject>
#include <QRegularExpression>

namespace cIcSpice{

	class SpiceObject: public QObject
	{
		Q_OBJECT
		Q_PROPERTY(QString name READ name WRITE setName)
		Q_PROPERTY(QStringList spiceStr READ spiceStr WRITE setSpiceStr)
		Q_PROPERTY(QStringList nodes READ nodes WRITE setNodes)
		Q_PROPERTY(int lineNumber READ lineNumber WRITE setLineNumber)



	public:
		SpiceObject();
		SpiceObject(const SpiceObject&);
		~SpiceObject();

		QString name(){return _name;}
		QString setName(QString val){ _name = val; return _name;}

		QStringList spiceStr(){return _spice_str;}
		QStringList setSpiceStr(QStringList val){ _spice_str = val; return _spice_str;}

		QStringList nodes(){return _nodes;}
		QStringList setNodes(QStringList val){ _nodes = val; return _nodes;}

		QMap<QString,QString> properties(){
		  return _properties;
		}

		int lineNumber(){return _line_number;}
		int setLineNumber(int val){ _line_number = val; return _line_number;}


	private:
		QString _name;
		QMap<QString,QString> _properties;
		QStringList _spice_str;
		QStringList _nodes;
		QList<SpiceObject*> _children;
		int _line_number;

		
	};


}


#endif
