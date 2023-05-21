//====================================================================
//        Copyright (c) 2015 Carsten Wulff Software, Norway
// ===================================================================
// Created       : wulff at 2015-1-26
// ===================================================================
//  Th program is free software: you can redistribute it and/or modify
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

#include <limits>
#include <QObject>
#include <QRectF>
#include <QDebug>
#include "core/rules.h"
#include <iostream>
#include "point.h"
using namespace std;

namespace cIcCore{

    class Rect: public QObject
    {

        Q_OBJECT

    public:
        Rect();
        Rect(const Rect&);
		~Rect();

		//! Copy rectangle from pointer (r)
		explicit Rect(Rect *r);

		Rect(QString layer, int left, int bottom, int width, int height);


        bool isHorizontal();
        bool isVertical();
        

		//! Return a copy of this rectangle, creates a new rectangle
        Rect* getCopy();

		//! Get a copy of this rectangle, but with in a different layer
        Rect* getCopy(QString layer);

		//!Name of GDS layer
		QString layer();

		//!Net name of this net
		QString net(){return net_;}
		void setNet(QString net){net_ = net;}

		//!Get the cIcCore::Rules object
		Rules * getRules(){return rules;}

		static QList<Rect*> sortLeftOnTop(QList<Rect*> rects);
		static QList<Rect*> sortRightOnTop(QList<Rect*> rects);
		static QList<Rect*> sortBottomOnTop(QList<Rect*> rects);
		static QList<Rect*> sortTopOnTop(QList<Rect*> rects);


        int left();           //! x1
        int right();          //! x2
        int top();            //! y1
        int bottom();         //! y2
        int width();          //! x2 - x1
        int height();         //! y2 - y1
        int centerX();        //! x1 + (x2 - x1)/2
        int centerY();        //! y1 + (y2 - y1)/2
        bool empty();         //! Is this rectangle empty, x1=x2=y1=y2
//        int x(){return x1_;}  //! x1
//        int y(){return y1_;}  //! y1
        int x1(){return x1_;} 
        int y1(){return y1_;}
        int x2(){return x2_;}
        int y2(){return y2_;}


		//! Move this rectangle by ax and ay
		virtual void translate(int ax, int ay);

		//! Move this rectangle to x and y
		virtual void moveTo(int x, int y);

		//! Place center of this rectangel at x and y
        virtual void moveCenter(int x, int y);

		//! Increase the size of this rectangle by dx on all sides
		virtual void adjust(int dx);
        virtual void adjust(int dx1, int dy1, int dx2, int dy2);

		//! Get a rectangele where each side is moved by xp1. Useful for generating a rectangle
		//! to enclose this rectangle by a certain amount
        Rect* adjustedOnce(int xp1);

        static Rect getScaled(Rect* r,int unit);
        
		
		//! Mirror around ax, will send "updated()"
        virtual void mirrorX(int ax);
		
		//! Mirror arround ay, will send "updated()" 
        virtual void mirrorY(int ay);


		void setPoint1(int x1, int y1){x1_ = x1;y1_ = y1;}
        void setPoint2(int x2, int y2){x2_ = x2;y2_ = y2;}

        bool isRect();
	    bool isInstance();

        //! Check if this is a cIcCore::Routeobject
        bool isRoute();

        //! Check if this is a cIcCore::Cut object
        bool isCut();
        
        //! Check if this is a cIcCore::Cell object
        bool isCell();

        //! Check if this is a cIcCore::LayoutCell object
        bool isLayoutCell();

		//! Check if this is a cIcCore::Port object
        bool isPort();

        //! Check if this is a cIcCore::Text object
        bool isText();
        



		
		//! Check if a rectangle is exactly to the left of of this rectangle
        bool abutsLeft(Rect * r);
		//! Check if a rectangle is exactly to the right of of this rectangle
        bool abutsRight(Rect * r);
		//! Check if a rectangle is exactly to the top of of this rectangle
        bool abutsTop(Rect * r);
		//! Check if a rectangle is exactly to the bottom of of this rectangle
        bool abutsBottom(Rect *);

                //!Get the rectangle of this
        Rect * parent(Rect* rect = 0);
		

		//! Snap to grid, defult 5 ångstrøm
		int snap(int x);

		//! Convert a rectangle to a string that can be printed to console, useful for debug
        virtual QString toString();
		

        static Rect* getHorizontalRectangleFromTo(QString layer, int x1, int x2, int y, int height);
        static Rect* getVerticalRectangleFromTo(QString layer, int x, int y1, int y2,  int width);

        void rotate(int i);

        virtual QJsonObject toJson();
        virtual void fromJson(QJsonObject o);
                        void setPrefix(QString prefix);


    protected:
        Rules* rules;
            QString prefix_;

    private:
        QString _layer;
		QString net_;
		
        Rect* _parent;
        int x1_ = 0;
        int y1_ = 0;
        int x2_ = 0;
        int y2_ = 0;
        bool isDevice_;

    signals:
		//! Notify listeners when the rectangle has moved
        void updated();

    public slots:
		//!Set GDS layer name
        void setLayer(QString layer);

		//! Set the left coordinate (x1)
        void setLeft(int left);

		//! Set the right coordinate (x2)
        void setRight( int right);

		//! Set the top coordinate (y2)
        void setTop(int top);

		//! Set the bottom coordinate (y1)
        void setBottom(int bottom);

		//! Set width, moves x2
        void setWidth(int width);

		//! Set height, moves y2
		      void setHeight(int height);

		//! Set coordinates based on rect
        void setRect(Rect rect){
            this->x1_ = rect.x1();
            this->y1_ = rect.y1();
            this->x2_ = rect.x2();
            this->y2_ = rect.y2();
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

    inline int Rect::centerX(){ return x1_ + width()/2.0;}
    inline int Rect::centerY(){ return y1_ + height()/2.0;}


     inline void Rect::setLeft(int left){ x1_ = left;emit updated();}
     inline void Rect::setRight(int right){ x2_ = right; emit updated();}
     inline void Rect::setTop(int top){ y2_ = top; emit updated() ;}
     inline void Rect::setBottom(int bottom){ y1_ = bottom; emit updated();}
    inline void Rect::setHeight(int height){ y2_ = y1_ + height; emit updated();}
    inline void Rect::setWidth(int width){ x2_ = x1_ + width; emit updated();}

    inline void Rect::moveTo(int x, int y){
        int lwidth = this->width();
        int lheight = this->height();
        x1_ = x;
        y1_ = y;
        x2_ = x + lwidth;
        y2_ = y + lheight;
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

    inline void Rect::adjust(int dx){
        x1_ -= dx;
        y1_ -=  dx;
        x2_ += dx;
        y2_ += dx;
    }

    inline void Rect::adjust(int dx1, int dy1, int dx2, int dy2){
        x1_ += dx1;
        y1_ +=  dy1;
        x2_ += dx2;
        y2_ += dy2;
    }

    inline void Rect::translate(int ax, int ay){
        x2_ += ax;
        x1_ += ax;
        y1_ += ay;
        y2_ += ay;
        emit updated();}

}
#endif
