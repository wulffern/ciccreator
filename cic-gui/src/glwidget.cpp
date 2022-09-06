
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

#include "glwidget.h"

GLWidget::GLWidget( QWidget *parent)
    : QOpenGLWidget(parent)
{
    cell = new Cell();
    cellPainter = new CellPainter();
    ycenter = 0;
    xcenter = 0;

    visibleArea.rect.setRect(0,0,1,1);
    visibleArea.zoom = 1;
    //setAttribute(Qt::WA_PaintOnScreen);
    //setAttribute(Qt::WA_NoSystemBackground);
        

    setAutoFillBackground(true);
    
}

void GLWidget::update()
{
//    qDebug() << visibleArea.zoom << visibleArea.rect.toString();
    QWidget::update();
}



void GLWidget::zoomOut()
{
    visibleArea.zoom /= 1.2;
    visibleArea.rect.moveTo(visibleArea.rect.x1()/1.2,visibleArea.rect.y1()/1.2);
    this->update();
}

void GLWidget::zoomIn()
{
    visibleArea.zoom *= 1.2;
    visibleArea.rect.moveTo(visibleArea.rect.x1()*1.2,visibleArea.rect.y1()*1.2);
    this->update();
}

void GLWidget::moveLeft()
{
    visibleArea.rect.translate(-step,0);
    this->update();
}


void GLWidget::moveRight()
{
    visibleArea.rect.translate(+step,0);
    this->update();
}

void GLWidget::moveUp()
{
    visibleArea.rect.translate(0,step);
    this->update();
}

void GLWidget::moveDown()
{
    visibleArea.rect.translate(0,-step);
    this->update();
}








void GLWidget::setCell(Cell* c)
{
    cell = c;

    if(!cell || !this) return;

    cell->setBoundaryIgnoreRouting(false);
    cell->updateBoundingRect();
    xcenter = this->width()/2;
    ycenter = this->height()/2;

    double margin = 0.1;
    
    double width = cell->width()*(1 + margin);
    double height = cell->height()*(1 + margin);
    double wratio = width/this->width();
    double hratio = height/this->height();

    bool isHigh = hratio > wratio ? true: false;
    double ratio = wratio > hratio ? wratio : hratio;

    visibleArea.zoom = 1/ratio;
    visibleArea.rect.setWidth(width);
    visibleArea.rect.setHeight(height);

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

}



void GLWidget::paintEvent(QPaintEvent *event)
{

    
    QPainter painter;
    painter.begin(this);
    glClearColor(1.0,1.0,1.0,1.0);
    glClear(GL_COLOR_BUFFER_BIT); // << If I don't do this it clears out my viewer white
// and I want it black
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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



