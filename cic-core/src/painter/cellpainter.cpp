//====================================================================
//        Copyright (c) 2016 Carsten Wulff Software, Norway
// ===================================================================
// Created       : wulff at 2016-12-5
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


#include "cellpainter.h"

namespace cIcPainter{

    void CellPainter::paintPort(QPainter &painter,Port * p){
        return;

        QFont font=painter.font() ;
        font.setFamily("Arial");

        font.setPointSize (10 );
        Layer *l = Rules::getRules()->getLayer(p->pinLayer());
        if(!l->visible){
            return;
        }
        QColor color(l->color);
        color.setAlpha(150);
        painter.setPen(QPen(color,Qt::SolidLine));
        painter.setFont(font);

        if(l->nofill){
            painter.setBrush(QBrush(Qt::NoBrush));
        }else{
            Qt::BrushStyle bstyle = Qt::SolidPattern;
            painter.setBrush(QBrush(color,bstyle));
        }


        painter.drawRect(toUnit(p->x1()),toUnit(p->y1()),toUnit(p->width()),toUnit(p->height()));
        painter.scale(1,-1);
        painter.drawText(p->x1(),-p->y1(),p->name());
        painter.scale(1,-1);

    }

    void CellPainter::paintText(QPainter &painter,Text * text){
        return;

        QFont font=painter.font() ;
        font.setFamily("Arial");
        font.setPointSize (10 );
        Layer *l = Rules::getRules()->getLayer(text->layer());
        if(!l->visible){
            return;
        }
        QColor color(l->color);
        color.setAlpha(150);
        painter.setPen(QPen(color,Qt::SolidLine));
        painter.setFont(font);

        painter.scale(1,-1);
        painter.drawText(text->x1(),-text->y1(),text->name());
        painter.scale(1,-1);

    }

    double CellPainter::toUnit(int angstrom){
        return angstrom/unit;
        ;}

    void CellPainter::paintReference(QPainter &painter, Instance * inst){

        QTransform trans;

        if(inst->angle() == "R90"){
            trans.rotate(90);
        }else if(inst->angle() == "R180"){
            trans.rotate(180);
        }else if(inst->angle() == "R270"){
            trans.rotate(270);
        }else if(inst->angle() == "MX"){
            trans.scale(1,-1);
        }else if(inst->angle() == "MY"){
            trans.scale(-1,1);
        }


        Point* p = inst->getCellPoint();
        painter.translate(p->x,p->y);

        //Paint with rotated transform
        QTransform old_trans = painter.transform();
        painter.setTransform(trans,true);
        this->paintCell(painter,inst->cell());
        painter.setTransform(old_trans,false);
        painter.translate(-p->x,-p->y);

    }

    void CellPainter::paintRect(QPainter &painter, Rect * r){
        if(r->layer() == "PR") return;

        Layer *l = Rules::getRules()->getLayer(r->layer());
        if(l->nofill || l->color == "" || l->visible == false) return;
        if(!(l->material == Layer::poly ||
             l->material == Layer::metal ||
             l->material == Layer::diffusion ||
             l->material == Layer::cut))return;

        QColor color(l->color);

        color.setAlpha(150);
        painter.setPen(QPen(color,Qt::SolidLine));

        //Set pattern and fill
        Qt::BrushStyle bstyle = Qt::SolidPattern;

        if(l->nofill){
            painter.setBrush(QBrush(Qt::NoBrush));
        }else{
            painter.setBrush(QBrush(color,bstyle));
        }

        painter.drawRect(r->x1(),r->y1(),r->width(),r->height());


};

void CellPainter::startCell(QPainter & painter,Cell *c){
    if(Cell::isEmpty(c)) return;


}

void CellPainter::endCell(QPainter & painter){
}

    void CellPainter::paint(QPainter & painter, Cell *c,int x, int y,int width, int height)
{
    if(Cell::isEmpty(c)) return;

//        painter.setRenderHint(QPainter::Antialiasing);
//        painter.fillRect(, QBrush(Qt::white));
    painter.save();

    //Calculate unit factor
    int w1 = c->width();
    int h1 = c->height();
    int size1 = w1 > h1 ? w1 : h1;

    int h2 = height;
    int w2 = width;
    int size2 = w2 > h2 ? w2 : h2;

    unit = floor(size1/size2);

    //Invert axis
    painter.scale(1,-1);
    painter.translate(0,-h2);
//        painter.translate(-toUnit(c->x1()),-toUnit(c->y1()));
    //  painter.translate(-c->x1(),-c->y1());
    painter.translate(x,y);
    

    this->paintCell(painter,c);

    painter.restore();



}


void CellPainter::paintCell(QPainter & painter,Cell * c){

    if(Cell::isEmpty(c)){return ;}

    this->startCell(painter,c);

    this->paintChildren(painter,c->children());


    this->endCell(painter);
}


void CellPainter::paintChildren(QPainter &painter,QList<Rect*> children){
    foreach(Rect * child,children){
        if(!child){continue;}
        if(child->isInstance()){
            Instance * inst = (Instance*)child;
            if(inst->name() == ""){continue;}
            this->paintReference(painter,(Instance*)child);
        }else if(child->isPort()){
            Port * p = (Port *) child;
            if(p->spicePort){
                this->paintPort(painter,p);
            }

        }else if(child->isText()){
            Text * p = (Text *) child;
            this->paintText(painter,p);
        }else if (child->isCell()){
            Cell * c = (Cell * ) child;
            this->paintChildren(painter,c->children());

        }else{
            this->paintRect(painter,child);
        }

    }

}







};
