
#include "gui/renderarea.h"

#include <QPainter>
#include <QPaintEvent>

namespace cIcGui{
//! [0]
RenderArea::RenderArea(QWidget *parent)
    : QWidget(parent)
{

  this->c = new Cell();

  rules = Rules::getRules();
  _zoom = 1.0/rules->gamma()  ;
  first = false;
  xc = 0;
  yc =0;
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
    return QSize(800 , 600);
}

QSize RenderArea::sizeHint() const
{
    return QSize(232, 232);
}

void RenderArea::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(event->rect(), QBrush(Qt::white));

   // Rules * rules = Rules::getRules();
   // painter.translate(0, -rules->gamma());

    painter.save();

    if(this->c && this->first){
      QRect rect = event->rect();
      Rect r = this->c->calcBoundingRect();
      r.adjust(this->c->width()/8.0);
      float xscale =  ((float)rect.width())/((float)r.width());
      float yscale = ((float)rect.height())/((float)r.height());
      float scale = xscale > yscale ? yscale : xscale;
      _zoom = scale;
      xc = -r.x();
      yc = -r.y();
      first = false;
    }

    transformPainter(painter);
    drawShape(painter);
    painter.restore();
   // painter.setWindow(c->x(),c->y(),c->width(),c->height());





}

void RenderArea::drawCoordinates(QPainter &painter)
{

}

void RenderArea::drawOutline(QPainter &painter)
{

}

QColor RenderArea::setPen(Rect * o,QPainter &painter){






  Layer *l = rules->getLayer(o->layer());


  QColor color(l->color);

  painter.setPen(QPen(color));
  color.setAlpha(100);
  if(l->nofill){
      painter.setBrush(QBrush(Qt::NoBrush));
    }else{
       painter.setBrush(QBrush(color));
    }



  return color;
}

void RenderArea::drawCell(int x, int y, Cell * c, QPainter &painter){

    painter.translate(x,y);

    painter.setPen(QPen(QColor("black"),10));
    painter.setBrush(QBrush(Qt::NoBrush));
    painter.drawRect(c->x(),c->y(),c->width(),c->height());
    foreach(Rect * r, c->children()){
           if(r->isInstance()){
              Instance *  inst= (Instance *) r;
              this->drawCell(inst->x(),inst->y(),inst->cell(), painter);
             }else{
               setPen(r,painter);
               painter.drawRect(r->x(),r->y(),r->width(),r->height());
             }

      }
    painter.translate(-x,-y);


}

void RenderArea::drawShape(QPainter &painter)
{

   if(this->c != 0){

      this->drawCell(-this->c->x(),0,this->c,painter);
      }

    //painter.fillPath(shape, Qt::blue);
}

void RenderArea::transformPainter(QPainter &painter)
{

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
