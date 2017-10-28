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
        boundaryIgnoreRouting_ = true;
        alternateGroup_ = false;
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

    void LayoutCell::alternateGroup(QJsonValue obj){
        alternateGroup_ = true;
    }

    void LayoutCell::noPowerRoute(QJsonValue obj){
        noPowerRoute();
    }

    void LayoutCell::noPowerRoute(){
        noPowerRoute_ = true;
    }

    QList<Graph*> LayoutCell::getNodeGraphs(QString regex)
    {
        QList<Graph*> graphs;

        foreach(QString node, nodeGraphList()){
            if(!node.contains(QRegularExpression(regex))) continue;
            graphs.append(nodeGraph_[node]);
        }
        return graphs;
    }
  


    void LayoutCell::addDirectedRoute(QJsonArray obj){

        if(obj.size() < 3){
            qDebug() << "Error: addDirectedRoute must contain at least three elements\n";
            return;
        }

        QString layer = obj[0].toString();
        QString net = obj[1].toString();
        QString route = obj[2].toString();

        QString options = "";
        if(obj.size() > 3){
            options = obj[3].toString();
        }

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

        if(start.count() > 0 && stop.count() > 0){


            Route * r = new Route(net,layer,start,stop,options,routeType);
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
        this->addConnectivityRoute(layer,regex,routeType,options,cuts,excludeInstances,includeInstances);

    }


    void LayoutCell::addConnectivityRoute(QString layer,QString regex, QString routeType,QString options,QString cuts,QString excludeInstances, QString includeInstances)
    {

        foreach(QString node, nodeGraphList()){
            if(!node.contains(QRegularExpression(regex))) continue;

            Graph * g = nodeGraph_[node];
            QList<Rect*>  rects = g->getRectangles(excludeInstances,includeInstances,layer);

            if(rects.count() == 0){
                qDebug() << "Error: Could not find rectangles on " << node << regex << rects.count() << "\n";
            }
            if(rects.count() > 0){
                QList<Rect*> empty;
                Route * r = new Route(node,layer,empty,rects,options,routeType);
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
                this->updatePort(port,r);
            }
        }
    }

    void LayoutCell::addVia(QJsonArray obj)
    {
        if(obj.size() < 4){
            qDebug() << "Error: addVia must contain at least four elements\n";
            return;
        }

        QString startlayer = obj[0].toString();
        QString stoplayer = obj[1].toString();
        QString path = obj[2].toString();
        int hcuts = obj[3].toInt();
        int vcuts =  (obj.size() > 4) ? obj[4].toInt() : 1;

        double offset = (obj.size() > 5) ? obj[5].toDouble() : 0;
        QString name = (obj.size() > 6) ? obj[6].toString() : "";
        double yoffset = (obj.size() > 7) ? obj[7].toDouble() : 0;


        QList<Rect*> rects = this->findAllRectangles(path,startlayer);

        
        foreach(Rect* r, rects){
            if(r == 0) continue;
            Instance * inst= Cut::getInstance(startlayer,stoplayer,hcuts,vcuts);
            inst->moveTo(r->x1() + offset*this->rules->get("ROUTE","horizontalgrid"),r->y1() + this->rules->get("ROUTE","verticalgrid")*yoffset);

            if(name != ""){
                Rect * p = inst->getRect(stoplayer);

                if(p != 0) named_rects_[name] = p;
                else qDebug() << "Error: Unknown rect " << name;

            }

            this->add(inst);

        }

    }

    Instance* LayoutCell::getInstanceFromInstanceName(QString instanceName)
    {
        QList<Rect*> instances = this->getChildren("cIcCore::Instance");
        foreach(Rect* r, instances){
            Instance* i = (Instance*) r;
            if(i->instanceName() == instanceName){
                return i;
            }
        }
        return 0;
    }
    


    void LayoutCell::addConnectivityVia(QJsonArray obj)
    {
        if(obj.size() < 4){
            qDebug() << "Error: addVia must contain at least four elements\n";
            return;
        }

        QString startlayer = obj[0].toString();
        QString stoplayer = obj[1].toString();
        QString name = obj[2].toString();
        double gridMultiplier = obj[3].toDouble();
        int vcuts = obj[4].toInt();
        int hcuts =  (obj.size() > 5) ? obj[5].toInt() : 1;
        double xoffset = (obj.size() > 6) ? obj[6].toDouble() : 0;
        double yoffset = (obj.size() > 7) ? obj[7].toDouble() : 0;
        QString netname = (obj.size() > 8) ? obj[8].toString() : "";

        int grid = this->rules->get("ROUTE","horizontalgrid");
        grid = int(grid*gridMultiplier/10)*10;

        
        
        QList<Rect*> rects = this->findAllRectangles(name,startlayer);
        int xgrid = 0;
        bool setPort = true;
        foreach(Rect* r, rects){
            if(r == 0) continue;

            Instance * inst= Cut::getInstance(startlayer,stoplayer,hcuts,vcuts);

            xgrid = (grid != 0) ? grid: inst->width();

            inst->moveTo(r->x1() + xoffset*grid,r->centerY() + yoffset*inst->height());

            if(netname != ""){
                Rect * p = inst->getRect(stoplayer);
                if(p != 0) named_rects_[netname] = p;
                else qDebug() << "Error: Unknown rect " << netname;
            }
            this->add(inst);
            if(setPort){
                Port* p;
                Rect* r = inst->getRect(stoplayer);
                if(ports_.contains(name)){
                    p = ports_[name];
                    p->set(r);
                }else{
                    p = new Port(name);
                    p->set(r);
                    this->add(p);
                }

                setPort = false;

            }
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
            inst->moveTo(r->x2() + xoffset*inst->width(),r->centerY() + yoffset*inst->height());

            Rect * rstop = inst->getRect(stoplayer);

            if(name != ""){
                if(r != 0) named_rects_[name] = rstop;
                else qDebug() << "Error: Unknown rect " << name;

            }
            if(ports_.contains(port)){
                Port *p = ports_[port];
                p->set(rstop);
            }else{
                Port * p = new Port(port);
                p->set(rstop);
                this->add(p);
            }
            this->add(inst);


        }
    }

    void LayoutCell::addVerticalRect(QJsonArray obj)
    {
        if(obj.size() < 2){
            qDebug() << "Error: addVerticalRects must contain at least 2 elements\n";
            return;
        }

        QString layer = obj[0].toString();
        QString path = obj[1].toString();
        int cuts = (obj.size() > 2) ? obj[2].toInt(): 0;

        QList<Rect*> rects = this->findAllRectangles(path,layer);

        
        foreach(Rect* r, rects){
            int width = r->width();
            if(cuts > 0){
                Instance * inst= Cut::getInstance("M1",layer,cuts,1);
                //TODO: Not sure why it can't be the cut size?
//                if((r->width() - inst->width()) < this->rules->get("M1","width")/2.0){
                    width = inst->width();
                    // }
                

            }
            Rect * rn = new Rect(layer,r->x1(), this->y1(),width,this->height());
            this->add(rn);


        }
    }

    void LayoutCell::addHorizontalRect(QJsonArray obj)
    {
        if(obj.size() < 2){
            qDebug() << "Error: addHorizontalRects must contain at least 2 elements\n";
            return;
        }

        QString layer = obj[0].toString();
        QString path = obj[1].toString();

        double xsize = (obj.size() > 2) ? obj[2].toDouble(): 1;
        double ysize = (obj.size() > 3) ? obj[3].toDouble(): 1;

        int xspace = this->rules->get("ROUTE","horizontalgrid")*xsize;
        int yspace = this->rules->get("ROUTE","verticalgrid")*ysize;
        
        QList<Rect*> rects = this->findAllRectangles(path,layer);

        foreach(Rect* r, rects){

            int x;
            int y;
            
            int width;
            
            if(xspace > 0){
                x = r->x1();
                y = r->y1() + yspace;
                width = xspace;
            }else{
                x = r->x1()  + xspace;
                y = r->y1() + yspace;
                width = -xspace;
            }
            
            Rect * rn = new Rect(layer, x, y,width,r->height());
            this->add(rn);
        }
    }


    void LayoutCell::addRectangle(QJsonArray obj)
    {
        if(obj.size() < 4){
            qDebug() << "Error: addRectangle must contain at least 4 element\n";
            return;
        }
        QString layer = obj[0].toString();
        int x1 = obj[1].toInt();
        int y1 = obj[2].toInt();
        int width = obj[3].toInt();

        int height = obj[4].toInt();
        QString angle = (obj.size() > 4) ? obj[5].toString(): "";
        this->addRectangle(layer,x1,y1,width,height,angle);

    }

    void LayoutCell::addRectangle(QString layer, int x1, int y1, int width, int height,QString angle)
    {
        Rect* r = new Rect(layer,x1,y1,width,height);
        if(angle == "R90"){
            r->rotate(90);
        }else if(angle == "R180"){
            r->rotate(90);
            r->rotate(90);
        }else if(angle == "R270"){
            r->rotate(90);
            r->rotate(90);
            r->rotate(90);
        }

        this->add(r);

    }



    void LayoutCell::addPowerRing(QJsonArray obj)
    {
        if(obj.size() < 2){
            qDebug() << "Error: addPowerRing must contain at least 2 element\n";
            return;
        }
        QString layer = obj[0].toString();
        QString name = obj[1].toString();
        QString location = (obj.size() > 2) ? obj[2].toString() : "rtbl";
        int widthmult = (obj.size() > 3) ? obj[3].toInt(): 1;

        this->addPowerRing(layer,name,location,widthmult);
    }

    void LayoutCell::addPowerRing(QString layer, QString name, QString location, int widthmult)
    {
        Instance* c = Cut::getInstance("M3","M4",2,2);
        int metalwidth = c->height()*widthmult;
        int xgrid = this->rules->get("ROUTE","horizontalgrid")*10;
        int ygrid = this->rules->get("ROUTE","horizontalgrid")*10;

        RouteRing* rr = new RouteRing(layer,name,this->getCopy(),location,ygrid,xgrid,metalwidth);

        //Add to named rects
        if(rr != 0){
            QString rail = "power_" + name;
            named_rects_[rail] = rr;
            named_rects_[QString("RAIL_BOTTOM_" + name)] = rr->getPointer("bottom");
            named_rects_[QString("RAIL_TOP_" + name)] =  rr->getPointer("top");
            named_rects_[QString("RAIL_LEFT_" + name)] =  rr->getPointer("left");
            named_rects_[QString("RAIL_RIGHT_" + name)] =  rr->getPointer("right");
        }

        this->updatePort(name,rr->getDefault());

        this->add(rr);

    }

    void LayoutCell::addRouteHorizontalRect(QJsonArray obj)
    {
        if(obj.size() < 3){
            qDebug() << "Error: addRouteHorizontalRects must contain at least 3 element\n";
            return;
        }
        QString layer = obj[0].toString();
        QString rectpath = obj[1].toString();
        int x = (obj.size() > 2) ? obj[2].toInt(): 1;
        QString name = (obj.size() > 3) ? obj[3].toString(): "";

        this->addRouteHorizontalRect(layer,rectpath,x,name);
    }

    void LayoutCell::addGuard(QJsonArray obj)
    {
        if(obj.size() < 3){
            qDebug() << "Error: addRouteHorizontalRects must contain at least 3 element\n";
            return;
        }
        QString layer = obj[0].toString();
        double enc = obj[1].toDouble();
        QList<QString> layers;
        QJsonArray encl = obj[2].toArray();

        foreach(QJsonValue enc, encl){
            layers.append(enc.toString());
        }


        this->addGuard(layer,enc,layers);
    }

    void LayoutCell::addGuard(QString port, double gridMultiplier, QList<QString> layers)
    {
        Rect* r = this->getCopy();
        int yenc = this->rules->get("ROUTE","verticalgrid")*gridMultiplier;
        int xenc = this->rules->get("ROUTE","verticalgrid")*gridMultiplier;
        
        r->adjust(-xenc,-yenc,xenc,yenc);

        Guard* g = new Guard(r,layers);
        this->add(g);
        Port * p;
        p = this->getPort(port);
        if(!p){
            p = new Port(port);
            this->add(p);
        }

        Rect* m1 = g->getRect("M1");
        
        if(p && m1){
                      p->set(m1);
        }


  

    }
    


    
    
    void LayoutCell::addRouteHorizontalRect(QString layer, QString rectpath, int x, QString name)
    {
        QList<Rect*> rects = this->findAllRectangles(rectpath,layer);
        auto xgrid = this->getRules()->get("ROUTE","horizontalgrid");
        auto mw = this->getRules()->get(layer,"width");
        foreach(Rect* r, rects) {
            auto p = new Rect(layer,r->x1(),r->y1(),xgrid*x,mw);

            if(name != ""){
                if(r != 0) named_rects_[name] = p;
                else qDebug() << "Error: Unknown rect " << name;
                
            }
            this->add(p);
    }


    }
    


    void LayoutCell::addRouteRing(QJsonArray obj)
    {
        if(obj.size() < 3){
            qDebug() << "Error: addRouteRing must contain at least 3 element\n";
            return;
        }
        QString layer = obj[0].toString();
        QString name = obj[1].toString();
        QString location = (obj.size() > 2) ? obj[2].toString(): "rtbl";
        int widthmult = (obj.size() > 3) ? obj[3].toInt(): 1;
        int spacemult = (obj.size() > 4) ? obj[4].toInt(): 2;

        this->addRouteRing(layer,name,location,widthmult,spacemult);
    }

    void LayoutCell::addRouteRing(QString layer, QString name, QString location, int widthmult, int spacemult)
    {
        this->addRouteRing(layer,name,location,widthmult,spacemult,true);
        
    }


    void LayoutCell::addRouteRing(QString layer, QString name, QString location, int widthmult, int spacemult,bool useGridForSpace)
    {
        int metalwidth = this->rules->get(layer,"width")*widthmult;
        int xgrid;
        int ygrid;
        if(useGridForSpace){
            xgrid = this->rules->get("ROUTE","horizontalgrid")*spacemult + metalwidth;
            ygrid = this->rules->get("ROUTE","horizontalgrid")*spacemult + metalwidth;
        }else{
            xgrid = this->rules->get(layer,"space")*spacemult + metalwidth;
            ygrid = this->rules->get(layer,"space")*spacemult + metalwidth;

        }


        QStringList names = expandBus(name);
        foreach(QString n,names){
            RouteRing* rr = new RouteRing(layer,n,this->getCopy(),location,ygrid,xgrid,metalwidth);
            this->updatePort(n,rr->getDefault());
            QString rail = "rail_" + n;
            if(rr != 0) named_rects_[rail] = rr;
            this->add(rr);
        }
    }


    void LayoutCell::addPowerConnection(QJsonArray obj)
    {
        if(obj.size() < 3){
            qDebug() << "Error: addRouteRing must contain at least 3 element\n";
            return;
        }
        QString name = obj[0].toString();
        QString includeInstances = obj[1].toString();
        QString location = obj[2].toString();

        this->addPowerConnection(name,includeInstances,location);
    }

    void LayoutCell::addPowerConnection(QString name, QString includeInstances, QString location)
    {
        if(!nodeGraph_.contains(name)) return;
        if(!named_rects_.contains("power_" + name)) return;

        Graph* g  = nodeGraph_[name];
        QList<Rect*>  rects = g->getRectangles("",includeInstances,"");
        RouteRing* routering = (RouteRing*) named_rects_["power_" + name];
        Rect* rrect = routering->get(location);
        foreach(Rect* r, rects){
            Instance* ct = Cut::getInstance(r->layer(),rrect->layer(),2,2);

            Rect* rr = r->getCopy();
            if (location == "top") {
                rr->setTop(rrect->y2());
                if(ct) ct->moveTo(rr->x1(),rrect->y1());
            } else if (location == "bottom") {
                rr->setBottom(rrect->y1());
                if(ct) ct->moveTo(rr->x1(),rrect->y1());
            } else if (location ==  "left") {
                rr->setLeft(rrect->x1());
                if(ct) ct->moveTo(rrect->x1(),rr->y1());
            } else if (location ==  "right") {
                rr->setRight(rrect->x2());
                if(ct) ct->moveTo(rrect->x1(),rr->y1());
            }
            routering->add(rr);
            routering->add(ct);
        }


    }


    void LayoutCell::addRouteConnection(QJsonArray obj)
    {
        if(obj.size() < 3){
            qDebug() << "Error: addRouteRing must contain at least 3 element\n";
            return;
        }
        QString path = obj[0].toString();
        QString includeInstances = obj[1].toString();
        QString location = obj[2].toString();
        QString layer = (obj.size() > 3) ? obj[3].toString(): "";
        QString options = (obj.size() > 4) ? obj[4].toString(): "";
        this->addRouteConnection(path,includeInstances,layer,location,options);
    }

    void LayoutCell::addRouteConnection(QString path, QString includeInstances, QString layer, QString location, QString options)
    {
        QString routeType = "-|--";

        if(location == "top"){
            routeType = "||";
            options += ",onTopB,fillvcut";
        }else if(location == "bottom"){
            routeType = "||";
            options += ",onTopT,fillvcut";
        }else if(location == "right"){
            routeType = "-";
            options += ",onTopL,fillhcut";
        }else if( location == "left"){
            options += ",onTopR,fillhcut";
            routeType = "-";
        }

        foreach(QString node, nodeGraphList()){
            if(!node.contains(QRegularExpression(path))) continue;
            if(!named_rects_.contains("rail_" + node)) continue;
            
            Graph * g = nodeGraph_[node];
            QList<Rect*>  rects = g->getRectangles("",includeInstances,layer);
            RouteRing* rr = (RouteRing*) named_rects_["rail_" + node];
            Rect* routering = rr->get(location);
            QList<Rect*> empty;
            foreach(Rect* r, rects){
                QList<Rect*> stop;
                stop.append(r);
                stop.append(routering);
                Route* ro = new Route(node,layer,empty,stop,options,routeType);
                routes_.append(ro);
                rr->add(ro);
            }

        }
    }

    void LayoutCell::trimRouteRing(QJsonArray obj)
    {

        if(obj.size() < 3){
            qDebug() << "Error: trimRouteRings must contain at least 3 element\n";
            return;
        }
        QString path = obj[0].toString();
        QString location = obj[1].toString();
        QString whichEndToTrim = obj[2].toString();
        this->trimRouteRing(path,location,whichEndToTrim);
    }
    void LayoutCell::trimRouteRing(QString path, QString location,QString whichEndToTrim)
    {
        QList<Rect*> rects = this->getChildren("cIcCore::RouteRing");
        foreach(Rect* r,rects){
            RouteRing* rr = (RouteRing*) r;
            if(rr->name().contains(QRegularExpression(path))){
                rr->trimRouteRing(location,whichEndToTrim);
            }
        }
    }






//------------------------------------------------------------------------------------------
// Internal functions
//------------------------------------------------------------------------------------------

    QStringList LayoutCell::expandBus(QString name){

        QStringList names;
        QRegularExpression re_bus("<(\\d+):(\\d+)>");
        QRegularExpressionMatch m_bus = re_bus.match(name);
        if(m_bus.hasMatch()){

            QString start = m_bus.captured(1);
            QString stop = m_bus.captured(2);
            int istart = start.toInt();
            int istop = stop.toInt();
            for(int i=istart;i>=istop;i=i-1){
                names.append(name.replace(QRegularExpression("<.*>"),QString("<%1>").arg(i)));
            }
        }else{
            names.append(name);
        }
        return names;
    }

    void LayoutCell::place(){

        QString prev_group = "";
        int prev_width = 0;
        int next_x = 0;
        int next_y = 0;
        int x = 0;
        int y = 0;
        bool mirror_y = false;
        foreach(cIcSpice::SubcktInstance * ckt_inst,_subckt->instances()){
            QString group = ckt_inst->groupName();
            if(prev_group.compare(group) != 0  && prev_group.compare("")  != 0){
                y = 0;
                x = next_x;
                prev_width = 0;
            }

            prev_group = group;


            int instance_x = x;
            int instance_y = y;
            

            if(ckt_inst->hasProperty("xoffset")){
                QString offset = ckt_inst->getPropertyString("xoffset");
                if(offset == "width")
                {
                    //Not implemented
                }else{
                    int xoffset = offset.toInt();
                    instance_x = instance_x + this->rules->get("ROUTE","horizontalgrid")*xoffset;
                }
            }

            if(ckt_inst->hasProperty("yoffset")){
                QString offset = ckt_inst->getPropertyString("yoffset");

                if(offset == "height"){
                    //Not implemented
                }else{
                    int yoffset = offset.toInt();
                    instance_y = instance_y + this->rules->get("ROUTE","verticalgrid")*yoffset;
                    
                    
                }
            }
            
            
            Instance* inst = this->addInstance(ckt_inst,instance_x,instance_y);


            if(ckt_inst->hasProperty("angle")){
                QString angle = ckt_inst->getPropertyString("angle");
                if(angle == "180"){
                    inst->setAngle("MY");
                }
            }

            
            if(alternateGroup_ && mirror_y){
                inst->setAngle("MY");
                mirror_y = false;
            }else  if(alternateGroup_ && !mirror_y){
                mirror_y = true;
            }
            

            next_x = inst->x2();
            next_y = inst->y2();

            x = inst->x1();
            
            
            if(useHalfHeight){
                y += (inst->y2() - instance_y)/2;
            }else{
                y = next_y;
            }
        }

        this->updateBoundingRect();

    }

    Instance* LayoutCell::addInstance(cIcSpice::SubcktInstance* ckt_inst, int x, int y)
    {

        //The chain of events is important here, ports get defined in the setSubckInstance
        Instance * inst = Instance::getInstance(ckt_inst->subcktName());
        inst->setSubcktInstance(ckt_inst);
        this->add(inst);
        Text * t = new Text(ckt_inst->name());
        t->moveTo( inst->width()/2,  inst->height()/2);
        inst->add(t);
        inst->moveTo(x,y);
        this->addToNodeGraph(inst);
        return inst;
    }

    void LayoutCell::addToNodeGraph(Instance * inst){

        if(inst == NULL) return;

        auto allp = inst->allports();
        auto keys = inst->allPortNames();
        
        foreach(QString s, keys){

            foreach(Port * p,allp[s]){
                if(p == NULL) continue;
                if(nodeGraph_.contains(p->name())){

                    nodeGraph_[p->name()]->append(p);
                    
                }else{
                    Graph *g = new Graph();
                    g->name = p->name();
                    g->append(p);
                    nodeGraphList_.append(p->name());
                    nodeGraph_[p->name()] = g;
                }
                                    
            }
        }

    }

    QList<QString> LayoutCell::nodeGraphList()
        {

//            qDebug() << nodeGraph_.keys();
//            qDebug() << nodeGraphList_;
//            qDebug() << "\n";
            
            
//            return nodeGraph_.keys();
            return nodeGraphList_;

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

        Cell::paint();
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
                bool skip = false;
                
                if(excludeInstances != "" && c->isInstance()){
                    Instance* i = (Instance*) c;
                    QString name = i->name();
                    QString instName = i->instanceName();
                    if(name.contains(QRegularExpression(excludeInstances))) skip = true;
                    if(instName.contains(QRegularExpression(excludeInstances))) skip = true;
                }
                
                if(!skip)
                    rects.append(r);
                
            }else{
                rects.append(r);
            }

        }

        //TODO: If there are multiple rectangles horizontally this
        //method makes a sheet, should really make it better
        if(rects.length() > 0){
            QList<Rect*>  cuts = Cut::getCutsForRects("M4",rects,2,1);
            Rect * rp = NULL;
            if(cuts.count() > 0){
                Rect r=  Cell::calcBoundingRect(cuts,false);
                r.setTop(this->top());
                r.setBottom(this->bottom());
                r.setLayer("M4");
                this->add(cuts);

                rp = new Rect(r);
            }else{
                Rect r=  Cell::calcBoundingRect(rects,false);
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
            }else if(cl == "Text"){
                Text * t = new Text();
                t->fromJson(co);
                this->add(t);
            }else if(cl == "Port"){
                Port * p = new Port();
                p->fromJson(co);
                this->add(p);
            }else if(cl == "Instance"){
                Instance * i = new Instance();
                i->fromJson(co);
                this->add(i);
            }else if(cl == "Cell" || cl== "cIcCore::Route" || cl == "cIcCore::RouteRing" || cl == "cIcCore::Guard" || cl == "cIcCore::Cell"){
                LayoutCell * l = new LayoutCell();
                l->fromJson(co);
                this->add(l);
            }else{
                qDebug() << "Error(laoyutcell.cpp): Unknown class " << cl ;
            }
        }
        this->updateBoundingRect();

    }


    void LayoutCell::routePower(){
        this->addPowerRoute("AVDD","NCH|DMY");
        this->addPowerRoute("AVSS","PCH|DMY");
    }

    void LayoutCell::addAllPorts(){
        if(!_subckt) return;
        QStringList nodes = _subckt->nodes();
        QString filterChild = "^B$";
        QString filterInstance = "";
        foreach(QString node,nodes){
            if(ports_.contains(node)) continue;
            QList<Rect*> rects = this->findRectanglesByNode(node+"$",filterChild,filterInstance);
            if(rects.count() > 0){
                Port * p = new Port(node);
                p->set(rects[0]);
                this->add(p);
            }
        }

    }

}
