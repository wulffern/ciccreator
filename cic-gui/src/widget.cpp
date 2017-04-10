
//====================================================================
//        Copyright (c) 2017 Carsten Wulff Software, Norway
// ===================================================================
// Created       : wulff at 2017-1-30
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

#include "widget.h"

Widget::Widget( QWidget *parent)
    : QWidget(parent)
{
    qApp->installEventFilter(this);
    cell = new Cell();
    cellPainter = new CellPainter();
    ycenter = 0;
    xcenter = 0;
    step = 20;
    
    QPalette pal = palette();
  
// set black background
    pal.setColor(QPalette::Background, Qt::white);
    this->setAutoFillBackground(true);
    this->setPalette(pal);

    //setAttribute(Qt::WA_PaintOnScreen);
    //setAttribute(Qt::WA_NoSystemBackground);



}

void Widget::update()
{
//    qDebug() << visibleArea.zoom << visibleArea.rect.toString();
    QWidget::update();
}



void Widget::zoomOut()
{
    visibleArea.zoom /= 1.2;
//    step /=1.2;
    visibleArea.rect.moveTo(visibleArea.rect.x1()/1.2,visibleArea.rect.y1()/1.2);
    this->update();
}

void Widget::zoomIn()
{
    visibleArea.zoom *= 1.2;
//    step *=1.2;
    visibleArea.rect.moveTo(visibleArea.rect.x1()*1.2,visibleArea.rect.y1()*1.2);
    this->update();
}

void Widget::moveLeft()
{
    visibleArea.rect.translate(-step,0);
    this->update();
}


void Widget::moveRight()
{
    visibleArea.rect.translate(+step,0);
    this->update();
}

void Widget::moveUp()
{
    visibleArea.rect.translate(0,step);
    this->update();
}

void Widget::moveDown()
{
    visibleArea.rect.translate(0,-step);
    this->update();
}


void Widget::fit()
{

    if(!cell || !this) return;

    xcenter = this->width()/2;
    ycenter = this->height()/2;

    double margin = 0;

    double width = cell->width()*(1 + margin);
    double height = cell->height()*(1 + margin);
    double wratio = width/this->width();
    double hratio = height/this->height();

    bool isHigh = hratio > wratio ? true: false;
    double ratio = wratio > hratio ? wratio : hratio;

    visibleArea.zoom = 1/ratio;

    double x = 0;
    double y = 0;

    y = cell->centerY()/ratio ;
    x = cell->centerX()/ratio;

    if(isHigh){
//        y -= height*margin/2/ratio;
    }else{
        //      x += width*margin/2/ratio;
    }

    visibleArea.rect.moveTo(x,y);
    this->update();



}


void Widget::zoomArea(QRect r)
{
    if(r.width() == 0 || r.height() == 0|| visibleArea.zoom == 0) return;

    double wratio = ((double)r.width())/this->width();
    double hratio = ((double)r.height())/this->height();
    double ratio = wratio > hratio ? wratio : hratio;
    visibleArea.zoom /= ratio;

    //Visible area is reffered to center of window
    double y = ( this->height() - (origin.y() + r.height()/2.0));
    double x = ( (origin.x() + r.width()/2.0) );

    double ya = y/visibleArea.zoom;
    double xa = x/visibleArea.zoom;


    visibleArea.rect.translate(x,0);
    this->update();
}



void Widget::wheelEvent(QWheelEvent *event)
{

      if (event->modifiers().testFlag(Qt::ControlModifier) ||event->modifiers().testFlag(Qt::MetaModifier) )
    {
        if(event->delta() < 0){
            zoomIn();
        }
        else{
            zoomOut();

        }
    }else  if (event->modifiers().testFlag(Qt::ShiftModifier))
    {
        if(event->delta() < 0){
            moveLeft();
        }
        else{
            moveRight();
        }


    } else {
        if(event->delta() < 0){
            moveDown();
        }
        else{
            moveUp();
        }

    }

}







void Widget::setCell(Cell* c)
{
    cell = c;

    if(!cell || !this) return;

    cell->setBoundaryIgnoreRouting(false);
    cell->updateBoundingRect();
    this->fit();

}



void Widget::paintEvent(QPaintEvent *event)
{


    QPainter painter;
    painter.begin(this);
//    glClearColor(1.0,1.0,1.0,1.0);
//    glClear(GL_COLOR_BUFFER_BIT); // << If I don't do this it clears out my viewer white
// and I want it black
    //   glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//    painter.setBackgroundMode(Qt::TranspaarentMode); // < was a test, doesn't seem to do anything
    painter.setBackground(QColor(1,1,1,1));


    painter.save();
    if(cell){

        //Zoom and invert
//        painter.translate(-visibleArea.rect.x1(),
//        -visibleArea.rect.y1());
//        painter.translate(-cell->width()*visibleArea.zoom/2,-cell->height()*visibleArea.zoom/2);

        painter.scale(visibleArea.zoom,-visibleArea.zoom);
        painter.translate(xcenter/visibleArea.zoom, -ycenter/visibleArea.zoom);
        painter.translate(-visibleArea.rect.x1()/visibleArea.zoom,-visibleArea.rect.y1()/visibleArea.zoom);

//        painter.translate(visibleArea.rect.x1()/visibleArea.zoom, -visibleArea.rect.y1()/visibleArea.zoom);
        cellPainter->paintCell(painter,cell,"");
    }

    painter.restore();

    painter.end();
}

bool isSHIFT;




bool Widget::eventFilter(QObject *obj, QEvent *event)
{

//        if (obj != widget) return false;


    Qt::KeyboardModifiers keyMod = QApplication::keyboardModifiers ();
    isSHIFT = keyMod.testFlag(Qt::ShiftModifier);
    bool isCTRL = keyMod.testFlag(Qt::ControlModifier);

    if(event->type() == QEvent::KeyPress){

        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

        int keyInt = keyEvent->key();

        Qt::Key key = static_cast<Qt::Key>(keyInt);

        if(keyEvent->key() == Qt::Key_F){
            this->fit();

        }


        if(keyEvent->key() == Qt::Key_Z){
            if (isSHIFT)
            {
                this->zoomIn();
                return true;
            }else{
                this->zoomOut();
                return true;
            }
        }

        if (keyEvent->key() == Qt::Key_Up)
        {
            this->moveUp();
            return true;
        }
        else if(keyEvent->key() == Qt::Key_Down)
        {
            this->moveDown();
            return true;
        }else if(keyEvent->key() == Qt::Key_Left)
        {
            this->moveLeft();
            return true;
        }
        else if(keyEvent->key() == Qt::Key_Right)
        {
            this->moveRight();
            return true;
        }
    }

    return QObject::eventFilter(obj, event);

}



void Widget::mousePressEvent(QMouseEvent *event)
{
    origin = event->pos();
    if (!rubberBand)
        rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
    rubberBand->setGeometry(QRect(origin, QSize()));
    rubberBand->show();
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    rubberBand->setGeometry(QRect(origin, event->pos()).normalized());
}

void Widget::mouseReleaseEvent(QMouseEvent *event)
{

    zoomArea(rubberBand->rect());
    rubberBand->hide();


}

