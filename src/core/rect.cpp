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

#include "core/rect.h"

namespace cIcCore{


    //----------------------------------------------------------------------
    // Constructors
    //----------------------------------------------------------------------
    Rect::~Rect(){
        x1_ = 0;
        y1_ = 0;
        x2_ = 0;
        y2_ = 0;
        rules = Rules::getRules();
    }

    Rect::Rect(const Rect& r){
        x1_= r.x1_;
        x2_= r.x2_;
        y1_= r.y1_;
        y2_= r.y2_;
        _layer= r._layer;
        rules = Rules::getRules();
    }


    Rect::Rect(Rect * r){
        x1_= r->x1_;
        x2_= r->x2_;
        y1_= r->y1_;
        y2_= r->y2_;
        _layer= r->_layer;
        rules = Rules::getRules();
    }

    Rect::Rect(){
        _layer = "PR";
        x1_ = 0;
        y1_ = 0;
        x2_ = 0;
        y2_ = 0;
        rules = Rules::getRules();
    }

    Rect::Rect(QString layer, int x, int y, int width, int height){
        _layer = layer;
        x1_ = x;
        y1_ = y;
        x2_ = x + width;
        y2_ = y + height;
        rules = Rules::getRules();
    }

    //----------------------------------------------------------------------
    // Properties
    //----------------------------------------------------------------------


	QString Rect::layer(){  return _layer;}

    void Rect::setLayer(QString layer){  _layer  = layer;}


    Rect *  Rect::parent(Rect* rect){
        if(rect){
            _parent = rect;
        }
        return _parent;
    }


    Rect* Rect::getCopy(){
        return new Rect(_layer, x1() , y1(), width(), height() );
    }
	

    Rect* Rect::getCopy(const QString layer){
        return new Rect(layer, x1(), y1(), width(), height() );
    }

    int Rect::snap(int x){
        int GRID= 5;
        int _x = (x/GRID)*GRID;
        return _x;
    }


    Rect* Rect::adjustedOnce(int xp1){
        Rect* rect = new Rect(layer(),x1() - xp1, y1() - xp1, width() + 2*xp1, height() + 2*xp1);
        return rect;
    }



    void Rect::mirrorX(int ax){
        this->setLeft(2.0*ax - this->left());
        this->setRight(2.0*ax - this->right());

        //Flip left and right if width is negative
        if (this->width() < 0  ) {
            int tmp = this->left();
            this->setLeft(this->right());
            this->setRight(tmp);
        }
        emit updated();
    }


    void Rect::mirrorY(int ay){

        this->setTop(2 *  ay - this->top());
        this->setBottom(2 *  ay - this->bottom());

        //Flip top and bottom if height is negative
        if (this->height() < 0) {
            qreal tmp = this->top();
            this->setTop(this->bottom());
            this->setBottom(tmp);
        }

        emit updated();
    }


    bool Rect::abutsLeft(Rect * r){
        if(this->x2() == r->x1() && this->y1() == r->y1()&& this->y2() == r->y2()){
            return true;
        }
        return false;
    }


    bool Rect::abutsRight(Rect * r){
        if(this->x1() == r->x2() && this->y1() == r->y1()&& this->y2() == r->y2()){
            return true;
        }
        return false;
    }


    bool Rect::abutsTop(Rect * r){
        if(this->x1() == r->x1() && this->y2() == r->y1()&& this->x2() == r->x2()){
            return true;
        }
        return false;
    }

    bool Rect::abutsBottom(Rect * r){
        if(this->x1() == r->x1() && this->y1() == r->y2()&& this->x2() == r->x2()){
            return true;
        }
        return false;
    }



    QString Rect::toString(){
        return QString("%6: layer=%1 X=%2 Y=%3 W=%4 H=%5").arg(layer()).arg(left()).arg(bottom()).arg(width()).arg(height()).arg(this->metaObject()->className());
    }

}
