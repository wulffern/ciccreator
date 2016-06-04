//====================================================================
//        Copyright (c) 2016 Carsten Wulff Software, Norway
// ===================================================================
// Created       : wulff at 2016-3-22
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

#include "printer/minecraft.h"

namespace cIcPrinter{

    void Minecraft::printPort(Port *){

    }

    int Minecraft::toMine(int angstrom){

//		double x = angstrom;
//		double gamma = Rules::;
		
//		int
//		qDebug() << angstrom ;
//		if(x != 0 ){
//			if(x != 0 && abs(xo) > 0 && abs(xo) < 1){
//			xo = xo/abs(xo);
//		}
		
		return angstrom/500;
		;}

    void Minecraft::printReference(Cell * o){
        QTextStream ts(&file);
        if(!o->isInstance()) return;
        Instance * i = (Instance *) o;

		int lcount = count;
		count ++;
		
        ts << "  this.chkpt('r" << lcount << "')";
        int x = toMine(o->x1());
        int y = toMine(o->y1());
        if(x < 0){
            ts << ".left(" << -x << ")";
        }else{
            ts << ".right(" << x << ")";
        }


        if(y < 0){
            ts << ".back(" << -y << ")";
        }else{
            ts << ".fwd(" << y << ")";
        }

		ts << "." << o->name() << "()";
		      ts <<  ".move('r" << lcount << "');\n";

    }



    void Minecraft::printRect(Rect * o){
        QTextStream ts(&file);
        ts << "  this.chkpt('r" << count << "')";
        int x = toMine(o->x1());
        int y = toMine(o->y1());

        if(x < 0){
            ts << ".left(" << -x << ")";
        }else{
            ts << ".right(" << x << ")";
        }

        if(y < 0){
            ts << ".back(" << -y << ")";
        }else{
            ts << ".fwd(" << y << ")";
        }


        QString map = "blocks.oak";
        if(block_map.contains(o->layer())){
            map = block_map[o->layer()];

            int height = 0;
            if(height_map.contains(o->layer())){
                height = height_map.indexOf(o->layer());
            }
            ts << ".up(" << height << ")";
            ts <<  ".box(" << map << "," << toMine(o->width())  << ",1," << toMine(o->height()) << ")";
        }

        ts << ".move('r" << count << "');\n";
		count++;
    };


    void Minecraft::startCell(Cell *cell){
        DesignPrinter::startCell(cell);
        if(cell == NULL) return;
        QTextStream ts(&file);
        cellname = cell->name();
        ts << "\n\n";
        ts << "function " << cellname << "() {\n";
        ts << "  chkpt('" << cellname << "');\n";

    }

	void Minecraft::startLib(QString name){
		DesignPrinter::startLib(name);
		QTextStream ts(&file);
		ts << "var Drone = require('drone');\n";
	}


    void Minecraft::endCell(){
        QTextStream ts(&file);
        ts << "  this.move('" << cellname << "');\n";
        ts << "}\n";
        ts << "Drone.extend(" << cellname << ");\n";

    }


};
