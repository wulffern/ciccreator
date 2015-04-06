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

#include "core/rect.h"

namespace cIcCore{

    Rect::Rect(){
        _layer = "M1";
        setLeft(0);
        setTop(0);
        setWidth(0);
        setHeight(0);
    }
    Rect::Rect(QString layer, int x, int y, int width, int height){
        _layer = layer;
        setLeft(x);
        setTop(y);
        setWidth(width);
        setHeight(height);
    }




//----------------------------------------------------------------------
// Properties
//----------------------------------------------------------------------

    QString Rect::layer(){  return _layer;}
    void Rect::setLayer(QString layer){  _layer  = layer;}



//  sub layer{ return (shift)->property("layer",(shift));}
// sub datatype{ return (shift)->property("datatype",(shift));}
//sub rules{ return (shift)->property("rules",(shift));}


//  void Rect::abutTopCenter(Rect* rect, qreal dy){
//    moveCenter(rect->center().x,rect->top() + height()/2.0 + dy);
//  }


    void Rect::parent(Rect* rect){
        _parent = rect;
    }

    Rect* Rect::getCopy(){
        return new Rect(_layer, left() , top(), width(), height() );
    }
    Rect* Rect::getCopy(QString layer){
        return new Rect(layer, left(), top(), width(), height() );
    }

    void Rect::abut(Rect* rect, qreal dx, qreal dy){
        moveTo(rect->right() + dx, rect->top()  + dy);
    }

    void Rect::abutTopLeft(Rect* rect){
        moveTo(rect->left()-width(),rect->top());
    }

    void Rect::abutBottom(Rect* rect){
        moveTo(rect->left(),rect->bottom());
    }

    void Rect::abutLeft(Rect* rect,qreal dx, qreal dy){
        moveTo(rect->right() + dx,rect->bottom()+dy);
    }

    void Rect::abutRight(Rect* rect, qreal dx, qreal dy){
        moveTo(rect->right() + dx,rect->bottom() + dy);
    }

    void Rect::abutTopRight(Rect* rect){
        moveTo(rect->left(),rect->top());
    }

    int Rect::snap(int x){
        int GRID= 5;
        int _x = (x/GRID)*GRID;
        return _x;
    }

    Rect* Rect::adjustedOnce(int xp1){
        Rect* rect = new Rect(layer(),left() - xp1, bottom() - xp1, width() + 2*xp1, height() + 2*xp1);
        return rect;
    }


    void Rect::mirrorX(int ax){
        this->setLeft(2.0*ax - this->left());
        this->setRight(2.0*ax - this->right());

        //Flip left and right if width is negative
        if (this->width() < 0  ) {
            qreal tmp = this->left();
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


    QString Rect::toString(){
        return QString("%6: layer=%1 X=%2 Y=%3 W=%4 H=%5").arg(layer()).arg(left()).arg(top()).arg(width()).arg(height()).arg(this->metaObject()->className());
    }

}
