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
#include "core/rules.h"

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
        void moveCenter(int x, int y);
        void mirrorX(int ax);
        void mirrorY(int ay);

     //   void abut(Rect* rect,qreal dx, qreal dy);
     //   void abutTopCenter(Rect* rect, qreal dy);
     //   void abutTopRight(Rect* rect);
     //   void abutTopLeft(Rect* rect);
     //   void abutBottom(Rect* rect);
      //  void abutLeft(Rect* rect,qreal dx, qreal dy);
     //   void abutRight(Rect* rect,qreal dx, qreal dy);
        void parent(Rect* rect);
        Rect* adjustedOnce(int xp1);
        int snap(int x);

        QString toString();

    protected:
        Rules* rules;

    private:
        QString _layer;

        Rect* _parent;
        int x1_;
        int y1_;
        int x2_;
        int y2_;

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
       // void setRect(QRect rect);
        void setRect( int x, int y, int width, int height){
            x1_  = x;
            y1_ = y;
            x2_ = x + width;
            y2_ = y + height;
        }
        void setRect(QString layer, int x, int y, int width, int height){
            _layer = layer;
            x1_  = x;
            y1_ = y;
            x2_ = x + width;
            y2_ = y + height;
        }

    };

    inline int Rect::left(){ return x1_;}
    inline int Rect::right(){ return x2_;}
    inline int Rect::top(){ return y2_;}
    inline int Rect::bottom(){ return y1_;}
    inline int Rect::width(){ return x2_ - x1_;}
    inline int Rect::height(){ return y2_ - y1_;}

    inline int Rect::centerX(){ return left() + width()/2.0;}
    inline int Rect::centerY(){ return top() + height()/2.0;}


    inline void Rect::setLeft(int left){ x1_ = left;emit updated();}
    inline void Rect::setRight(int right){ x2_ = right; emit updated();}
    inline void Rect::setTop(int top){ y2_ = top; emit updated() ;}
    inline void Rect::setBottom(int bottom){ y1_ = bottom; emit updated();}
    inline void Rect::setHeight(int height){ y2_ = y1_ + height; emit updated();}
    inline void Rect::setWidth(int width){ x2_ = x1_ + width; emit updated();}

    inline void Rect::moveTo(int x, int y){
        x2_ = x2_ + x - x1_;
        y2_ = y2_ + y - y1_;
        x1_ = x;
        y1_ = y;
        emit updated();}

    inline void Rect::moveCenter(int xc, int yc){
      int w = width();
      int h = height();
      x1_ = xc - w/2;
      y1_ = yc - h/2;
      x2_ = x1_ + w;
      y2_ = y1_ + h;
      emit updated();
    }

    inline void Rect::translate(int ax, int ay){
    x2_ += ax;
    x1_ += ax;
    y1_ += ay;
    y2_ += ay;
      emit updated();}

}
#endif
