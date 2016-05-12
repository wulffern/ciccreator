//====================================================================
//        Copyright (c) 2015 Carsten Wulff Software, Norway
// ===================================================================
// Created       : wulff at 2015-7-14
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
#include "core/layoutcell.h"

namespace cIcCore{

    LayoutCell::LayoutCell(): Cell()
    {
        useHalfHeight = false;
        noPowerRoute_ = false;
    }

    LayoutCell::LayoutCell(const LayoutCell& cell): Cell(cell){
        useHalfHeight = false;
        noPowerRoute_ = false;
    }

    LayoutCell::~LayoutCell()
    {
        useHalfHeight = false;
    }

    void LayoutCell::setYoffsetHalf(QJsonValue obj){
        useHalfHeight = true;
    }

    void LayoutCell::noPowerRoute(QJsonValue obj){
        noPowerRoute_ = true;
    }

    void LayoutCell::addDirectedRoute(QJsonArray obj){

        if(obj.size() < 3){
            qDebug() << "Error: addDirectedRoute must contain at least three elements\n";
            return;
        }

        QString layer = obj[0].toString();
        QString net = obj[1].toString();
        QString route = obj[2].toString();
        QString startRegex;
        QString routeType;
        QString stopRegex;
        QList<Rect*> start;
        QList<Rect*> stop;

        //Decode route
        QRegularExpression re("^([^-\\|<>]*)([-\\|<>]+)([^-\\|<>]*)$");
        QRegularExpressionMatch m_route = re.match(route);
        if(m_route.hasMatch()){
            startRegex = m_route.captured(1);
            routeType = m_route.captured(2);
            stopRegex = m_route.captured(3);

            start = this->findAllRectangles(startRegex,layer);
            stop = this->findAllRectangles(stopRegex,layer);

        }else{
            qDebug() << "Error: Could not parse route command '" << route << "'\n";
            return;
        }

        QString options = "";
        if(obj.size() > 3){
            options = obj[3].toString();
        }

        if(start.count() > 0 && stop.count() > 0){


            Route * r = new Route(net,layer,start,stop,options,routeType);
            // r->setPoint1(this->x1(),this->y1());
            // routes_.append(r);
            this->add(r);
        }else{
            qDebug() << "Error: Route did not work [" << layer << net << route << options << "] stop="<< stop.count() << " start=" <<start.count();
        }
    }

    void LayoutCell::addConnectivityRoute(QJsonArray obj)
    {

        if(obj.size() < 3){
            qDebug() << "Error: addDirectedRoute must contain at least three elements\n";
            return;
        }

        QString layer = obj[0].toString();
        QString regex = obj[1].toString();
        QString routeType = obj[2].toString();
        QString options = "";
        QString cuts = "";
        QString excludeInstances = "";
		QString includeInstances = "";
        if(obj.size() > 3){
            options = obj[3].toString();
        }

        if(obj.size() > 4){
            cuts = obj[4].toString();
        }
        if(obj.size() > 5){
            includeInstances = obj[5].toString();
        }

        foreach(QString node, nodeGraph_.keys()){
            if(!node.contains(QRegularExpression(regex))) continue;

            Graph * g = nodeGraph_[node];
            QList<Rect*>  rects = g->getRectangles(excludeInstances,includeInstances,layer);

            if(rects.count() == 0){
                qDebug() << "Could not find rectangles on " << node << regex << rects.count() << "\n";
            }
            if(rects.count() > 0){
                QList<Rect*> empty;
                Route * r = new Route(regex,layer,empty,rects,options,routeType);
                this->add(r);
            }
        }


    }

    void LayoutCell::addPortOnRect(QJsonArray obj)
    {
        if(obj.size() < 2){
            qDebug() << "Error: addPortsOnRect must contain at least two elements\n";
            return;
        }

        QString port = obj[0].toString();
        QString layer = obj[1].toString();
        QString path = port;

        if(obj.size() == 3){
            path = obj[2].toString();
        }

        QList<Rect*> rects = this->findAllRectangles(path,layer);
        if( rects.count() == 0){
            qDebug()<< "Could not find port " << port << "on path " << path << " in layer " << layer;
        }else{
            Rect * r = rects[0];
            if(r->layer() != layer){
                qDebug()<< "Layer " << r->layer() << " is different from " << port << "on rect " << path << " in layer " << layer;
            }else{
                if(ports_.contains(port)){
                    Port *p = ports_[port];
                    p->set(r);
                }else{
                    Port * p = new Port(port);
                    p->set(r);
                    this->add(p);


                }
            }
        }


    }

