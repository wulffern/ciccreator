//====================================================================
//ce
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

    class SimpleRect
    {
    protected:
        int x1_ = 0;
        int y1_ = 0;
        int x2_ = 0;
        int y2_ = 0;

    public:
        SimpleRect() {}
        SimpleRect(int x1, int y1, int x2, int y2) :
            x1_(x1),y1_(y1), x2_(x2), y2_(y2) {}

        int left() const      { return x1_; }
        int right() const     { return x2_; }
        int top() const       { return y1_; }
        int bottom() const    { return y2_; }
        int width() const     { return x2_ - x1_; }
        int height() const    { return y2_ - y1_; }
        int centerX() const   { return x1_ + (x2_ - x1_)/2; }
        int centerY() const   { return y1_ + (y2_ - y1_)/2; }
        bool empty() const    { return (x1_ == x2_ || y1_ == y2_); }

        int x1() const { return x1_; }
        int y1() const { return y1_; }
        int x2() const { return x2_; }
        int y2() const { return y2_; }

        void setPoint1(int x1, int y1) { x1_ = x1;y1_ = y1; }
        void setPoint2(int x2, int y2) { x2_ = x2;y2_ = y2; }

        void setRect(const SimpleRect& rect){
            this->x1_ = rect.x1();
            this->y1_ = rect.y1();
            this->x2_ = rect.x2();
            this->y2_ = rect.y2();
        }

        void setRect(int x, int y, int width, int height) {
            x1_ = x;
            y1_ = y;
            x2_ = x + width;
            y2_ = y + height;
        }

        void adjust(int dx) {
            x1_ -= dx;
            y1_ -=  dx;
            x2_ += dx;
            y2_ += dx;
        }

        void adjust(int dx1, int dy1, int dx2, int dy2){
            x1_ += dx1;
            y1_ +=  dy1;
            x2_ += dx2;
            y2_ += dy2;
        }

        void moveTo(int x, int y) {
            int lwidth = this->width();
            int lheight = this->height();
            x1_ = x;
            y1_ = y;
            x2_ = x + lwidth;
            y2_ = y + lheight;
        }

        void moveCenter(int xc, int yc) {
            int w = width();
            int h = height();
            x1_ = xc - w/2;
            y1_ = yc - h/2;
            x2_ = x1_ + w;
            y2_ = y1_ + h;
        }

        void translate(int ax, int ay) {
            x2_ += ax;
            x1_ += ax;
            y1_ += ay;
            y2_ += ay;
        }

        void setLeft(int left) { x1_ = left;}
        void setRight(int right) { x2_ = right; }
        void setTop(int top) { y2_ = top;}
        void setBottom(int bottom) { y1_ = bottom; }
        void setHeight(int height) { y2_ = y1_ + height; }
        void setWidth(int width) { x2_ = x1_ + width; }

        void rotate(int i);
    };

    class Rect : public QObject, public SimpleRect
    {

        Q_OBJECT

    public:
        using SimpleRect::setRect;

        Rect() :
            SimpleRect(),
            rules(Rules::getRules()),
            _layer("PR"),
            net_("")
        {}

        ~Rect()
        {}

        Rect(QString layer, int left, int bottom, int width, int height) :
            SimpleRect(left, bottom, left + width, bottom + height),
            _layer(layer),
            net_("")
        {}

        Rect(const SimpleRect &r) :
            SimpleRect(r),
            _layer("PR"),
            net_("")
        {}

        Rect(QString layer, const SimpleRect &r) :
            SimpleRect(r),
            _layer(layer),
            net_("")
        {}

        //! Copy rectangle from pointer (r)
        explicit Rect(Rect * r) :
            SimpleRect(*(SimpleRect*)r),
            rules(Rules::getRules()),
            _layer(r->_layer),
            net_(r->net_)
         {}


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


        //! Move this rectangle by ax and ay
        virtual void translate(int ax, int ay);

        //! Move this rectangle to x and y
        virtual void moveTo(int x, int y);

        //! Place center of this rectangel at x and y
        virtual void moveCenter(int x, int y);

        //! Increase the size of this rectangle by dx on all sides
        virtual void adjust(int dx) { SimpleRect::adjust(dx); }
        virtual void adjust(int dx1, int dy1, int dx2, int dy2) { SimpleRect::adjust(dx1, dy1, dx2, dy2); }

        //! Get a rectangele where each side is moved by xp1. Useful for generating a rectangle
        //! to enclose this rectangle by a certain amount
        Rect* adjustedOnce(int xp1);

        static Rect* getScaled(Rect* r,int unit);

        //! Mirror around ax, will send "updated()"
        virtual void mirrorX(int ax);

        //! Mirror arround ay, will send "updated()"
        virtual void mirrorY(int ay);


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

        Rect* _parent = NULL;
        bool isDevice_ = false;

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

        //! Set height, moves y2
        void setHeight(int height);

        //! Set width, moves x2
        void setWidth(int width);

        //! Set coordinates based on rect
        void setRect(QString layer, int x, int y, int width, int height);
    };


    inline void Rect::setLeft(int left){ x1_ = left; emit updated();}
    inline void Rect::setRight(int right){ x2_ = right; emit updated();}
    inline void Rect::setTop(int top){ y2_ = top; emit updated() ;}
    inline void Rect::setBottom(int bottom){ y1_ = bottom; emit updated();}
    inline void Rect::setHeight(int height){ y2_ = y1_ + height; emit updated();}
    inline void Rect::setWidth(int width){ x2_ = x1_ + width; emit updated();}

    inline void Rect::setRect(QString layer, int x, int y, int width, int height) {
        SimpleRect::setRect(x, y, width, height);
        _layer = layer;
    }

    inline void Rect::moveTo(int x, int y) {
        SimpleRect::moveTo(x, y);
        emit updated();
    }

    inline void Rect::moveCenter(int x, int y) {
        SimpleRect::moveCenter(x, y);
        emit updated();
    }

    inline void Rect::translate(int ax, int ay) {
        SimpleRect::translate(ax, ay);
         emit updated();
    }

}
#endif
