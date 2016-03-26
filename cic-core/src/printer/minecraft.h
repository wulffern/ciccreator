//====================================================================
//        Copyright (c) 2015 Carsten Wulff Software, Norway
// ===================================================================
// Created       : wulff at 2015-7-15
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


#ifndef CIC_PRINTER_MINECRAFT_H
#define CIC_PRINTER_MINECRAFT_H

#include <QObject>
#include <QString>
#include "printer/designprinter.h"
#include <cmath>

namespace cIcPrinter{
    using namespace cIcCore;
	using namespace std;
    class Minecraft : public DesignPrinter{

    public:

        Minecraft(QString filename):DesignPrinter(filename){
			block_map["PO"] = "'35:14'";
			block_map["M1"] = "'35:11'";
			block_map["M2"] = "'35:4'";
			block_map["M3"] = "'35:3'";
			block_map["M4"] = "'35:9'";
			block_map["M5"] = "'35:6'";
			block_map["M6"] = "'35:8'";
			block_map["OD"] = "'35:5'";
			block_map["CO"] = "'41'";
			block_map["VIA1"] = "'35:11'";
			block_map["VIA2"] = "'35:4'";
			block_map["VIA3"] = "'35:3'";
			block_map["VIA4"] = "'35:6'";
			block_map["VIA5"] = "'35:8'";
			height_map.append("OD");
			height_map.append("PO");
			height_map.append("CO");
			height_map.append("M1");
			height_map.append("VIA1");
			height_map.append("M2");
			height_map.append("VIA2");
			height_map.append("M3");
			height_map.append("VIA3");
			height_map.append("M4");
			height_map.append("VIA4");
			height_map.append("M5");
			height_map.append("VIA5");
			height_map.append("M6");
			height_map.append("VIA6");
			cellname = "";
			count = 0;
        }
        ~Minecraft(){

        }

        virtual void startCell(Cell * cell);
        virtual void endCell();
        virtual void printPort(Port *);
        virtual void printRect(Rect * rect);
        virtual void printReference(Cell * o);

		virtual void startLib(QString name);
//        virtual void endLib();
		
		int toMine(int angstrom);

		QHash<QString,QString> block_map;
		QList<QString> height_map;

		QString cellname;
		int count;
    };
};

#endif
