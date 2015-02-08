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
        Q_OBJECT;

    public:
        Rect();
        Rect(QString layer, qreal left, qreal top, qreal width, qreal height);

        Rect* getCopy();
        Rect* getCopy(QString layer);

        QString layer();

        QRectF rect();
        qreal left();
        qreal right();
        qreal top();
        qreal bottom();
        qreal width();
        qreal height();
        qreal centerX();
        qreal centerY();

        void translate(qreal ax, qreal ay);
        void moveTo(qreal x, qreal y);
        void mirrorX(qreal ax);
        void mirrorY(qreal ay);

        void abut(Rect* rect,qreal dx, qreal dy);
        void abutTopCenter(Rect* rect, qreal dy);
        void abutTopRight(Rect* rect);
        void abutTopLeft(Rect* rect);
        void abutBottom(Rect* rect);
        void abutLeft(Rect* rect,qreal dx, qreal dy);
        void abutRight(Rect* rect,qreal dx, qreal dy);


        void parent(Rect* rect);

        Rect* adjustedOnce(qreal xp1);

        qreal snap(qreal x);

        QString toString();

    private:
        QString _layer;
        Rect* _parent;
        QRectF _rect;

    signals:
        void updated();

    public slots:
        void setLayer(QString layer);
        void setLeft(qreal left);
        void setRight( qreal right);
        void setTop(qreal top);
        void setBottom(qreal bottom);
        void setWidth(qreal width);
        void setHeight(qreal height);
        void setRect(QRectF rect);

    };

    inline QRectF Rect::rect(){ return _rect;}

    inline qreal Rect::left(){ return _rect.left();}
    inline qreal Rect::right(){ return _rect.right();}
    inline qreal Rect::top(){ return _rect.top();}
    inline qreal Rect::bottom(){ return _rect.bottom();}
    inline qreal Rect::width(){ return _rect.width();}
    inline qreal Rect::height(){ return _rect.height();}

    inline qreal Rect::centerX(){ return left() + width()/2.0;}
    inline qreal Rect::centerY(){ return top() + height()/2.0;}


    inline void Rect::setLeft(qreal left){ _rect.setLeft(left);emit updated();}
    inline void Rect::setRight(qreal right){ _rect.setRight(right); emit updated();}
    inline void Rect::setTop(qreal top){ _rect.setTop(top); emit updated() ;}
    inline void Rect::setBottom(qreal bottom){ _rect.setBottom(bottom); emit updated();}
    inline void Rect::setHeight(qreal height){ _rect.setHeight(height); emit updated();}
    inline void Rect::setWidth(qreal width){ _rect.setWidth(width); emit updated();}
    inline void Rect::setRect(QRectF rect){ _rect.setRect(rect.x(),rect.y(),rect.width(),rect.height()); emit updated();}
    inline void Rect::moveTo(qreal x, qreal y){ _rect.moveTo(x,y); emit updated();}
    inline void Rect::translate(qreal ax, qreal ay){ _rect.translate(ax,ay); emit updated();}

}
#endif
