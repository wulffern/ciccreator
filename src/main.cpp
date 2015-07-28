//====================================================================
//        Copyright (c) 2015 Carsten Wulff Software, Norway 
// ===================================================================
// Created       : wulff at 2015-4-19
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

#include <QApplication>
#include "core/design.h"
#include "core/rules.h"
#include "printer/svg.h"
#include "printer/gds.h"
#include "gui/window.h"
#include "core/consoleoutput.h"
#include <iostream>
#include <QDebug>
#include <QString>

int main(int argc, char *argv[])
{
	
	if(argc >=  3){

		QString file = argv[1];
		QString rules = argv[2];

		//Load rules
		cIcCore::Rules::loadRules(rules);

		//Load design
		cIcCore::Design * d = new cIcCore::Design();
		d->read(file);

		cIcPrinter::Svg * pr = new cIcPrinter::Svg("test");
		pr->print(d);


		cIcCore::ConsoleOutput console;
		console.comment("Writing GDS");
		cIcPrinter::Gds * gd = new cIcPrinter::Gds("TEST");
		gd->print(d);

	if(argc == 4){
		QApplication app(argc, argv);
		cIcGui::Window window;
		window.loadDesign(d);
		window.show();
		return app.exec();
	  }
		

	}else{
	   qWarning() << "Wrong number of arguments " << argc;
	  }

		

  
}
