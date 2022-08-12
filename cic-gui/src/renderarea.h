/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef CIC_GUI_RENDERAREA_H
#define CIC_GUI_RENDERAREA_H

#include <QList>
#include <QPainterPath>
#include <QRect>
#include <QPen>
#include <QWidget>
#include <QPushButton>
#include "cic-core.h"
#include <QPainter>
#include <QPaintEvent>
#include <QScrollArea>

QT_BEGIN_NAMESPACE
class QPaintEvent;
QT_END_NAMESPACE

namespace cIcGui{
  using namespace cIcCore;

enum Operation { NoTransformation, Translate, Rotate, Scale };


class RenderArea : public QWidget{
    Q_OBJECT

public:
    RenderArea(QWidget *parent = 0);
    void setOperations(const QList<Operation> &operations);
    void setCell(Cell * c);
    void setZoom(float zoom);
    void setRenderLevel(int level)
    {
        renderlevel_  = level;
        this->update();
    }

    Rect* getCellRect();
    

    QSize minimumSizeHint() const Q_DECL_OVERRIDE;
    QSize sizeHint() const Q_DECL_OVERRIDE;

    void paintChildren(Cell* c, QPainter &painter,int level);
public slots:
    void zoomIn();
    void zoomOut();
    void fit();

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

//! [1]

//! [2]
private:
  // void RenderArea::drawCoordinates(QPainter)
  // {

  // }

  // void RenderArea::drawOutline(QPainter)
  // {

  // }
  // 	void drawCoordinates(QPainter &painter);
  // 	 void drawOutline(QPainter &painter);
	void invertY(QPainter &painter);
	void nonInvertY(QPainter &painter);
    void drawShape(QPainter &painter);
    void transformPainter(QPainter &painter);
    void drawCell(int x, int y, Cell * c, QPainter &painter,int level);




    
    QList<Operation> operations;
    float _zoom;
    bool first;
	QRect * viewRectangle;
    int xc;
    int renderlevel_;
    int yc;
    Cell * c;
    QRect xBoundingRect;
    QRect yBoundingRect;
    Rules * rules;
};
//! [2]

}
#endif // RENDERAREA_H
