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
        _layer = "PR";
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

    void Rect::setPrefix(QString prefix){
        prefix_ = prefix;
    }


    //----------------------------------------------------------------------
    // Properties
    //----------------------------------------------------------------------


    QString Rect::layer(){  return _layer;}

    QList<Rect *> Rect::sortLeftOnTop(QList<Rect *> rects)
    {
        if(rects.count() < 2){return rects;}
        int index = 0;
        int count = 0;
        int x = std::numeric_limits<int>::max();
        foreach(Rect *r, rects){
            if(r->x1() < x){
                index = count;
                x = r->x1();
            }
            count++;
        }

        Rect * a = rects[0];
        rects[0] = rects[index];
        rects[index]  = a;
        return rects;
    }

    QList<Rect *> Rect::sortRightOnTop(QList<Rect *> rects)
    {
        if(rects.count() < 2){return rects;}
        int index = 0;
        int count = 0;
        int x = -std::numeric_limits<int>::max();
        foreach(Rect *r, rects){
            if(r->x2() > x){
                index = count;
                x = r->x2();
            }
            count++;
        }
        Rect * a = rects[0];
        rects[0] = rects[index];
        rects[index]  = a;
        return rects;
    }

    QList<Rect *> Rect::sortBottomOnTop(QList<Rect *> rects)
    {
        if(rects.count() < 2){return rects;}
        int index = 0;
        int count = 0;
        int y = std::numeric_limits<int>::max();
        foreach(Rect *r, rects){
            if(r->y1() < y){
                index = count;
                y = r->y1();
            }
            count++;
        }
        Rect * a = rects[0];
        rects[0] = rects[index];
        rects[index]  = a;
        return rects;
    }

    QList<Rect *> Rect::sortTopOnTop(QList<Rect *> rects)
    {
        if(rects.count() < 2){return rects;}
        int index = 0;
        int count = 0;
        int y = -std::numeric_limits<int>::max();
        foreach(Rect *r, rects){
            if(r->y2() > y){
                index = count;
                y = r->y2();
            }
            count++;
        }

        Rect * a = rects[0];
        rects[0] = rects[index];
        rects[index]  = a;
        return rects;
    }

    bool Rect::isHorizontal()
    {
        if(this->width() >= this->height()) return true;
        return false;
    }

    bool Rect::isVertical()
    {
        if(this->height() >= this->width()) return true;
        return false;
    }

    
    
    void Rect::setLayer(QString layer){  _layer  = layer;}


    Rect *  Rect::parent(Rect* rect){
        if(rect){
            _parent = rect;
        }
        return _parent;
    }


    Rect* Rect::getCopy(){
        Rect* r =  new Rect(_layer, x1() , y1(), width(), height() );
        r->setNet(this->net());
        return r;
    }

    Rect Rect::getScaled(Rect* r, int unit){
        Rect ro;
        ro.setPoint1(r->x1()/unit,r->y1()/unit);
        ro.setPoint2(r->x2()/unit,r->y2()/unit);
        ro.setLayer(r->layer());
        
        return ro;
        
    }
    


    Rect* Rect::getCopy(const QString layer){
        Rect * r = new Rect(layer, x1(), y1(), width(), height() );
        r->setNet(this->net());
        return r;
    }

    int Rect::snap(int x){
        int GRID= 5;
        int _x = (x/GRID)*GRID;
        return _x;
    }

    void Rect::rotate(int i)  {
        Point p1(x1_,y1_);
        Point p2(x2_,y2_);


        p1.rotate(0,0,i);
        p2.rotate(0,0,i);

        if(p2.x < p1.x){
            int xx = p1.x;
            p1.x = p2.x;
            p2.x = xx;
        }

        if(p2.y < p1.y){
            int yy = p1.y;
            p1.y = p2.y;
            p2.y = yy;
        }

        x1_ = p1.x;
        x2_ = p2.x;
        y1_ = p1.y;
        y2_ = p2.y;
        emit updated();
    }


    Rect* Rect::adjustedOnce(int xp1){
        Rect* rect = new Rect(layer(),x1() - xp1, y1() - xp1, width() + 2*xp1, height() + 2*xp1);
        return rect;
    }



    void Rect::mirrorY(int ax){
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


    void Rect::mirrorX(int ay){

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

    Rect *Rect::getHorizontalRectangleFromTo(QString layer, int x1, int x2, int y, int height)
    {
        Rect *r;
        if(x1 > x2){
            r = new Rect(layer,x2,y,x1-x2,height);
        }else{
            r = new Rect(layer,x1,y,x2-x1,height);
        }
        return r;
    }

    Rect *Rect::getVerticalRectangleFromTo(QString layer, int x, int y1, int y2, int width)
    {
        Rect *r;
        if(y1 > y2){
            r = new Rect(layer,x,y2,width,y1-y2);
        }else{
            r = new Rect(layer,x,y1,width,y2-y1);
        }
        return r;

    }


    void Rect::fromJson(QJsonObject o){
        x1_ = o["x1"].toInt();
        x2_ = o["x2"].toInt();
        y1_ = o["y1"].toInt();
        y2_ = o["y2"].toInt();
        _layer = o["layer"].toString();
    }

    QJsonObject Rect::toJson(){
        QJsonObject o;

        //Not all programs like negative width/height, so fix it
        int tmp;
        if(x2_ < x1_){
            tmp = x2_;
            x2_ = x1_;
            x1_ = tmp;
        }

        if(y2_ < y1_){
            tmp = y2_;
            y2_ = y1_;
            y1_ = tmp;
        }

        o["class"] = "Rect";
        o["x1"] = x1_;
        o["y1"] = y1_;
        o["x2"] = x2_;
        o["y2"] = y2_;
        o["layer"] = _layer;
        o["net"] = this->net();
        return o;
    }

    bool Rect::isRect(){
        QString c = this->metaObject()->className();
        if(c == "cIcCore::Rect"){
            return true;
        }

        return false;
    }


    //! Check if this is an cIcCore::Instance object
    bool Rect::isInstance(){
        QString c = this->metaObject()->className();
        if(c == "cIcCore::Instance"){
            return true;
        }else if(this->inherits("cIcCore::Instance")){
            //
            return true;
        }
        return false;
    }

    //! Check if this is a cIcCore::Routeobject
    bool Rect::isRoute(){
        QString c = this->metaObject()->className();
        if(c == "cIcCore::Route"){
            return true;
        }else if(this->inherits("cIcCore::Route")){
            return true;
        }
        return false;
    }

    //! Check if this is a cIcCore::Cut object
    bool Rect::isCut(){
        QString c = this->metaObject()->className();
        if(c == "cIcCore::Cut"){
            return true;
        }else if(this->inherits("cIcCore::Cut")){
            return true;
        }else if(c == "cIcCore::InstanceCut"){
            return true;
        }else if(this->inherits("cIcCore::InstanceCut")){
            return true;
        }
        return false;
    }

    //! Check if this is a cIcCore::Cell object
    bool Rect::isCell(){
        QString c = this->metaObject()->className();
        if(c == "cIcCore::Cell" ){

            return true;
        }else  if(this->inherits("cIcCore::Cell")){
            return true;
        }
        return false;
    }

    //! Check if this is a cIcCore::LayoutCell object
    bool Rect::isLayoutCell(){
        QString c = this->metaObject()->className();
        if(c == "cIcCore::LayoutCell" ){
            return true;
        }else  if(this->inherits("cIcCore::LayoutCell")){
            return true;
        }
        return false;
    }


    //! Check if this is a cIcCore::Port object
    bool Rect::isPort(){
        QString c = this->metaObject()->className();
        if(c == "cIcCore::Port" ){
            return true;
        }else if(this->inherits("cIcCore::Port")){
            return true;
        }
        return false;

    }

    //! Check if this is a cIcCore::Text object
    bool Rect::isText(){
        QString c = this->metaObject()->className();
        if(c == "cIcCore::Text"){
            return true;
        }else if(this->inherits("cIcCore::Text")){
            return true;
        }
        return false;

    }
}
