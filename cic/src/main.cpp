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


#include "cic-core.h"
#include <iostream>
#include <QDebug>
#include <QString>



int main(int argc, char *argv[])
{
    try
    {

        QStringList includePaths;

        if(argc >=  3){
            QStringList arguments;

            //Parse options
            for(int i=0;i<argc;i++){
                QString arg = argv[i];
                
                if(arg == "--I" && (i+1)< argc){
                    includePaths.append(argv[i+1]);
                    i  = i+1;
                }else{
                    arguments.append(arg);
                }
            }
            
            
            QString file = arguments[1];
            QString rules = arguments[2];
            QString library = arguments[3];

            if(library == ""){
                QRegularExpression re("/?([^\\/]+)\\.json");
                QRegularExpressionMatch m = re.match(file);
                library = m.captured(1);
            }

            //Load rules
            cIcCore::Rules::loadRules(rules);

            //Load design, this is where the magic happens
            cIcCore::Design * d = new cIcCore::Design();
            foreach(QString path,includePaths){
                d->addIncludePath(path);
            }
            
            d->read(file);

            //Print SPICE file
            cIcPrinter::Spice * spice = new cIcPrinter::Spice(library);
            spice->print(d);
            delete(spice);

            cIcPrinter::Cics * cics = new cIcPrinter::Cics(library);
            cics->print(d);
            delete(cics);


            //Write GDS
            cIcCore::ConsoleOutput console;
            console.comment("Writing GDS");
            cIcPrinter::Gds * gd = new cIcPrinter::Gds(library);
            gd->print(d);
            delete(gd);

            //Write JSON
            d->writeJsonFile(library + ".cicl");

        }else{
            qWarning() << "Usage: cic <JSON file> <Technology file> [<Output name>]";
            qWarning() << "Example: cic ALGIC003_STDLIB.json ST_28NM_FDSOI.tech";
            qWarning() << "About: cIcCreator reads a JSON object definition file, technology rule file\n and a SPICE netlist (assumes same name as object definition file)\n and outputs a; SPICE netlist, connectivity description (*.cics),\n a layout description (*.cicl), and a GDSII file.";
        }

    }catch(...){

        return -1;
    }





}
