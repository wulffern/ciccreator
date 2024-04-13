//====================================================================
//        Copyright (c) 2015 Carsten Wulff Software, Norway
// ===================================================================
// Created       : wulff at 2015-1-26
// ===================================================================
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//====================================================================

#include "core/cell.h"

namespace cIcCore{

    QMap<QString,Cell*> Cell::_allcells;

    Cell::Cell(): Rect(){
        _subckt = NULL;
        boundaryIgnoreRouting_ = false;
        _physicalOnly = false;
        abstract_ = false;
        lib_cell_ = false;
        lib_path_ = "";
        cell_used_ = false;
        _has_pr = false;

    }

    Cell::Cell(const Cell&){
        _subckt = NULL;
        boundaryIgnoreRouting_ = false;
        _physicalOnly = false;
        lib_cell_ = false;
        lib_path_ = "";
        cell_used_ = false;
        _has_pr = false;
    }

    Cell::~Cell() {

    }

    void Cell::meta(QJsonObject obj){
        meta_ = obj;
    }



    void Cell::boundaryIgnoreRouting(QJsonValue obj)
    {
        int bir = obj.toInt();
        if(bir==1) setBoundaryIgnoreRouting(true);
        else setBoundaryIgnoreRouting(false);
    }

    void Cell::setBoundaryIgnoreRouting(bool bir)
    {
        boundaryIgnoreRouting_ = bir;

    }
    bool Cell::boundaryIgnoreRouting()
    {
        return boundaryIgnoreRouting_;

    }

    void Cell::abstract(QJsonValue obj){
        auto v = obj.toInt();
        if(v == 1)
            abstract_ = true;
        else
            abstract_ = false;
    }



    void Cell::mirrorCenterX(){
        this->mirrorX(this->centerY());
    }

    void Cell::mirrorCenterY(){
        this->mirrorY(this->centerX());
    }

    void Cell::paint(){

    }
    void Cell::route(){}
    void Cell::place(){}

    QList<Rect*> Cell::findAllRectangles(QString regex,QString layer){
        QList<Rect*> rects = findRectanglesByRegex(regex, layer);
        findRectangles(rects,regex,layer);
        return rects;
    }

    void Cell::findRectangles(QList<Rect*> &rects, QString name, QString layer){

        //Search instance ports
        foreach(Rect* child, children()){
            if(child == NULL) continue;
            if(child->isInstance()){
                Cell * inst = static_cast<Cell *>(child);
                if(inst == NULL) continue;
                foreach(Port *p, inst->ports()){
                    if(p->name() == name){
                        Rect *r = p->get(layer);
                        if(r==NULL)
                            r = p->get();
                        if(r){
                            rects.append(r);
                        }
                    }
                }
            }
        }

        //Search custom ports
        foreach(QString rect_name,named_rects_.keys()){
            if(rect_name == name){
                rects.append(named_rects_[rect_name]);
            }
        }
    }

    QList<Rect*> Cell::findRectanglesByRegex(QString regex,QString layer){

        //If the regex contains : then search child ports, if it does not, then search local ports
        QStringList re_s = regex.split(",",Qt::SkipEmptyParts);
        QList<Rect*> rects;
        foreach(QString s,re_s){
            if(s.contains(":")){
                QStringList str_tok = s.split(":",Qt::SkipEmptyParts);
                QString instname = str_tok[0];
                str_tok.pop_front();
                QString path = str_tok.join(":");

                //Find and match instance
                QRegularExpression re_inst(instname);
                foreach(Rect * child, children()){
                    if(child->isInstance()){
                        Cell * inst = static_cast<Cell *>(child);
                        QRegularExpressionMatch m_inst = re_inst.match(inst->instanceName_);
                        if(m_inst.hasMatch()){
                            QList<Rect*> child_rects = inst->findRectanglesByRegex(path,layer);
                            foreach(Rect * r, child_rects){
                                rects.append(r);
                            }
                        }
                    }
                }
            }else{



                //Search ports
                foreach(Port * p, ports_){
                    bool hasMatch = false;
                    if(0){
                        //Match name on top level
                        QRegularExpression re(s);
                        QRegularExpressionMatch m_port = re.match(p->name());
                        if(m_port.hasMatch()){
                            hasMatch = true;
                        }
                    }else{
                        if(p->name() == s){
                            hasMatch = true;
                        }
                    }


                    if(hasMatch){
                        Rect * r= p->get(layer);
                        if(!r){
                            r = p->get();
//                          r->setLayer(layer);
                        }

                        if(r){
                            rects.append(r);
                        }
                    }

                }
            }

        }

        return rects;
    }

