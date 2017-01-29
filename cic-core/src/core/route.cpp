//====================================================================
//        Copyright (c) 2015 Carsten Wulff Software, Norway
// ===================================================================
// Created       : wulff at 2015-10-25
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

#include "route.h"

namespace cIcCore{

    int Route::getIntegerFromMatch(QString regex, QString options, int defaultValue)
    {
        int match =defaultValue;
        QRegularExpression re(regex);
        QRegularExpressionMatch m =  re.match(options);
        if(m.hasMatch()){
            match = m.captured(1).toInt();
        }
        return match;
    }

    Route::Route(QString net, QString layer, QList<Rect*> start, QList<Rect*> stop, QString options,QString routeType){
        net_ = net;
        routeLayer_ = layer;
        routeType_ = ROUTE_UNKNOWN;
        route_ = routeType;
        options_ = options;
        sortDirection_ = SORT_RIGHT;
        startOffset_ = NO_OFFSET;
        stopOffset_ = NO_OFFSET;
        track_ = 0;
        cuts_ = 2;
        vcuts_ = 1;
        fillvcut_ = false;
        fillhcut_ = false;
        antenna_ = false;
        hasTrack_= false;
        leftAlignCut = true;
        start_rects_ = start;
        stop_rects_ = stop;
        this->setName(net);
        this->setBoundaryIgnoreRouting(false);

        //Decode options
        if(options.contains(QRegularExpression("fillhcut"))){ fillhcut_ = true ;}
        if(options.contains(QRegularExpression("fillvcut"))){ fillvcut_ = true ;}
        if(options.contains(QRegularExpression("antenna"))){ antenna_ = true ;}

        //- Sort direction
        if(options.contains(QRegularExpression("onTopR"))){
            start_rects_ = Rect::sortRightOnTop(start_rects_);
            stop_rects_ = Rect::sortRightOnTop(stop_rects_);
        }
        else if(options.contains(QRegularExpression("onTopB"))){
            start_rects_ = Rect::sortBottomOnTop(start_rects_);
            stop_rects_ = Rect::sortBottomOnTop(stop_rects_);
        }
        else if(options.contains(QRegularExpression("onTopT"))){
            start_rects_ = Rect::sortTopOnTop(start_rects_);
            stop_rects_ = Rect::sortTopOnTop(stop_rects_);
        }else{
            start_rects_ = Rect::sortLeftOnTop(start_rects_);
            stop_rects_ = Rect::sortLeftOnTop(stop_rects_);
        }

        if(start_rects_.count() == 0 && stop_rects_.count() > 0){
            Rect * r = stop_rects_[0];
            stop_rects_.removeFirst();
            start_rects_.append(r);
        }


        //- Start offset
        if(options.contains(QRegularExpression("offsethigh(,|\\s+|$)"))){ startOffset_ = HIGH ;}
        else if(options.contains(QRegularExpression("offsetlow(\\s+|,|$)"))){ startOffset_ = LOW ;}

        //- Stop offset;
        if(options.contains(QRegularExpression("offsethighend"))){ stopOffset_ = HIGH ;}
        else if(options.contains(QRegularExpression("offsetlowend"))){ stopOffset_ = LOW ;}

        if(options.contains(QRegularExpression("track(\\d+)")))
            hasTrack_ = true;

        track_  = getIntegerFromMatch("track(\\d+)", options,0);

        cuts_  = getIntegerFromMatch("(\\d+)cuts", options,2);

        vcuts_  = getIntegerFromMatch("(\\d+)vcuts", options,1);

        if(routeType == "-|--"){routeType_ = LEFT;}
        else if(routeType == "--|-"){routeType_ = RIGHT;}
        else if(routeType == "-"){routeType_ = STRAIGHT;}
        else if(routeType == "-|"){routeType_ = U_RIGHT;}
        else if(routeType == "|-"){routeType_ = U_LEFT;}
        else if(routeType == "->"){routeType_ = STRAIGHT;}
        else if(routeType == "||"){routeType_ = VERTICAL;}

        else{
            routeType_ = ROUTE_UNKNOWN;
        }

        //- Route
        if(options.contains(QRegularExpression("leftdownleftup"))){
            routeType_ = LEFT_DOWN_LEFT_UP;
        }
        if(options.contains(QRegularExpression("leftupleftdown"))){
            routeType_ = LEFT_UP_LEFT_DOWN;
        }

        if(options.contains(QRegularExpression("strap"))){
            routeType_ = STRAP;
        }


        

    }

