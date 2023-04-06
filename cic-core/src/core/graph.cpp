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

#include "graph.h"

namespace cIcCore{

    QJsonObject Graph::toJson(){
        QJsonObject o;
        o["node"] = name;
        QJsonArray pa;
        foreach(auto * p, this->ports){
            QJsonObject op;

            if(p->isInstancePort()){
                InstancePort * pi = static_cast<InstancePort*>(p);
                Instance* i = static_cast<Instance*>(pi->parent());
                op["node"] = pi->childName();
                op["inst"] = i->instanceName();
                pa.append(op);
            }

        }
        o["instances"] = pa;

        return o;
    }

}
