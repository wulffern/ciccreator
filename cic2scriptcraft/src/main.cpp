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

        if(argc >=  3){

            QString file = argv[1];
            QString rules = argv[2];
            QString library = argv[3];

            if(library == ""){
                QRegularExpression re("/?([^\\/]+)\\.json");
                QRegularExpressionMatch m = re.match(file);
                library = m.captured(1);
            }



            //Load rules
            cIcCore::Rules::loadRules(rules);

            //Load design, this is where the magic happens
            cIcCore::Design * d= new cIcCore::Design();
            d->readJsonFile(file);


            //Minecraft JavaScript
            cIcPrinter::Minecraft * m = new cIcPrinter::Minecraft(library + ".js");
            m->print(d);


        }else{
            qWarning() << "Usage: cic2scriptcraft <CIC file> <Technology file> [<Library Name>]";

        }

    }catch(...){

        return -1;
    }





}