    Route::Route(const Route&){

    }

    Route::~Route(){

    }

    void Route::addStartCuts(){
        if(!this->options_.contains(QRegularExpression("nostartcut"))){
            this->addCuts(start_rects_,startcuts);
        }
    }

    void Route::addEndCuts(){
        if(!this->options_.contains(QRegularExpression("noendcut"))){
            this->addCuts(stop_rects_,endcuts);
        }
    }

    void Route::addCuts(QList<Rect*> rects,QList<Rect*>& allcuts){
        //TODO: Make poly routing work, right now all routing must happen in > M1
        if(routeLayer_ == "PO"){
            return;
        }

        
        
        QList<Rect*>  cuts = Cut::getCutsForRects(routeLayer_,rects,cuts_,vcuts_);
        foreach(Rect* r,cuts){
            allcuts.append(r);
        }

        this->add(cuts);

    }

    void Route::route(){

        //Take a copy of all routerects to ensure we don't change the originals
        QList<Rect*> start_rects_org = start_rects_;
        QList<Rect*> stop_rects_org = stop_rects_;
        stop_rects_.clear();
        start_rects_.clear();
        
        foreach(Rect* r, start_rects_org){
            start_rects_.append(r->getCopy());
        }
        foreach(Rect* r, stop_rects_org){
            stop_rects_.append(r->getCopy());
        }
        
        

        
        
        this->addStartCuts();
        this->addEndCuts();
        switch(routeType_){
        case LEFT:
            this->routeOne();
            break;
        case RIGHT:
            leftAlignCut = false;
            this->routeOne();
            break;
        case STRAIGHT:
            this->routeStraight();
            break;
        case VERTICAL:
            this->routeVertical();
            break;
        case U_RIGHT:
            leftAlignCut = false;
            this->routeU();
            break;
        case U_LEFT:
            this->routeU();
            break;
        case LEFT_DOWN_LEFT_UP:
            this->routeLeftDownLeftUp();
            break;
        case LEFT_UP_LEFT_DOWN:
            this->routeLeftUpLeftDown();
            break;
        case STRAP:
            this->routeStrap();
            break;

        default:
            cerr << "Error(route.cpp): Unknown route routeType=" << route_.toStdString() << " net=" << net_.toStdString() << " layer=" << routeLayer_.toStdString() << " options="<< options_.toStdString() << "\n";
            break;

        }

    }

    void Route::applyOffset(int width, Rect* rect,Offset offset)
    {
        switch(offset){
        case HIGH:
            rect->translate(0,+width);
            break;
        case LOW:
            rect->translate(0,-width);
            break;
        case NO_OFFSET:
            break;

        }

        this->updateBoundingRect();


    }

    void Route::routeOne(){
        int width = rules->get(routeLayer_,"width");
        int space = rules->get(routeLayer_,"space");

        Rect start_bound = this->calcBoundingRect(start_rects_);
        
        int hgrid = this->rules->get("ROUTE","horizontalgrid");

        int x = 0;
        if(routeType_ == RIGHT){
            x = start_bound.left() - space - width;                
            
            if(hasTrack_)
                x = x - hgrid*track_ - space;

        }else if(routeType_ ==LEFT){
            x = start_bound.right() + space;
            if(hasTrack_)
                x = x + hgrid*track_ + space;
        }else{

        }

        this->addHorizontalTo(x,start_rects_,startOffset_);
        this->addHorizontalTo(x,stop_rects_,stopOffset_);
        this->updateBoundingRect();
        this->addVertical(x);
    }

