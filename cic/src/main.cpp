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
#include "version.h"


//- Default



int main(int argc, char *argv[])
{
    int error = 0;
    try
    {

        QStringList includePaths;
        QStringList libPaths;
        QString prefix = "";

        QStringList arguments;

        //Parse options
        for(int i=0;i<argc;i++){
            QString arg = argv[i];

            if(arg == "--I" && (i+1)< argc){
                includePaths.append(argv[i+1]);
                i  = i+1;
            }else if(arg == "--L" && (i+1)< argc){
                libPaths.append(argv[i+1]);
                i  = i+1;
            }else if(arg == "--prefix" && (i+1)< argc){
                prefix = argv[i+1];
                i  = i+1;
            }else{
                arguments.append(arg);
            }
        }


        if(arguments.length() >=  3){


            //Store info on this run
            QJsonObject info;


            QString file = arguments[1];
            info["file"] = file;
            QString rules = arguments[2];
            info["rules"] = rules;
            info["includepaths"] = includePaths.join(" ");
            info["arguments"] = arguments.join(" ");
            info["libpaths"] = libPaths.join(" ");
            info["prefix"] = prefix;


            QString library ="" ;
            if(arguments.length() > 3){
                library = arguments[3];

            }


            if(library == ""){
                QRegularExpression re("/?([^\\/]+)\\.json");
                QRegularExpressionMatch m = re.match(file);
                library = m.captured(1);
            }

            info["library"] = library;

            //Load rules
            cIcCore::Rules::loadRules(rules);


            //Load design, this is where the magic happens
            cIcCore::Design * d = new cIcCore::Design();

            d->setPrefix(prefix);

            foreach(QString path,includePaths){
                d->addIncludePath(path);
            }


            if(d->read(file)){

                info["version"] = CICVERSION;
                info["hash"] = CICHASH;


                //Write JSON
                d->writeJsonFile(library + ".cic",info);

            }


        }else{
            throw "Die";
        }

    }catch(const std::exception &exc){
        qWarning() << "Usage: cic <JSON file> <Technology file> [<Output name>]";
        qWarning() << "Example: cic examples/SAR_ESSCIRC16_28N.json examples/tech.json SAR_ESSCIRC16_28N";
        qWarning() << "About: \n\tcIcCreator reads a JSON object definition file, technology rule file\n" <<
            "\tand a SPICE netlist (assumes same name as object definition file)\n\tand outputs a cic description file (.cic)." <<
            "\n\tUse cicpy (https://github.com/wulffern/cicpy) to transpile to other formats."
            "\n\tVersion" << CICVERSION <<
            "\n\tHash" << CICHASH <<
            "\nOptions:\n" <<
            "\t --I\t <path> \t Path to search for include files\n" <<
            "\t --L\t <cic file> \t Import compiled CIC file as a library\n" <<
             "";
        error = 1;

    }

    return error;





}