    //-------------------------------------------------------------
    // Port functions
    //-------------------------------------------------------------
    void Cell::addPort(QString name, Rect* r)
    {
        Port* p = new Port(name);
        p->set(r);
        p->spicePort = this->isASpicePort(name);
        this->add(p);
    }

    void Cell::addAllPorts(){}
    QList<Port*> Cell::ports(){
        return  ports_.values();
    }

    QMap<QString,QList<Port*>> Cell::allports(){
        return allports_;
    }

    QList<QString> Cell::allPortNames(){
        return allPortNames_;
    }

    Port * Cell::getPort(QString name){

        Port * p = NULL;
        if(ports_.contains(name)){
            p = ports_[name];
        }

        return p;
    }

    Rect* Cell::getRect(QString layer){
        foreach (Rect* child, _children){
            QString lay = child->layer();
            if (lay.compare(layer) == 0) {
                return child;
            }
        }
        return 0    ;
    }

    bool Cell::isASpicePort(QString name)
    {
        if(!_subckt){
            return true;
        }

        if(_subckt->nodes().contains(name)){
            return true;
        }else{
            return false;

        }
    }




    Port * Cell::updatePort(QString name,Rect* r)
    {

        Port* p_ptr = 0;
        if(ports_.contains(name)){
            p_ptr = ports_[name];
            p_ptr->spicePort = this->isASpicePort(name);
            p_ptr->set(r);
        }else{
            if(_subckt && _subckt->nodes().contains(name)){
                p_ptr = new Port(name);
                p_ptr->spicePort = this->isASpicePort(name);
                p_ptr->set(r);
                this->add(p_ptr);
            }

        }
        return p_ptr;
    }




    //-------------------------------------------------------------
    // Children handling
    //-------------------------------------------------------------


    QList<Rect*> Cell::getChildren(QString type)
    {


        if(children_by_type.contains(type)){
            return children_by_type[type];
        }else{

            qDebug() << "Error: Could not find child type " << type << " in " << children_by_type.keys();
            QList<Rect*> r;

            return r;

        }


    }


    void Cell::add(QList<Rect*> children){
        foreach(Rect * r, children){
            this->add(r);
        }
    }

    void Cell::add(Rect* child){

        if ( child == 0){
            qDebug() << "Tried to add null-pointer to " << this->name();
            return;
        }

        if (child && !_children.contains(child)) {

            QString type = child->metaObject()->className();

            if(!children_by_type.contains(type)){
                QList<Rect*> a;
                children_by_type[type] = a;
            }

            children_by_type[type].append(child);

            if(child->isPort()){
                Port* p = static_cast<Port *>(child);
                ports_[p->name()] = p;
                allPortNames_.append(p->name());
                allports_[p->name()].append(p);
            }

            if(child->isRoute()){

                routes_.append(child);
            }

            child->parent(this);

            this->_children.append(child);

            connect(child,SIGNAL(updated()),this, SLOT(updateBoundingRect()));

        }


        this->updateBoundingRect();

    }

    void Cell::translate(int dx, int dy) {
        Rect::translate(dx,dy);
        foreach(Rect* child, _children) {
            child->translate(dx,dy);
        }
        this->updateBoundingRect();
        emit updated();
    }

    void Cell::mirrorY(int ax) {
        Rect::mirrorY(ax);

        foreach(Rect * child, _children) {
            child->mirrorY(ax);
        }

        foreach(Port* p, ports_){
            if(!p) continue;
            p->mirrorY(ax);
        }

        this->updateBoundingRect();
        emit updated();
    }

    void Cell::mirrorX(int ay) {

        Rect::mirrorX(ay);


        foreach(Rect* child, _children) {
            child->mirrorX(ay);
        }

        foreach(Port* p, ports_){
            if(!p) continue;
            p->mirrorX(ay);
        }

        this->updateBoundingRect();
        emit updated();
    }

