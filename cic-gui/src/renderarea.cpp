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
    first = false;
    xc = 0;
    yc =0;
  }

  void RenderArea::setZoom(float zoom){
    this->resize(this->sizeHint());
    _zoom = zoom; update();
  }

  void RenderArea::setOperations(const QList<Operation> &operations)
  {
    this->operations = operations;
    update();
  }

  void RenderArea::setCell(Cell *c)
  {
    this->c = c;
    first = true;
    update();
  }

  QSize RenderArea::minimumSizeHint() const
  {
    return QSize(1000 , 800);
  }



  QSize RenderArea::sizeHint() const
  {
	  
    int w = 1000;
    int h = 800;
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

  void RenderArea::paintEvent(QPaintEvent *event)
  {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(event->rect(), QBrush(Qt::white));

    painter.save();

    if(this->c && this->first){
        QRect rect = event->rect();
        Rect r = this->c->calcBoundingRect();
        r.adjust(this->c->width()/8.0);
        float xscale =  ((float)rect.width())/((float)r.width());
        float yscale = ((float)rect.height())/((float)r.height());
        float scale = xscale > yscale ? yscale : xscale;
        _zoom = scale;
        xc = -r.x1();
        yc = -r.y1();
        first = false;
      }

    transformPainter(painter);
    drawShape(painter);
    painter.restore();
  }





  void RenderArea::paintChildren(Cell* c, QPainter &painter)
  {
	  if(!c)
		  return;
	  
    foreach(Rect * r, c->children()){
        if(r->isInstance()){
            Instance *  inst= (Instance *) r;

            QTransform old_trans = painter.transform();
            QTransform trans;
            //qDebug() << inst->angle();
            if(inst->angle() == "R90"){
                  trans.rotate(90);
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
            this->drawCell(inst->x1(),inst->y1(),inst->cell(), painter);
            painter.setTransform(old_trans,false);
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
             painter.drawText(p->x1(),p->y1()+p->height(),p->name());
              }

          }else if(r->isCell()){
            Cell * childcell = (Cell*)r;
            this->paintChildren(childcell,painter);
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

  void RenderArea::drawCell(int x, int y, Cell * c, QPainter &painter){

    if(c==NULL){return;}
    painter.translate(x,y);

    painter.setPen(QPen(QColor("black"),10));
    painter.setBrush(QBrush(Qt::NoBrush));
    painter.drawRect(c->x1(),c->y1(),c->width(),c->height());
    this->paintChildren(c, painter);
    painter.translate(-x,-y);
    QRectF r = painter.window();
    //  this->resize(r.width(),r.height());


  }

  void RenderArea::drawShape(QPainter &painter)
  {

    if(this->c != 0){

        this->drawCell(-this->c->x1(),0,this->c,painter);
      }

    //painter.fillPath(shape, Qt::blue);
  }

  void RenderArea::transformPainter(QPainter &painter)
  {

	  if(!c)
		  return;
    painter.scale(_zoom,_zoom);
    painter.translate(xc,yc);
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
