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

#include "core/console.h"


namespace cIcCore{

    Console::Console(int argc, char* argv[])
    {

        this->initialize(argc, argv);
    }

    void Console::initialize(int argc, char* argv[])
    {
        this->initialize();
    }

    void Console::initialize()
    {
    }
    
    
    void Console::addOption(Option* o)
    {
        options.insert(make_pair(o->name,o));
    }

    Option* Console::option(string name)
    {
        auto iter = options.find(name);
        if(iter != options.end()){
            return options[name];
        }else{
            Option* o = new Option();
            o->type = Option::valueType::EMPTY;
            return o;            
        }
        
    }

}

