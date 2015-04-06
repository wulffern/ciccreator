//====================================================================
//        Copyright (c) 2015 Carsten Wulff Software, Norway
// ===================================================================
// Created       : wulff at 2015-1-26
// ===================================================================
// Licensed under the Apache License, Version 2.0 (the "License")
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//====================================================================

#ifndef CIC_CORE_RECT_H
#define CIC_CORE_RECT_H

#include <QObject>
#include <QRectF>
#include <QDebug>

namespace cIcCore{

    class Rect : public QObject
    {
      Q_OBJECT

    public:
        Rect();
        Rect(QString layer, int left, int top, int width, int height);

        Rect* getCopy();
        Rect* getCopy(QString layer);

        QString layer();

        QRect rect();
        int left();
        int right();
        int top();
        int bottom();
        int width();
        int height();
        int centerX();
        int centerY();

        void translate(int ax, int ay);
        void moveTo(int x, int y);
        void mirrorX(int ax);
        void mirrorY(int ay);

        void abut(Rect* rect,qreal dx, qreal dy);
        void abutTopCenter(Rect* rect, qreal dy);
        void abutTopRight(Rect* rect);
        void abutTopLeft(Rect* rect);
        void abutBottom(Rect* rect);
        void abutLeft(Rect* rect,qreal dx, qreal dy);
        void abutRight(Rect* rect,qreal dx, qreal dy);


        void parent(Rect* rect);

        Rect* adjustedOnce(int xp1);

        int snap(int x);

        QString toString();

    private:
        QString _layer;
        Rect* _parent;
        QRect _rect;

    signals:
        void updated();

    public slots:
        void setLayer(QString layer);
        void setLeft(int left);
        void setRight( int right);
        void setTop(int top);
        void setBottom(int bottom);
        void setWidth(int width);
        void setHeight(int height);
        void setRect(QRect rect);

    };

    inline QRect Rect::rect(){ return _rect;}

    inline int Rect::left(){ return _rect.x();}
    inline int Rect::right(){ return _rect.x() + _rect.width();}
    inline int Rect::top(){ return _rect.y() + _rect.height();}
    inline int Rect::bottom(){ return _rect.y();}
    inline int Rect::width(){ return _rect.width();}
    inline int Rect::height(){ return _rect.height();}

    inline int Rect::centerX(){ return left() + width()/2.0;}
    inline int Rect::centerY(){ return top() + height()/2.0;}


    inline void Rect::setLeft(int left){ _rect.setX(left);emit updated();}
    inline void Rect::setRight(int right){ _rect.setWidth(right - _rect.x()); emit updated();}
    inline void Rect::setTop(int top){ _rect.setHeight(top - _rect.y()); emit updated() ;}
    inline void Rect::setBottom(int bottom){ _rect.setX(bottom); emit updated();}
    inline void Rect::setHeight(int height){ _rect.setHeight(height); emit updated();}
    inline void Rect::setWidth(int width){ _rect.setWidth(width); emit updated();}
    inline void Rect::setRect(QRect rect){ _rect.setRect(rect.x(),rect.y(),rect.width(),rect.height()); emit updated();}
    inline void Rect::moveTo(int x, int y){ _rect.moveTo(x,y); emit updated();}
    inline void Rect::translate(int ax, int ay){ _rect.translate(ax,ay); emit updated();}

}
#endif
