//====================================================================
//        Copyright (c) 2016 Carsten Wulff Software, Norway 
// ===================================================================
// Created       : wulff at 2016-8-7
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


RenderArea::RenderArea(QWidget *parent)
    : QWidget(parent)
{
    height = 30;
    QStringList names = QColor::colorNames();
    totalheight = names.count()*height;
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
}

QSize RenderArea::sizeHint() const
{


    return QSize(600, totalheight);
}

QSize RenderArea::minimumSizeHint() const
{
    return QSize(100, 100);
}

void RenderArea::paintEvent(QPaintEvent *  event )
{

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(event->rect(), QBrush(Qt::white));
    painter.save();

    QStringList names = QColor::colorNames();


    int x = 0;
    int y = 0;

    QColor fontcolor("black");
    QFont font=painter.font() ;
    font.setFamily("Arial");
    Qt::BrushStyle bstyle = Qt::SolidPattern;

    foreach(QString name, names){
        QColor color(name);



        painter.setPen(QPen(color,Qt::SolidLine));
        painter.setBrush(QBrush(color,bstyle));
        painter.drawRect(x+150,y,this->width()-150,height);

        painter.setPen(QPen(fontcolor,Qt::SolidLine));
        painter.setFont(font);
        painter.drawText(x + 10,y+height/2,name);


        painter.setPen(QPen(fontcolor,Qt::SolidLine));
        painter.setBrush(QBrush(Qt::NoBrush));
        painter.drawRect(x,y,this->width(),height);
        

        y += height;

    }
    painter.restore();

}