    void LayoutCell::addVia(QJsonArray obj)
    {
        if(obj.size() < 5){
            qDebug() << "Error: addVia must contain at least five elements\n";
            return;
        }

        QString startlayer = obj[0].toString();
        QString stoplayer = obj[1].toString();
        QString path = obj[2].toString();
        int hcuts = obj[3].toInt();
        int vcuts =  obj[4].toInt();

        double offset = (obj.size() > 5) ? obj[5].toDouble() : 0;
        QString name = (obj.size() > 6) ? obj[6].toString() : "";



        QList<Rect*> rects = this->findAllRectangles(path,startlayer);


        foreach(Rect* r, rects){
            if(r == 0) continue;
            Instance * inst= Cut::getInstance(startlayer,stoplayer,hcuts,vcuts);
            inst->moveTo(r->x1() + offset*this->rules->get("ROUTE","horizontalgrid"),r->y1());


            if(name != ""){
                Rect * p = inst->getRect(stoplayer);

                if(p != 0) named_rects_[name] = p;
                else qDebug() << "Error: Unknown rect " << name;

            }

            this->add(inst);

        }




    }

    void LayoutCell::addPortVia(QJsonArray obj)
    {
        if(obj.size() < 8){
            qDebug() << "Error: addPortVia must contain at least 8 elements\n";
            return;
        }

        QString startlayer = obj[0].toString();
        QString stoplayer = obj[1].toString();
        QString port = obj[2].toString();
        QString path = obj[3].toString();
        int vcuts = obj[4].toInt();
        int hcuts =  obj[5].toInt();
        double xoffset = obj[6].toDouble();
        double yoffset = obj[7].toDouble();
		QString name = (obj.size() > 8) ? obj[8].toString() : "";

        QList<Rect*> rects = this->findAllRectangles(path,startlayer);

        foreach(Rect* r, rects){
            if(r == 0) continue;
            Instance * inst= Cut::getInstance(startlayer,stoplayer,hcuts,vcuts);
			qDebug() << "Y-offset" << yoffset << obj[7] << "\n";
            inst->moveTo(r->x2() + xoffset*inst->width(),r->centerY() + yoffset*inst->height());

            Rect * r = inst->getRect(stoplayer);

            if(name != ""){
                if(r != 0) named_rects_[name] = r;
                else qDebug() << "Error: Unknown rect " << name;

            }
            if(ports_.contains(port)){
                Port *p = ports_[port];
                p->set(r);
            }else{
                Port * p = new Port(port);
                p->set(r);
                this->add(p);
            }
			this->add(inst);


        }




    }









//------------------------------------------------------------------------------------------
// Internal functions
//------------------------------------------------------------------------------------------






    void LayoutCell::place(){

        QString prev_group = "";
        int prev_width = 0;
        int x = 0;
        int y = 0;
        foreach(cIcSpice::SubcktInstance * ckt_inst,_subckt->instances()){
            QString group = ckt_inst->groupName();

            if(prev_group.compare(group) != 0  && prev_group.compare("")  != 0){
                y = 0;
                x = x + prev_width;
            }
            prev_group = group;

            //The chain of events is important here, ports get defined in the setSubckInstance
            Instance * inst = Instance::getInstance(ckt_inst->subcktName());
            inst->setSubcktInstance(ckt_inst);
            this->add(inst);
            inst->moveTo(x,y);
            this->addToNodeGraph(inst);
            prev_width = inst->width();
            if(useHalfHeight){
                y += inst->height()/2;
            }else{
                y += inst->height();
            }


        }

        this->updateBoundingRect();

    }

