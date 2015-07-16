//====================================================================
//        Copyright (c) 2015 Carsten Wulff Software, Norway
// ===================================================================
// Created       : wulff at 2015-1-26
// ===================================================================
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//====================================================================

#ifndef CIC_CORE_RECT_H
#define CIC_CORE_RECT_H

#include <QObject>
#include <QRectF>
#include <QDebug>
#include "core/rules.h"

namespace cIcCore{



    class Rect: public QObject
    {

      Q_OBJECT


    public:
        Rect();
        Rect(const Rect&);
        ~Rect();
        Rect(QString layer, int left, int top, int width, int height);

        Rect* getCopy();
        Rect* getCopy(QString layer);

        QString layer();

       // QRect rect();
        int left();
        int right();
        int top();
        int bottom();
        int width();
        int height();
        int centerX();
        int centerY();
		bool empty();
		int x(){return x1_;}
		int y(){return y1_;}
		int x1(){return x1_;}
		int y1(){return y1_;}
		int x2(){return x2_;}
		int y2(){return y2_;}
		
        void translate(int ax, int ay);
        void moveTo(int x, int y);
        void moveCenter(int x, int y);
        void mirrorX(int ax);
        void mirrorY(int ay);
         void setPoint1(int x1, int y1){x1_ = x1;y1_ = y1;}
         void setPoint2(int x2, int y2){x2_ = x2;y2_ = y2;}
         bool isInstance(){
           if(strcmp(this->metaObject()->className(),"cIcCore::Instance") == 0){
               return true;
             }
           return false;

         }

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
        void setRect(Rect rect){
          this->x1_ = rect.x1();
          this->y1_ = rect.y1();
          this->x2_ = rect.x2();
          this->y2_ = rect.y2();
         // qWarning() << rect.toString();
        }

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
    inline int Rect::centerY(){ return bottom() + height()/2.0;}


    inline void Rect::setLeft(int left){ x1_ = left;emit updated();}
    inline void Rect::setRight(int right){ x2_ = right; emit updated();}
    inline void Rect::setTop(int top){ y2_ = top; emit updated() ;}
    inline void Rect::setBottom(int bottom){ y1_ = bottom; emit updated();}
    inline void Rect::setHeight(int height){ y2_ = y1_ + height; emit updated();}
    inline void Rect::setWidth(int width){ x2_ = x1_ + width; emit updated();}

    inline void Rect::moveTo(int x, int y){
		int width = this->width();
		int height = this->height();
		x1_ = x;
		y1_ = y;
		x2_ = x + width;
		y2_ = y + height;
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

	inline bool Rect::empty(){
		if(x1_ == x2_ || y1_ == y2_){
			return true;
		}
		return false;
	}

    inline void Rect::translate(int ax, int ay){
    x2_ += ax;
    x1_ += ax;
    y1_ += ay;
    y2_ += ay;
      emit updated();}

}
#endif