    void Cell::moveTo(QJsonArray obj)
    {

        if(obj.size() < 2){
            qDebug() << "Error: moveTo must contain at least two elements:[int,int]\n";
            return;
        }

        int x = obj[0].toInt();
        int y = obj[1].toInt();
        this->moveTo(x,y);


    }


    void Cell::moveTo(int ax, int ay) {
        int x1 = this->x1();
        int y1 = this->y1();
        Rect::moveTo(ax,ay);
        foreach(Rect* child, _children) {
            child->translate( ax - x1, ay -  y1);
        }
        emit updated();

    }

    void Cell::moveCenter(int ax, int ay) {
        this->updateBoundingRect();

        int xc1 = this->centerX();
        int yc1 = this->centerY();

        int xpos = this->left() - ( xc1 - ax );
        int ypos = this->bottom() - ( yc1 - ay );

        this->moveTo(xpos,ypos);
    }


    //-------------------------------------------------------------
    // Bounding rectangle functions
    //-------------------------------------------------------------
    void Cell::updateBoundingRect(){

        Rect r = this->calcBoundingRect();
        this->setRect(r);
    }

    Rect Cell::calcBoundingRect(QList<Rect*> children){
        return Cell::calcBoundingRect(children,false);
    }


    Rect Cell::calcBoundingRect(QList<Rect*> children,bool ignoreBoundaryRouting){



        int x1  = std::numeric_limits<int>::max();
        int y1  = std::numeric_limits<int>::max();
        int x2  = -std::numeric_limits<int>::max();
        int y2  =  -std::numeric_limits<int>::max();

        if(children.count() == 0){
            x1 = y1 = x2 = y2 = 0;
        }
        foreach(Rect* cr, children) {

            if(ignoreBoundaryRouting && (!cr->isInstance() || cr->isCut())) continue;



            int cx1 = cr->x1();
            int cx2 = cr->x2();
            int cy1 = cr->y1();
            int cy2 = cr->y2();

            if (cx1 < x1) {
                x1 = cx1;
            }

            if (cy1 < y1) {
                y1 = cy1;
            }
            if (cx2 > x2) {
                x2 = cx2;
            }
            if (cy2 > y2) {
                y2 = cy2;

            }

        }
        Rect r;
        r.setPoint1(x1,y1);
        r.setPoint2(x2,y2);

        return r;


    }

    Rect Cell::calcBoundingRect(){
        return this->calcBoundingRect(this->children(),this->boundaryIgnoreRouting());
    }

    QString Cell::toString(){
        QString str;
        str.append(" ");
        str.append(this->name());
        str.append(" ");
        str.append(Rect::toString());
        str.append("\n {\n");
        QString strpar = this->metaObject()->className();
        foreach(Rect* child, _children){
            str.append("  ");
            str.append(child->toString());
            str.append("\n");
        }
        str.append(" }");
        return str;

    }

    Rect* Cell::getBottomLeftRect(){
        int xmin = std::numeric_limits<int>::max();
        int ymin = std::numeric_limits<int>::max();
        Rect * bottomLeft = NULL;
        foreach(Rect * r, this->_children){
            if(r->x1() < xmin && r->y1() < ymin){
                xmin = r->x1();
                ymin = r->y1();
                bottomLeft = new Rect(r);
            }
        }

        return bottomLeft;
    }

    Rect* Cell::getTopLeftRect(){
        int xmin = std::numeric_limits<int>::max();
        int ymax = -std::numeric_limits<int>::max();
        Rect * topLeft = NULL;
        foreach(Rect * r, this->_children){
            if(r->x1() < xmin && r->y2() > ymax){
                xmin = r->x1();
                ymax = r->y2();
                topLeft = new Rect(r);
            }
        }

        return topLeft;
    }