    void LayoutCell::addToNodeGraph(Instance * inst){

        if(inst == NULL) return;

        foreach(Port * p, inst->ports()){
            if(p == NULL) continue;

            if(nodeGraph_.contains(p->name())){
                nodeGraph_[p->name()]->append(p);
            }else{
                Graph *g = new Graph();
                g->name = p->name();
                g->append(p);
                nodeGraph_[p->name()] = g;
            }
        }

    }

    void LayoutCell::route(){

        foreach(Rect *r, routes_){
            if(r->isRoute()){
                Route * route = (Route *) r;
                route->route();
            }
        }



    }

    void LayoutCell::paint(){
        if(!noPowerRoute_){
            this->routePower();
        }

    }

    QList<Rect *> LayoutCell::findRectanglesByNode(QString node,  QString filterChild, QString matchInstance)
    {
        QList<Rect *> rects;
        foreach(Rect * r, this->children()){
            if(!r->isInstance()) continue;
            Instance * i = (Instance *)r;
            if(i == NULL){continue;}

            if(matchInstance != "" && !i->name().contains(QRegularExpression(matchInstance))){continue;}
            QList<Rect* > childRects = i->findRectanglesByNode(node, filterChild);
            foreach(Rect *r, childRects){
                rects.append(r);
            }

        }
        return rects;

    }

    void LayoutCell::addPowerRoute(QString net,QString excludeInstances)
    {
        QList<Rect*> foundrects  = this->findRectanglesByNode(net,"^(B|G)$", "");

        QList<Rect*> rects;
        foreach(Rect * r, foundrects){
            Rect * parent  = r->parent();
            if(parent && parent->isCell() ){
                Cell *c = (Cell*) parent;
                QString name = c->name();
                if(excludeInstances == "" || !name.contains(QRegularExpression(excludeInstances))){
                    rects.append(r);
                }
            }else{
                rects.append(r);
            }

        }

        if(rects.length() > 0){
            QList<Rect*>  cuts = Cut::getCutsForRects("M4",rects,2,1);
            Rect * rp = NULL;
            if(cuts.count() > 0){
                Rect r=  Cell::calcBoundingRect(cuts);
                r.setTop(this->top());
                r.setBottom(this->bottom());
                r.setLayer("M4");
                this->add(cuts);

                rp = new Rect(r);
            }else{
                Rect r=  Cell::calcBoundingRect(rects);
                r.setTop(this->top());
                r.setBottom(this->bottom());
                r.setLayer("M4");
                rp = new Rect(r);
            }
            if(rp){
                this->add(rp);
                if(ports_.contains(net)){
                    Port * p = ports_[net];
                    p->set(rp);
                }
            }
        }
    }

    void LayoutCell::fromJson(QJsonObject o){
        Cell::fromJson(o);
        QJsonArray car = o["children"].toArray();
        foreach(QJsonValue child,car){
            QJsonObject co = child.toObject();
            QString cl = co["class"].toString();
            if(cl == "Rect"){
                Rect * r = new Rect();
                r->fromJson(co);
                this->add(r);
            }else if(cl == "Port"){
                Port * p = new Port();
                p->fromJson(co);
                this->add(p);
            }else if(cl == "Instance"){
                Instance * i = new Instance();
                i->fromJson(co);
                this->add(i);
            }else if(cl == "Cell" || cl== "cIcCore::Route"){
                LayoutCell * l = new LayoutCell();
                l->fromJson(co);
                this->add(l);
            }else{
                qDebug() << "Error: Unknown class " << cl ;
            }
        }
        this->updateBoundingRect();

    }


    void LayoutCell::routePower(){
        this->addPowerRoute("AVDD","NCH");
        this->addPowerRoute("AVSS","PCH");
    }

    void LayoutCell::addAllPorts(){
        QStringList nodes = _subckt->nodes();
        QString filterChild = "^B$";
        QString filterInstance = "";
        foreach(QString node,nodes){
            if(ports_.contains(node)) continue;
            QList<Rect*> rects = this->findRectanglesByNode(node,filterChild,filterInstance);
            if(rects.count() > 0){
                Port * p = new Port(node);
                p->set(rects[0]);
                this->add(p);
            }
        }

    }

}
