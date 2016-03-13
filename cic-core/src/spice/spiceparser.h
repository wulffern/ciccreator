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


#ifndef CIC_SPICE_SPICEPARSER_H
#define CIC_SPICE_SPICEPARSER_H

#include <QObject>
#include <QMap>
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QStringBuilder>
#include "subckt.h"

namespace cIcSpice{

	class SpiceParser: public QObject
	{
		Q_OBJECT
	//	Q_PROPERTY(QMap<QString, Subckt*> subckt READ subckt)
	public:
		SpiceParser();
		SpiceParser(const SpiceParser&);
		~SpiceParser();
		void parseFile(QString filename);

		QMap<QString,Subckt*> subckt(){
		  return _subckt;
		}

		Subckt* getSubckt(QString name){
			if(_subckt.contains(name)){
				return _subckt[name];
			}else{
				return NULL;
			}
		}

		void parseSubckt(int line_number, QList<QString> subckt_buffer);
	private:
		QMap<QString, Subckt* > _subckt;

	};


}


#endif
