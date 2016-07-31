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


#include "renderarea.h"

namespace cIcGui{
    //! [0]
    RenderArea::RenderArea(QWidget *parent)
        : QWidget(parent)
    {

        this->c = new Cell();

        rules = Rules::getRules();
        if(rules){
            _zoom = 1.0/rules->gamma()  ;

        }else{
            _zoom = 1.0;
        }
        renderlevel_=0;
        first = false;
        xc = 0;
        yc =0;
    }

    void RenderArea::setZoom(float zoom){
        this->resize(this->sizeHint());
        _zoom = zoom; update();
    }

    void RenderArea::fit(){

        if(this->c){
            QScrollArea* ar =  static_cast<QScrollArea*>(this->parentWidget());
            if(!ar) return;
            QSize rect = ar->size();
            Rect r = this->c->calcBoundingRect();
            float xscale =  ((float)rect.width())/((float)r.width());
            float yscale = ((float)rect.height())/((float)r.height());
            float scale = xscale > yscale ? yscale : xscale;
            _zoom = scale;
        }

        update();
    }

    void RenderArea::setOperations(const QList<Operation> &operations)
    {
        this->operations = operations;
        update();
    }

    void RenderArea::setCell(Cell *c)
    {
        if(c==0) return;
        this->c = c;

        first = true;
        this->update();
        

    }


    QSize RenderArea::minimumSizeHint() const
    {
        return QSize(4000 , 4000);
    }



    QSize RenderArea::sizeHint() const
    {
        int w = 4000;
        int h = 4000;
        if(!c)
            return QSize(w,h);

        if(!this->c->empty()){
            int w1 = this->c->width()*1.25 * _zoom + 200;
            int h1 = this->c->height()*1.25 * _zoom + 400;
            w = w1 < w ? w : w1;
            h = h1 < h ? h : h1;
        }
        return QSize(w, h);
    }

    void RenderArea::zoomIn()
    {
        this->setZoom(_zoom*1.25);

    }

    void RenderArea::zoomOut()
    {
        this->setZoom(_zoom/1.25);
    }

    void RenderArea::invertY(QPainter &painter){
        int w1 = this->c->width()*1*_zoom;
        int h1 = this->c->height()*1*_zoom;

        painter.translate(0,+h1);
        painter.scale(1,-1);
//        painter.setMatrix( m );

    }

    void RenderArea::nonInvertY(QPainter &painter){
        int w1 = this->c->width()*1*_zoom;
        int h1 = this->c->height()*1*_zoom;
        painter.scale(1,-1);
        painter.translate(0,-h1);

    }


    Rect* RenderArea::getCellRect()
    {
        if(c){
            int w1 = this->c->width()*1*_zoom;
            int h1 = this->c->height()*1*_zoom;
            int x1 = this->width()/2-w1/2;
            int y1 = this->height()/2-h1/2;
            return new Rect("PR",x1,y1,w1,h1);
        }
        return new Rect("PR", this->width()/2,this->height()/2,10,10);
    }

    
    

    void RenderArea::paintEvent(QPaintEvent *event)
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.fillRect(event->rect(), QBrush(Qt::white));
        painter.save();

        int w1 = this->c->width()*1*_zoom;
        int h1 = this->c->height()*1*_zoom;

        
        painter.translate(this->width()/2-w1/2,this->height()/2-h1/2);
        invertY(painter);
        
        transformPainter(painter);

        if(this->c != 0){
            this->drawCell(0,0,this->c,painter,0);
        }

