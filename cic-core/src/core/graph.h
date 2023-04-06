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

#ifndef CIC_CORE_GRAPH_H
#define CIC_CORE_GRAPH_H

#include "instance.h"

namespace cIcCore{

    class Graph{


        public:
            QList<Port*> ports;
            QString name;
            void append(Port * p){

                //- Don't add a port to the graph if it's already there
                if(!ports.contains(p)){
                    ports.append(p);
                }

            }

            QJsonObject toJson();


            QList<Rect*> getRectangles(QString excludeInstances,QString includeInstances, QString layer){
                QList<Rect*> rects;
                foreach(Port *p, ports){
                    Rect * r = p->parent();
                    if(r == NULL) continue;
                    if(!r->isInstance()) continue;
                    Instance *i = static_cast<Instance *>(r);

                    QString instanceName = i->instanceName();

                    if(excludeInstances != "" && (instanceName.contains(QRegularExpression(excludeInstances))
                                                  || i->name().contains(QRegularExpression(excludeInstances)) )) continue;

                    if(includeInstances != "" && !(i->name().contains(QRegularExpression(includeInstances))
                                                   || instanceName.contains(QRegularExpression(includeInstances)) ) ) continue;
                    Rect * rp = p->get(layer);

                    if(rp == NULL) rp = p->get();
                    if(rp != NULL) {
                        rects.append(rp);

                    }
                }
                return rects;
            }




    };

};

#endif
