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
        spiceObject_ = NULL;
        _subckt = NULL;
    }

    Cell::Cell(const Cell&){
        spiceObject_ = NULL;
        _subckt = NULL;
    }

    Cell::~Cell() {

    }

    void Cell::mirrorCenterX(){
        this->mirrorX(this->centerX());
    }

    void Cell::paint(){

    }
    void Cell::route(){}
    void Cell::place(){}

    QList<Rect*> Cell::findRectanglesByRegex(QString regex,QString layer){

        //If the regex contains : then search child ports, if it does not, then search local ports
        QStringList re_s = regex.split(",",QString::SkipEmptyParts);
        QList<Rect*> rects;
        foreach(QString s,re_s){
            if(s.contains(":")){
                QStringList str_tok = s.split(":",QString::SkipEmptyParts);
                QString instname = str_tok[0];
                str_tok.pop_front();
                QString path = str_tok.join(":");

                //Find and match instance
                QRegularExpression re_inst(instname);
                foreach(Rect * child, children()){
                    if(child->isInstance()){
                        Cell * inst = (Cell*) child;
                        QRegularExpressionMatch m_inst = re_inst.match(inst->instanceName_);
                        if(m_inst.hasMatch()){
                            QList<Rect*> child_rects = inst->findRectanglesByRegex(path,layer);
                            foreach(Rect * r, child_rects){
                                rects.append((r));
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
    void Cell::addAllPorts(){}
    QList<Port*> Cell::ports(){
        return  ports_.values();
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



    //-------------------------------------------------------------
    // Children handling
    //-------------------------------------------------------------
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
            if(child->isPort()){
                Port* p = (Port*) child;
                ports_[p->name()] = p;
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

    void Cell::mirrorX(int ax) {
        this->setLeft(2*ax - this->left());
        this->setRight(2*ax - this->right());

        if (this->right() < this->left()) {
            int tmp = this->left();
            this->setLeft(this->right());
            this->setRight(tmp);
        }

        foreach(Rect * child, _children) {
            child->mirrorX(ax);
        }
        this->updateBoundingRect();
        emit updated();
    }

    void Cell::mirrorY(int ay) {

        this->setTop(2 *  ay - this->top());
        this->setBottom(2 *  ay - this->bottom());

        if (this->bottom() < this->top()) {
            int tmp = this->top();
            this->setTop(this->bottom());
            this->setTop(tmp);
        }

        foreach(Rect* child, _children) {
            child->mirrorY(ay);
        }
        this->updateBoundingRect();
        emit updated();
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
        int ypos = this->top() - ( yc1 - ay );

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
        int x1  = std::numeric_limits<int>::max();
        int y1  = std::numeric_limits<int>::max();
        int x2  = -std::numeric_limits<int>::max();
        int y2  =  -std::numeric_limits<int>::max();

        if(children.count() == 0){
            x1 = y1 = x2 = y2 = 0;
        }


        foreach(Rect* cr, children) {

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
        return this->calcBoundingRect(this->children());
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
        Rect * bottomLeft;
        foreach(Rect * r, this->_children){
            if(r->x1() < xmin && r->y1() < ymin){
                if(bottomLeft){
                    delete(bottomLeft);
                }
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
        Rect * topLeft;
        foreach(Rect * r, this->_children){
            if(r->x1() < xmin && r->y2() > ymax){
                if(topLeft){
                    delete(topLeft);
                }
                xmin = r->x1();
                ymax = r->y2();
                topLeft = new Rect(r);
            }
        }

        return topLeft;
    }
}
