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


        Layer *l = Rules::getRules()->getLayer(p->pinLayer());
        if(!l->visible){
            return;
        }
        QColor color(l->color);
        color.setAlpha(150);
        painter.setPen(QPen(color,Qt::SolidLine));
        if(l->nofill){
            painter.setBrush(QBrush(Qt::NoBrush));
        }else{
            Qt::BrushStyle bstyle = Qt::SolidPattern;
            painter.setBrush(QBrush(color,bstyle));
        }
        painter.drawRect(p->x1(),p->y1(),p->width(),p->height());

        int size = int(sqrt(abs(p->width()*p->height())))/4;

        this->paintMyText(painter,p->x1(),p->y1(),p->name(),l->color,size);
    }



    void CellPainter::paintText(QPainter &painter,Text * text){
        return;

        Layer *l = Rules::getRules()->getLayer(text->layer());
        if(!l->visible){
            return;
        }
        this->paintMyText(painter,text->x1(),text->y1(),text->name(),l->color,text->width());

    }

    double CellPainter::toUnit(int angstrom){
        return angstrom/unit;
        ;}

    void CellPainter::paintReference(QPainter &painter, Instance * inst,QString hierarchy){

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

        hierarchy.append(":" + inst->instanceName());


        
        bool isPainting = false;
        if(_instanceName != "" && inst->instanceName() != ""){

            if(_instanceName.contains("|" + hierarchy + "|")){
                _paint = true;
                isPainting = true;
            }
        }

        


        this->paintCell(painter,inst->cell(),hierarchy);

        if(hierarchy.count(QLatin1Char(':')) == 1){
            auto *c = inst->cell();
            Layer *l = Rules::getRules()->getLayer(c->layer());
            if(l->visible){

                this->paintRect(painter,c);

                //Need to avoid wraparound
                QFont font = painter.font();
                font.setPointSize(1);
                QFontMetrics fm(font);
                QString cn = c->name()  + "(" + inst->instanceName() + ")";
                int psize = c->width()/fm.horizontalAdvance(cn)*0.7;

                this->paintMyText(painter,c->centerX(),c->centerY(),cn,l->color,psize);
            }
        }



        if(isPainting){
            _paint = false;
        }


        painter.setTransform(old_trans,false);
        painter.translate(-p->x,-p->y);


    }

    void CellPainter::paintRect(QPainter &painter, Rect * r){

        Layer *l = Rules::getRules()->getLayer(r->layer());
        if(!_paint) return;
        if( l->color == "" || l->visible == false) return;

        QColor color(l->color);

        QPen pen = QPen(color,Qt::SolidLine);

        //Use 50 Ångstrøm for the width
        pen.setWidth(50);
        color.setAlpha(150);
        painter.setPen(pen);


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
        this->paintRect(painter,c);

    }

    void CellPainter::endCell(QPainter & painter){

    }

    /*    void CellPainter::paint(QPainter & painter, Cell *c, int x, int y,int width, int height,QString instanceName)
    {

        _instanceName = instanceName ;
        if(_instanceName == ""){
            _paint = true;
        }else{
            _instanceName += ",";
            
            _paint = false;
        }

        _hasPainted = "";
        
        if(Cell::isEmpty(c)) return;


        painter.save();

        //Calculate unit factor
        painter.scale(1,-1);
        painter.translate(0,-height);
        painter.translate(x,y);
        qDebug() << "Painting " << c->name();
        this->paintRect(painter,c);
        //painter.drawRect(c->x1(),c->y1(),c->width(),c->height());

        this->paintCell(painter,c,"");

        painter.restore();

    }

    void CellPainter::paint(QPainter & painter, Cell *c,int x, int y,int width, int height)
    {

        this->paint(painter,c,x,y,width,height,"");
    }
*/

    void CellPainter::paintCell(QPainter & painter,Cell * c,QString hierarchy){



      if(Cell::isEmpty(c)){qDebug() << "Empty cell\n "; return ;}

        this->startCell(painter,c);

        this->paintChildren(painter,c->children(),hierarchy);

        this->endCell(painter);
    }

    void CellPainter::paintChildren(QPainter &painter,QList<Rect*> children,QString hierarchy){
        foreach(Rect * child,children){
            if(!child){continue;}
            if(child->isInstance()){
                Instance * inst = static_cast<Instance*>(child);

                //- This excludes cuts!! They have no instance name, should they not have that??
                //if(inst->name() == ""){continue;}

                this->paintReference(painter,inst,hierarchy);
            }else if(child->isPort()){
                Port * p = static_cast<Port *>(child);
                if(p->spicePort){
                    this->paintPort(painter,p);
                }
            }else if(child->isText()){
                Text * p = static_cast<Text*>(child);
                this->paintText(painter,p);
            }else if (child->isCell()){
                Cell * c = static_cast<Cell*>(child);
                this->paintChildren(painter,c->children(),hierarchy);
            }else{
                this->paintRect(painter,child);
            }

        }
    }

    void CellPainter::paintMyText(QPainter &painter, int x, int y, QString txt, QString colorName,int size){

        QFont font=painter.font() ;
        font.setFamily("Arial");
        font.setPointSize (size );
        QFontMetrics fm(font);
        int width=fm.horizontalAdvance(txt);
        int height = fm.height();
        QColor color(colorName);
        color.setAlpha(150);
        painter.setPen(QPen(color,Qt::SolidLine));
        painter.setFont(font);
        painter.scale(1,-1);
        painter.drawText(x - width/2,-(y - height/2),txt);
        painter.scale(1,-1);
    }







};
