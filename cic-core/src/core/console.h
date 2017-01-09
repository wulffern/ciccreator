//====================================================================
//        Copyright (c) 2016 Carsten Wulff Software, Norway
// ===================================================================
// Created       : wulff at 2016-12-26
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

#ifndef CONSOLE_H
#define CONSOLE_H

#include <iostream>
#include <string>
#include <map>


namespace cIcCore{
    using namespace std;

    class Option
    {
    public:
        enum valueType{INT,DOUBLE,STRING,EMPTY};

        valueType type;
        int ival;
        double dval;
        string sval;
        string name;
        string getopt;

        Option()
        {
            type = EMPTY;

        }
        
        
        Option(string getopt, string name, int val)
        {
            getopt = getopt;
            name = name;
            ival = val;
            type = INT;
        }

        Option(string getopt, string name, double val)
        {
            getopt = getopt;
            name = name;
            dval = val;
            type = DOUBLE;
        }

        Option(string getopt, string name, string val)
        {
            getopt = getopt;
            name = name;
            sval = val;
            type = STRING;
        }
    };



    class Console
    {

    public:
        Console(int argc, char* argv[]);
        ~Console();
        virtual void initialize(int argc, char* argv[]);
        virtual void initialize();
        void addOption(Option* o );
        Option* option(string name);

    private:
        map<string,Option*> options;

    };

}

#endif
