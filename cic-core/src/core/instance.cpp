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
#include "core/instance.h"

namespace cIcCore{

    Instance::Instance():Cell()
    {
        _cell = 0;
        ckt_inst_ = 0;
        xcell= 0;
        ycell = 0;
        angle_ = "";
    }

    Instance::Instance(const Instance& inst): Cell(inst)
    {
        _cell = 0;
        ckt_inst_ = 0;
        xcell = 0;
        ycell = 0;
        angle_ = "";

    }


    Instance::~Instance()
    {
        delete(ckt_inst_);
        delete(_cell);

    }

    Rect* Instance::getRect(QString layer){
        Rect * r = 0;
        if(this->cell()){
            r = this->cell()->getRect(layer);
            if(r){
                r = r->getCopy();
                this->transform(r);
            }
        }
        return r;
    }

    void Instance::transform(Rect* r)
    {

        if(this->angle() == "R90"){
            r->rotate(90);
            r->translate(xcell,ycell);
        }else if(this->angle() == "MY"){
            r->mirrorY(0);
            r->translate(xcell,ycell);
            
        }else if(this->angle() == "MX"){
            r->mirrorX(0);
            r->translate(xcell,ycell);

        }
        r->translate(this->x1(),this->y1());
    }


    QList<Rect*> Instance::findRectanglesByRegex(QString regex,QString layer){
        QList<Rect*> rects;
        Cell * c = this->cell();
        if(c){
            QList<Rect*> child_rects = c->findRectanglesByRegex(regex,layer);
            foreach(Rect * r, child_rects){
                this->transform(r);
                rects.append(r);
            }
        }
        return rects;
    }

    QList<Rect *> Instance::findRectanglesByNode(QString node, QString filterChild)
    {
        QList<Rect *> rects;
        foreach(Rect * r,this->children()){

            if(! r->isPort()) continue;
            Port * p = static_cast<Port *>(r);
            if(! p->isInstancePort()) continue;
            InstancePort * pi = static_cast<InstancePort*>(p);
            if(pi== NULL) continue;

            if(pi->name().contains(QRegularExpression(node)) &&
               (filterChild == "" || !pi->childName().contains(QRegularExpression(filterChild)))){
                Rect * r = pi->get();
                if(r){
                    r->parent(this);
                    rects.append(r);
                }
            }

        }
        return rects;

    }

    void Instance::setSubcktInstance(cIcSpice::SubcktInstance *inst){

        ckt_inst_ = inst;

        instanceName_ = inst->name();

        ports_.clear();
        cIcSpice::Subckt* ckt = cIcSpice::Subckt::getInstanceSubckt(inst);

        //Add ports, but don't proceed if the subcircuit cannot be found
        QStringList cktNodes;
        Cell* cell = this->cell();

        if(cell == NULL){ qWarning() << "Error: Could find cell" << inst->subcktName(); return;}
        if(cell->isPhysicalOnly()){return;}
        

        if(ckt){
            cktNodes = ckt->nodes();
        }
        else{
            qWarning() << "Warning: Subckt " << inst->subcktName() << " not found";
            return;
        }

        QStringList instNodes = inst->nodes();

        if(instNodes.count() != cktNodes.count()){
            qWarning() << "Error: different number of nodes for " << inst->name() << "(" << instNodes.count() << ")" << " and " << inst->subcktName() << "(" << cktNodes.count() << ")";

        }

        if(cktNodes.count() != instNodes.count()){

            cerr << "Error(instance.cpp): Instance nodes (" << instNodes.count() << ") do not match subckt nodes (" << cktNodes.count() <<") for " << this->name().toStdString() << "\n";
            return;
        }

        //- Make ports;
        for(int i=0;i<cktNodes.count();i++){
            QString instNode = instNodes[i];
            QString cktNode= cktNodes[i];
            Port * cellPort = cell->getPort(cktNode);
            if(cellPort){
                InstancePort * instPort = new InstancePort(instNode,cellPort,this);
                this->add(instPort);
            }
        }
    }

    void Instance::setAngle(QString angle){
        angle_ = angle;
        if(angle == "R90"){
            xcell = this->cell()->y2();
        }else if(angle == "MY"){
            xcell = this->cell()->x2();
            //Fix instanceports
        }else if(angle == "MX"){
            ycell = ycell +  this->cell()->y1() + this->cell()->y2();
        }
        foreach(Rect* r,this->children()){
            //TODO: HOW TO HANDLE INSTANCES WITH NEGATIVE COORDINATES
            r->translate(-this->x1(),-this->y1());
            this->transform(r);
        }


        this->updateBoundingRect();
    }

    Rect Instance::calcBoundingRect(){


        //- Somehow the cell is not set, so keep the bounding box
        if(this->_cell == 0){
            qDebug() << "_cell in " << this->name() << " instance is null, that should not happen";
            return Rect(static_cast<Rect *>(this));
        }

        Rect r = this->_cell->calcBoundingRect();

        if(this->angle() == "R90"){
            r.rotate(90);
        }


        //TODO: HOW TO HANDLE INSTANCES WITH NEGATIVE COORDINATES
        r.moveTo(this->x1(), this->y1());
        return r;
    }



    void Instance::setCell(QString cell){
        if(Cell::_allcells.contains(cell)){
            this->_cell  = _allcells[cell];
            this->setName(this->_cell->name());
            Rect r = this->_cell->calcBoundingRect();
            this->setLayer("PR");
            this->updateBoundingRect();
        }else{
            qDebug() << "Could not find cell " << cell << " in " << this;
            this->_cell = new Cell();
            this->setName("");
            this->updateBoundingRect();
        }
    }

    Point* Instance::getCellPoint()
    {
        Point* p = new Point(this->x1() + xcell,this->y1() + ycell);
        return p;
    }


    Instance * Instance::getInstance(QString cell){
        Instance * c = new Instance();
        c->setCell(cell);

        return c;
    }

    QString Instance::toString(){
        QString str;
        str.append(this->name());
        str.append(" ");
        str.append(Rect::toString());
        str.append("\n {\n");
        if(this->cell()){
            str.append(this->cell()->toString());
        }else{
            str.append(" Empty cell");
        }
        str.append(" }");
        return str;
    }

    void Instance::fromJson(QJsonObject o){


        angle_ = o["angle"].toString();
        xcell = o["xcell"].toInt();
        ycell = o["ycell"].toInt();
        instanceName_ = o["instanceName"].toString();

        this->ckt_inst_ = new cIcSpice::SubcktInstance();
        this->ckt_inst_->fromJson(o["subcktInstance"].toObject());
        this->setCell(o["cell"].toString());

        //Do cell last. The _cell must be set before bounding rect can be calculated
        Cell::fromJson(o);

    }

    QJsonObject Instance::toJson(){
        QJsonObject o = Cell::toJson();
        o["class"]  = "Instance";
        if(_cell){
            o["cell"] = this->prefix_ + _cell->name();
            o["libpath"] = _cell->libPath();
        }

        o["angle"] = angle_;
        o["xcell"] = xcell;
        o["ycell"] = ycell;
        o["instanceName"] = instanceName_;

        if(this->ckt_inst_){
            o["subcktInstance"] = this->ckt_inst_->toJson();
        }


        return o;
    }

    void Instance::updateUsedChildren(){
        Cell::updateUsedChildren();
        if(this->_cell){
            this->_cell->setUsed(this->cell_used_);
            this->_cell->updateUsedChildren();
        }
    }



}