        painter.restore();
    }


    void RenderArea::paintChildren(Cell* c, QPainter &painter,int level)
    {
        if(!c)
            return;



        foreach(Rect * r, c->children()){
            if(r->isInstance() && renderlevel_ >= level){
                Instance *  inst= (Instance *) r;

                QTransform old_trans = painter.transform();
                QTransform trans;
                //qDebug() << inst->angle();
                int x = inst->x1();
                int y = inst->y1();
                
                
                if(inst->angle() == "R90"){
                    trans.rotate(90);
                    y -= inst->height();
                }else if(inst->angle() == "R180"){
                    trans.rotate(180);
                }else if(inst->angle() == "R270"){
                    trans.rotate(270);
                }else if(inst->angle() == "MX"){
                    trans.scale(-1,1);
                }else if(inst->angle() == "MY"){
                    trans.scale(1,-1);
                }
                painter.setTransform(trans,true);
                this->drawCell(x,y,inst->cell(), painter,level+1);
                painter.setTransform(old_trans,false);
            }else if(r->isText()){
                if(c == this->c){
                    Text * p = (Text *) r;
                    QFont font=painter.font() ;
                    font.setFamily("Arial");
                    font.setPointSize (500 );
                    Layer *l = rules->getLayer(p->layer());
                    if(!l->visible){
                        continue;
                    }
                    QColor color(l->color);
                    color.setAlpha(150);
                    painter.setPen(QPen(color,Qt::SolidLine));
                    painter.setFont(font);


                    painter.scale(1,-1);
//                  painter.translate(0,-this->c->height());

                    painter.drawText(p->x1(),-p->y1(),p->name());

//                  painter.translate(0,+this->c->height());
                    painter.scale(1,-1);

                }

            }else if(r->isPort()){
                if(c == this->c){
                    Port * p = (Port *) r;
                    QFont font=painter.font() ;
                    font.setFamily("Arial");

                    font.setPointSize (500 );
                    Layer *l = rules->getLayer(p->pinLayer());
                    if(!l->visible){
                        continue;
                    }
                    QColor color(l->color);
                    color.setAlpha(150);
                    painter.setPen(QPen(color,Qt::SolidLine));
                    painter.setFont(font);

                    painter.scale(1,-1);
//                  painter.translate(0,-this->c->height());

                    painter.drawText(p->x1(),-p->y1(),p->name());

//                  painter.translate(0,+this->c->height());
                    painter.scale(1,-1);

                }

            }else if(r->isCell() && renderlevel_ >= level){
                Cell * childcell = (Cell*)r;
                this->paintChildren(childcell,painter,level);
            }
            else{
                Layer *l = rules->getLayer(r->layer());
                if(!l->visible){
                    continue;
                }
                QColor color(l->color);
                color.setAlpha(150);
                painter.setPen(QPen(color,Qt::SolidLine));

                //Set pattern and fill
                Qt::BrushStyle bstyle = Qt::SolidPattern;
                //            if(l->material == Layer::metal){
                //                int index = l->name.mid(1,1).toInt();
                //                float res = index % 2;
                //                if(res ==  0){
                //                    bstyle =   Qt::FDiagPattern;
                //                  }else{
                //                    bstyle  = Qt::BDiagPattern;
                //                  }
                //              }
                if(l->nofill){
                    painter.setBrush(QBrush(Qt::NoBrush));
                }else{
                    painter.setBrush(QBrush(color,bstyle));
                }
                //setPen(r,painter);
                painter.drawRect(r->x1(),r->y1(),r->width(),r->height());
            }

        }
    }

    void RenderArea::drawCell(int x, int y, Cell * c, QPainter &painter,int level){

        if(c==NULL){return;}
        painter.translate(x,y);
        painter.setPen(QPen(QColor("black"),0.5/_zoom));
        painter.setBrush(QBrush(Qt::NoBrush));
        painter.drawRect(c->x1(),c->y1(),c->width(),c->height());
        this->paintChildren(c, painter, level);
        painter.translate(-x,-y);
        QRectF r = painter.window();
        //  this->resize(r.width(),r.height());


    }

    void RenderArea::drawShape(QPainter &painter)
    {

        

        //painter.fillPath(shape, Qt::blue);
    }

    void RenderArea::transformPainter(QPainter &painter)
    {

        if(!c)
            return;
        painter.scale(_zoom,_zoom);
        for (int i = 0; i < operations.size(); ++i) {
            switch (operations[i]) {
            case Translate:
                painter.translate(50, 50);
                break;
            case Scale:
                break;
            case Rotate:
                painter.rotate(60);
                break;
            case NoTransformation:
            default:
                ;
            }
        }
    }

}
