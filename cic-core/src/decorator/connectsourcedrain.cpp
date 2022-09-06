//====================================================================
//        Copyright (c) 2018 Carsten Wulff Software, Norway 
// ===================================================================
// Created       : wulff at 2018-3-11
// ===================================================================
//  The MIT License (MIT)
// 
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
// 
//  The above copyright notice and this permission notice shall be included in all
//  copies or substantial portions of the Software.
// 
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//  SOFTWARE.
//  
//====================================================================

#include "decorator/connectsourcedrain.h"

namespace cIcCore{

    void ConnectSourceDrain::parseOptions()
    {
        
        QJsonArray jobj = jsonval.toArray();
        
        if(jobj.size() < 3){
            qDebug() << "Error: addDirectedRoute must contain at least three elements\n";
            return;
        }

        layer = jobj[0].toString();
        routeType = jobj[1].toString();
        options = jobj[2].toString();

    }
    

    void ConnectSourceDrain::beforeRoute()
    {
        if(!layoutcell){return;}
        
            
            
        QList<Instance*> transistors;
        foreach(auto cell, layoutcell->children()){
            if(cell && cell->isInstance())
            {
                Instance * inst = static_cast<Instance*>(cell);
                if(inst->cell() && strcmp(inst->cell()->metaObject()->className(),"cIcCore::PatternTransistor") == 0)
                {
                    transistors.append(inst);
                }                
            }
        }

        if(transistors.count() == 0){
            return;
        }
        Instance * first_inst = transistors[0];
        QString prev_groupname = first_inst->subcktInstance()->groupName();
        QString prev_instname = first_inst->instanceName();

        //Have to assume drain is on top
        for(int i=1;i<transistors.count();i++){
            Instance * inst = transistors[i];
            QString instname = inst->instanceName();
            QString groupname =  inst->subcktInstance()->groupName();

            if(prev_groupname == groupname){
                QList<Rect*> start;
                QList<Rect*> stop;
                QString startrects = prev_instname+":D";
                QString stoprects = instname+":S";
                
                start = layoutcell->findAllRectangles(startrects,layer);
                stop = layoutcell->findAllRectangles(stoprects,layer);

                if(start.count() > 0 && stop.count() > 0){
                    Route * r = new Route(QString("SD%1").arg(i),layer,start,stop,options,routeType);
                    layoutcell->add(r);
                }
                
                
            }
            prev_groupname = groupname;
            prev_instname = instname;
        }
        
        


    }
    
}