    void Route::routeLeftDownLeftUp(){
        int width = rules->get(routeLayer_,"width");
        int space = rules->get(routeLayer_,"space");

        Rect start_bound = this->calcBoundingRect(start_rects_);
        int hgrid = this->rules->get("ROUTE","horizontalgrid");

        int x = 0;

        x = start_bound.x2() + space*2;

        foreach(auto rect,start_rects_){

            auto r = new Rect(routeLayer_,rect->x1(),rect->y1(),x-rect->x1(),width);
            this->applyOffset(width,r,startOffset_);
            this->add(r);

        }

        foreach(auto rect,stop_rects_) {

            auto x1 = rect->x1();
            Rect* ra;
            Rect* rb;
            if (x1 > x) {
                ra = new Rect(routeLayer_,x,rect->y1()-space-width,x1-x ,width);
            } else {
                ra = new Rect(routeLayer_,x1,rect->y1()-space-width,x-x1 ,width);
            }

            rb = new Rect(routeLayer_,ra->x2(),ra->y1(),width,rect->y1() - ra->y1());
            this->add(ra);
            this->add(rb);
        }

        this->updateBoundingRect();


        auto r = new Rect(routeLayer_,x,this->y1(),width,this->height());
        this->add(r);
    }

    void Route::routeLeftUpLeftDown(){
        int width = rules->get(routeLayer_,"width");
        int space = rules->get(routeLayer_,"space");


        Rect start_bound = this->calcBoundingRect(start_rects_);
        int hgrid = this->rules->get("ROUTE","horizontalgrid");

        int x = 0;
        x = start_bound.x2() + space*2;
        int y = 0;
        
        foreach(auto rect,start_rects_){
            auto r = new Rect(routeLayer_,rect->x1(),rect->y1(),x-rect->x1(),width);
            this->applyOffset(width,r,startOffset_);
            this->add(r);

            Rect* rc = new Rect(routeLayer_,r->x2(),r->y1(),width,space+width);
            this->add(rc);
            if(rc->y2() > y)
                y = rc->y2();
        }
        double yca = -1e32;

        

        foreach(auto rect,stop_rects_) {

            auto x1 = rect->x1();
            Rect* ra;
            Rect* rb;
            if (x1 > x) {
                ra = new Rect(routeLayer_,x,y,x1-x + width ,width);
            } else {
                ra = new Rect(routeLayer_,x1,y,x-x1 + width ,width);
            }

            rb = new Rect(routeLayer_,rect->x1(),rect->y2(),width,ra->y2() - rect->y2());

            if (yca < ra->y1()) {
                yca = ra->y1();
            }

            this->add(ra);
            this->add(rb);
        }

        this->updateBoundingRect();


//        auto r = new Rect(routeLayer_,x,yca,width,this->y2() - yca);
//        this->add(r);

    }

    void Route::routeStrap(){
                int width = rules->get(routeLayer_,"width");
	
        if(start_rects_.count() == 1){
            auto sr = start_rects_[0];
            foreach(Rect* r, stop_rects_){
                auto cs = Cut::getInstance(sr->layer(),routeLayer_,1,2);
                auto rc = new Rect(routeLayer_,sr->x1(),r->y1(),r->x1()-sr->x1(),width);
                cs->moveTo(rc->x1(),rc->y1());
                this->add(rc);
                
                if( sr->layer() != routeLayer_){
                    this->add(cs);
                }
            }
        }else if (stop_rects_.count() == 1){
            auto sr = stop_rects_[0];
            foreach(Rect* r, start_rects_){
                auto cs = Cut::getInstance(sr->layer(),routeLayer_,1,2);
                auto rc = new Rect(routeLayer_,r->x2(),r->y1(),sr->x2()-r->x2(),width);
                cs->moveTo(rc->x2()-cs->width(),rc->y1());
                this->add(rc);
                if( sr->layer() != routeLayer_){
                    this->add(cs);
                }
                
            }
        }else{
            qDebug() << "Error: Cannot route strap!";
        }
        
        
            
  }




