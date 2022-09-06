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

#ifndef CIC_GLWIDGET_H
#define CIC_GLWIDGET_H

#include <QOpenGLWidget>
#include <QKeyEvent>
#include <QApplication>
#include "cic-core.h"

using namespace cIcCore;
using namespace cIcPainter;


struct VisibleArea
{

    Rect rect;
    double zoom;
};

    


class GLWidget : public QOpenGLWidget
{

    Q_OBJECT

public:
    explicit GLWidget(QWidget * parent);
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void setCell(Cell* c);
    void zoomOut();
    void zoomIn();
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();
    virtual void update();

protected:

    
    
private:
    Cell* cell;
    CellPainter* cellPainter;
    VisibleArea visibleArea;
    
    int step = 10;

    double xcenter = 0;
    double ycenter = 0;
    

};


#endif
    
