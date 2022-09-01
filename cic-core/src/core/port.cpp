
//====================================================================
//        Copyright (c) 2015 Carsten Wulff Software, Norway
// ===================================================================
// Created       : wulff at 2015-8-20
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

#include "core/port.h"


namespace cIcCore{
    Port::Port(){
        name_ = "";
        routeLayer_ = 0;
        rect_ = 0;
        spicePort = true;
    }

    Port::~Port(){
    }

    Port::Port(QString name){
        name_ = name;
        routeLayer_ = 0;
        rect_ = 0;
        spicePort = true;
    }

    

    void Port::mirrorX(int ay)
    {
        updateRect();
    }

    void Port::mirrorY(int ax)
    {
        updateRect();
    }


    QString Port::name(){return name_;}

    void Port::setName(QString name){name_ = name;}

    QString Port::pinLayer(){if(routeLayer_){return routeLayer_->pin;}else{return "";}}

    void Port::set(Rect * r ){
        if(!r){return;};
        if(r == rect_){return;}
        Layer * l = rules->getLayer(r->layer());
        routeLayer_ = l;
        alternates_rectangles_.append(r);
        this->setLayer(l->name);
        rect_ = r;
        connect(r,SIGNAL(updated()),this, SLOT(updateRect()));
        this->setRect(r->layer(),r->x1(),r->y1(),r->width(),r->height());

    }

    void Port::updateRect(){
        if(rect_){
            this->setRect(rect_->x1(),rect_->y1(),rect_->width(),rect_->height());
        }
        
    }

    Rect * Port::get(){
        Rect* r = 0;

        if(routeLayer_){
            QString layer = routeLayer_->name;
            r = this->getCopy(layer);
            r->setNet(this->name());
        }
        return r;
    }

    Rect * Port::get(QString layer){
        foreach(Rect* r,alternates_rectangles_){
            if(r->layer() == layer){
                Rect * rp = r->getCopy();
                rp->setNet(this->name());
                return rp;
            }
        }
        return NULL;
    }

    QList<Rect*> Port::getAll(QString layer)
    {
        QList<Rect*> rects;
        foreach(Rect* r, alternates_rectangles_){
            if(layer == "" && r->layer() == layer){
                Rect * rp = r->getCopy();
                rp->setNet(this->name());
                rects.append(rp);
            }
        }
        return rects;
    }


    void Port::add(Rect *r){
        alternates_rectangles_.append(r);

    }



    void Port::fromJson(QJsonObject o){
        Rect::fromJson(o);
        name_ = o["name"].toString();
        spicePort = o["spicePort"].toBool();
        routeLayer_ = rules->getLayer(this->layer());
        rect_ = this->getCopy();
        rect_->setNet(this->name());
    }

    QJsonObject Port::toJson(){
        QJsonObject o = Rect::toJson();
        o["class"] = "Port";
        o["name"] = name_;
        o["spicePort"] = spicePort;
        o["pinLayer"] = this->pinLayer();
        return o;
    }

    QString Port::toString(){
        return QString("%7: name=%1 layer=%2 X=%3 Y=%4 W=%5 H=%6").arg(name()).arg(layer()).arg(left()).arg(bottom()).arg(width()).arg(height()).arg(this->metaObject()->className());
    }


}