    void Route::addVertical(int x){

        int width = rules->get(routeLayer_,"width");
        if(this->options_.contains(QRegularExpression("novert"))) return;
        if(this->options_.contains(QRegularExpression("antenna"))){
            //TODO::Add antenna code
            auto nextlayer = this->getRules()->getNextLayer(this->getRules()->getNextLayer(routeLayer_));


            auto c1 = Cut::getInstance(routeLayer_,nextlayer,1,2);
            auto c2 = Cut::getInstance(routeLayer_,nextlayer,1,2);


            Rect* r;

            if(this->height() > c1->height()*2 + width*2){
                r = new Rect(nextlayer,x,this->y1(),width,this->height());
                c1->moveTo(x,this->y1());
                c2->moveTo(x,r->y2() - c2->height());
                this->add(c1);
                this->add(c2);
            }else{
                r = new Rect(routeLayer_,x,this->y1(),width,this->height());

            }

            this->add(r);


        }else{

            Rect * r = Rect::getVerticalRectangleFromTo(routeLayer_,x,this->y1(),this->y2(),width);
            if(r){
                this->add(r);
            }
        }
    }


    void Route::routeVertical(){


        if( !(start_rects_.count() > 0 && stop_rects_.count() > 0)) return;



        Rect start_bound = Cell::calcBoundingRect(start_rects_);
        Rect stop_bound = Cell::calcBoundingRect(stop_rects_);
        int width = this->rules->get(routeLayer_,"width");

        int yc = start_bound.y1();
        int height = stop_bound.y2() - yc;
        if(start_bound.y1() > stop_bound.y1()){
            yc = stop_bound.y1();
            height = start_bound.y2() - yc;

        }
        int xc = start_bound.centerX() - width/2;




        //Align cuts with center of start rectangle
        foreach(Rect * r, this->children()){

            if(r->isCut()){
                r->moveCenter(start_bound.centerX(), r->centerY());
            }
        }

        Rect *r = new Rect(routeLayer_,xc,yc,width,height);
        this->add(r);


    }

    void Route::routeStraight(){
        if(start_rects_.count() == stop_rects_.count()){

            int count = start_rects_.count();
            for(int x = 0;x < count;x++){
                Rect *r1 = start_rects_[x];
                Rect* r2 = stop_rects_[x];

                //Use the lowest rectangle to route
                int height = r1->height();
                if(r1->height() > r2->height()){
                    height = r2->height();
                }

                int center = r1->centerY();
                Rect * r = new Rect(routeLayer_, r1->x1(),center - height/2.0,r2->x2() - r1->x1(),height);
                this->add(r);

                //TODO: Relocate cuts for endcuts
                foreach(Rect* r,endcuts){
                    r->moveCenter(r2->centerX(),center);

                }

            }

        }else if(start_rects_.count() == 1){
            Rect *r1 = start_rects_[0];
            int count = stop_rects_.count();
            for(int x = 0;x < count;x++){
                Rect* r2 = stop_rects_[x];

                //Use the lowest rectangle to route
                int height = r1->height();
                if(r1->height() > r2->height()){
                    height = r2->height();
                }

                int center = r1->centerY();
                Rect * r = new Rect(routeLayer_, r1->x1(),center - height/2.0,r2->x2() - r1->x1(),height);
                this->add(r);

            }

        }


    }


    void Route::routeU(){
        int width = rules->get(routeLayer_,"width");
        int space = rules->get(routeLayer_,"space");

        int hgrid = this->rules->get("ROUTE","horizontalgrid");

        QList<Rect*> allrect = start_rects_ + stop_rects_;
        Rect all_bound = Cell::calcBoundingRect(allrect);

        int x = 0;
        if(routeType_ == U_RIGHT){
            x = all_bound.right() +space;
            if(hasTrack_)
                x = x + hgrid*track_ + space;

        }else if(routeType_ == U_LEFT){
            x = all_bound.left() - space - width;
            if(hasTrack_)
                x = x - hgrid*track_ - space;

        }else{
            cerr << "Error(route.cpp): Unknown U route " << routeType_ << "\n";
        }
        this->addHorizontalTo(x,start_rects_,startOffset_);
        this->addHorizontalTo(x,stop_rects_,stopOffset_);
        this->updateBoundingRect();
        this->addVertical(x);
    }

    void Route::addHorizontalTo(int x, QList<Rect*> rects,Offset offset){
        int width = rules->get(routeLayer_,"width");

        foreach(Rect *r, rects){
            Rect * rect  = Rect::getHorizontalRectangleFromTo(routeLayer_,r->centerX(),x,r->y1(),width);
            if(rect){
                this->applyOffset(width, rect,offset);
                this->add(rect);
            }
        }
    }




}
