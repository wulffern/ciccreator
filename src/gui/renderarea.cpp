
#include "gui/renderarea.h"

#include <QPainter>
#include <QPaintEvent>

namespace cIcGui{
//! [0]
RenderArea::RenderArea(QWidget *parent)
    : QWidget(parent)
{

  this->c = new Cell();
  Rules * rules = Rules::getRules();
  _zoom = 1.0/rules->gamma()  ;

}

void RenderArea::setOperations(const QList<Operation> &operations)
{
    this->operations = operations;
    update();
}

void RenderArea::setCell(Cell *c)
{
    this->c = c;
    update();
}

QSize RenderArea::minimumSizeHint() const
{
    return QSize(182, 182);
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
    transformPainter(painter);
    drawShape(painter);
    painter.restore();
   // painter.setWindow(c->x(),c->y(),c->width(),c->height());

    drawOutline(painter);



    transformPainter(painter);
    drawCoordinates(painter);
}

void RenderArea::drawCoordinates(QPainter &painter)
{

}

void RenderArea::drawOutline(QPainter &painter)
{

}

QColor RenderArea::getPen(Rect * o){

  QColor color;


  if(o->layer() == "PO"){
      color = QColor("red");
  }else if(o->layer() == "M1"){
       color = QColor("blue");
  }else if(o->layer() == "M2"){
             color = QColor("goldenrod");
  }else if(o->layer() == "M3"){
             color = QColor("aqua");
  }else if(o->layer() == "M4"){
             color = QColor("darkgreen");
  }else if(o->layer() == "M5"){
             color = QColor("brown");
  }else if(o->layer() == "M6"){
             color = QColor("darkviolet");

  }else if(o->layer() == "OD"){
             color = QColor("green");
  }else if(o->layer() == "CO"){
             color = QColor("yellow");

  }else{
             color = QColor("grey");

  }

 // color.setAlpha(0.5);
  color.setAlpha(100);

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
               QColor color = getPen(r);
               painter.setPen(QPen(color));
               painter.setBrush(QBrush(color));
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