    void Cell::fromJson(QJsonObject o){
        Rect::fromJson(o);
        this->setName(o["name"].toString());

        if(!this->isInstance()){
            _has_pr = o["has_pr"].toBool();
            this->boundaryIgnoreRouting_ = o["boundaryIgnoreRouting"].toBool();
            //this->instanceName_ = o["instanceName"].toString();
            this->abstract_ = o["abstract"].toBool();
            this->_physicalOnly = o["physicalOnly"].toBool();
        }

        if(!o.contains("children")) return;
        QJsonArray car = o["children"].toArray();

        if(o.contains("libpath")){
            QString tmp = o["libpath"].toString();
            if(tmp != ""){
                lib_path_ = tmp;
            }
        }

        if(o.contains("meta")){
            meta_ = o["meta"].toObject();
        }

        if(o.contains("ckt")){
            cIcSpice::Subckt * subckt = new cIcSpice::Subckt();
            subckt->setLibPath(lib_path_);
            subckt->fromJson(o["ckt"].toObject());
            _subckt = subckt;
        }

        foreach(QJsonValue child,car){
            QJsonObject co = child.toObject();

            auto c = cellFromJson(co);
            if(c == 0){
                if( !(co["name"].toString().contains("cut_"))){

                    qDebug() << this << "Unknown cell " << co["name"] << co["class"];
                }
            }else{
                this->add(c);
            }


        }
    }

    Rect * Cell::cellFromJson(QJsonObject co){

        QString cl = co["class"].toString();

        if(cl == "Rect"){
            Rect * r = new Rect();
            r->fromJson(co);
            return r;
        }else if(cl == "Text"){
            Text * t = new Text();
            t->fromJson(co);
            return static_cast<Rect *>(t);
        }else if(cl == "Port"){
            Port * p = new Port();
            p->fromJson(co);
            return static_cast<Rect *>(p);
        }else if(cl == "Cell" || cl == "cIcCore::Cell"){
            Cell * l = new Cell();
            l->fromJson(co);
            return static_cast<Rect *>(l);
        }
        return 0;
    }


    QJsonObject Cell::toJson(){
        QJsonObject o = Rect::toJson();
        o["class"] =  this->metaObject()->className();
        if(this->isText() or this->isPort() or this->isRoute()  ){
            o["name"] = this->name();
        }else{
            o["name"] = this->prefix_ + this->name();
        }

        if(!this->isInstance()){
            o["has_pr"] = this->_has_pr;
            o["abstract"] = this->abstract_;
            o["meta"] = meta_;
            o["physicalOnly"] = this->_physicalOnly;
            o["libcell"] = this->lib_cell_;
            o["libpath"] = this->lib_path_;
            o["cellused"] = this->cell_used_;
            o["boundaryIgnoreRouting"] = this->boundaryIgnoreRouting_;
        }


        cIcSpice::Subckt * ckt = this->subckt();

        if(ckt){
            ckt->setPrefix(this->prefix_);
            QJsonObject ockt = ckt->toJson();
            ockt["class"] = this->metaObject()->className();
            o["ckt"] = ockt;
        }

        QJsonArray ar;

        QMap<QString,bool> _printedRects;

        foreach(Rect * r, children()){

            //If it's a rectangle, then don't print duplicates
            if(r->isRect()){
                QString rid = r->toString();
                if(_printedRects.contains(rid)){
                    //Skip duplicate rectangles
                }else{
                    _printedRects[rid] = true;
                    QJsonObject child = r->toJson();
                    ar.append(child);
                }
            }else{
                r->setPrefix(this->prefix_);
                QJsonObject child = r->toJson();
                ar.append(child);
            }


        }
        o["children"] = ar;
        return o;
    }

    bool Cell::isEmpty(Cell * c){
        if(c->name() == ""){
            //Cell * r = (Cell * ) c->parent();
            return true;
        }
        return false;
    }

    void Cell::addEnclosingLayers(QList<QString> layers)
    {


        Rect* r = this->getCopy();
        foreach(QString lay, layers){

            int enc = 0;
            if(rules->hasRule(lay,this->layer() + "enclosure")){
                enc = this->rules->get(lay,this->layer() + "enclosure");
            }else{
                enc = this->rules->get(lay,"enclosure");
            }

            r->adjust(enc);
            Rect * r_enc = new Rect(r);
            r_enc->setLayer(lay);
            this->add(r_enc);
        }

    }

    void Cell::updateUsedChildren(){
        foreach(Rect * r, this->_children){
            if(r->isCell()){
                Cell * c = static_cast<Cell*>(r);
                c->setUsed(this->cell_used_);
                c->updateUsedChildren();
            }
        }
    }


}
