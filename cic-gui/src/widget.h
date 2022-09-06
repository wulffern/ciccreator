
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

#ifndef CIC_WIDGET_H
#define CIC_WIDGET_H

#include <QWidget>
#include <QKeyEvent>
#include <QApplication>
#include "cic-core.h"
#include "cellpainter.h"
#include <QRubberBand>
#include <QRect>
#include <QPixmap>
using namespace cIcCore;
using namespace cIcPainter;


struct VisibleArea
{

    Rect rect;
    double zoom;
};





class Widget : public QWidget
{

    Q_OBJECT

public:
    explicit Widget(QWidget * parent);
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void setCell(Cell* c);
    void zoomOut();
    void zoomIn();
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();
    void fit();
    void zoomArea(Rect r);
    QPixmap getIcon(Layer *l);
    QImage drawColorIconProof(QColor color, const QImage & alpha);
    QImage icon(int size);
    void drawColorIcon(QPainter & p, QColor color, const QImage & alpha);
    
    
    virtual void update();

    
    

protected:
    virtual void wheelEvent(QWheelEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    
private:
    Cell* cell;
    CellPainter* cellPainter;
    VisibleArea visibleArea;
    QRubberBand* rubberBand;
    bool rubberBandActive;
    
    int step;

    double xcenter;
    double ycenter; 
    QPoint  origin;
    
    

};


#endif
    
