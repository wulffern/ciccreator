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

  QString Route::getQStringFromMatch(QString regex, QString options, QString defaultValue)
  {
    QString match =defaultValue;
    QRegularExpression re(regex);
    QRegularExpressionMatch m =  re.match(options);
    if(m.hasMatch()){
      match = m.captured(1);
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
    startOffsetCut_ = NO_OFFSET;
    endOffsetCut_ = NO_OFFSET;
    stopOffset_ = NO_OFFSET;
    startTrim_ = NO_TRIM;
    endTrim_ = NO_TRIM;
    track_ = 0;
    startCuts_ = 0;
    startVCuts_ = 0;
    endCuts_ = 0;
    endVCuts_ = 0;

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


    //- Start trim
    if(options.contains(QRegularExpression("trimstartleft(,|\\s+|$)"))){ startTrim_ = TRIM_START_LEFT ;}
    else if(options.contains(QRegularExpression("trimstartright(\\s+|,|$)"))){ startTrim_ = TRIM_START_RIGHT ;}

    //- End trim
    if(options.contains(QRegularExpression("trimendleft(,|\\s+|$)"))){ endTrim_ = TRIM_END_LEFT ;}
    else if(options.contains(QRegularExpression("trimendright(\\s+|,|$)"))){ endTrim_ = TRIM_END_RIGHT ;}

    //- Start offset
    if(options.contains(QRegularExpression("offsethigh(,|\\s+|$)"))){ startOffset_ = HIGH ;}
    else if(options.contains(QRegularExpression("offsetlow(\\s+|,|$)"))){ startOffset_ = LOW ;}

    //- start cut offset
    if(options.contains(QRegularExpression("startoffsetcuthigh(,|\\s+|$)"))){ startOffsetCut_ = HIGH ;}
    else if(options.contains(QRegularExpression("startoffsetcutlow(,|\\s+|$)"))){ startOffsetCut_ = LOW ;}

    //- start cut offset
    if(options.contains(QRegularExpression("endoffsetcuthigh(,|\\s+|$)"))){ endOffsetCut_ = HIGH ;}
    else if(options.contains(QRegularExpression("endoffsetcutlow(,|\\s+|$)"))){ endOffsetCut_ = LOW ;}

    //- Stop offset;
    if(options.contains(QRegularExpression("offsethighend"))){ stopOffset_ = HIGH ;}
    else if(options.contains(QRegularExpression("offsetlowend"))){ stopOffset_ = LOW ;}

    if(options.contains(QRegularExpression("track(\\d+)")))
      hasTrack_ = true;


    track_  = getIntegerFromMatch("track(\\d+)", options,0);


    //Cuts
    startCuts_ = getIntegerFromMatch("(\\d+)startcuts(\\s+|,|$)", options,0);
    startVCuts_ = getIntegerFromMatch("(\\d+)startvcuts(\\s+|,|$)", options,0);

    endCuts_ = getIntegerFromMatch("(\\d+)endcuts(\\s+|,|$)", options,0);
    endVCuts_ = getIntegerFromMatch("(\\d+)endvcuts(\\s+|,|$)", options,0);

    //control the number of horizontal cuts
    cuts_  = getIntegerFromMatch("(\\d+)cuts", options,2);

    //Control the number of vertical cuts
    vcuts_  = getIntegerFromMatch("(\\d+)vcuts", options,1);
    routeWidthRule_ = getQStringFromMatch("routeWidth=([^,\\s+,$]+)",options,"width");

    //Ability to force the start layer. Overerrides start rectangle layer
    startLayer_ = getQStringFromMatch("startLayer=([^,\\s+,$]+)",options,"");

    if(startLayer_ != ""){
      foreach( auto *r,start_rects_){
        r->setLayer(startLayer_);
      }
    }

    //Ability to force the stop layer. Overerrides stop rectangles layer
    stopLayer_ = getQStringFromMatch("stopLayer=([^,\\s+,$]+)",options,"");

    if(stopLayer_ != ""){
      foreach( auto *r,stop_rects_){
        r->setLayer(stopLayer_);
      }
    }





    //Route

    if(routeType == "-|--"){routeType_ = LEFT;}
    else if(routeType == "--|-"){routeType_ = RIGHT; leftAlignCut = false;}
    else if(routeType == "-"){routeType_ = STRAIGHT;}
    else if(routeType == "-|"){routeType_ = U_RIGHT; leftAlignCut = false;}
    else if(routeType == "|-"){routeType_ = U_LEFT;}
    else if(routeType == "--|"){routeType_ = U_TOP;}
    else if(routeType == "|--"){routeType_ = U_BOTTOM;}
    else if(routeType == "->"){routeType_ = STRAIGHT;}
    else if(routeType == "||"){routeType_ = VERTICAL;}

    else{
      routeType_ = ROUTE_UNKNOWN;
    }

    if(options.contains(QRegularExpression("cutalignright(,|\\s+|$)"))){ leftAlignCut = false ; }

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

  Route::Route(const Route& r){
    routeType_ = r.routeType_;
    sortDirection_ = r.sortDirection_;
    startOffset_ = r.startOffset_;
    startOffsetCut_ = r.startOffsetCut_;
    endOffsetCut_ = r.endOffsetCut_;
    stopOffset_ = r.stopOffset_;
    startTrim_ = r.startTrim_;
    endTrim_ = r.endTrim_;
    track_ = r.track_;
    startCuts_ = r.startCuts_;
    startVCuts_ = r.startVCuts_;
    endCuts_ = r.endCuts_;
    cuts_ = r.cuts_;
    vcuts_= r.vcuts_;
    endVCuts_ = r.endVCuts_;
  }

  Route::~Route(){

  }

  void Route::add(Rect *r){
    Cell::add(r);
    if(r){
      if(!r->isCut()){
        routes_.append(r);
      }
    }
  }
  void Route::add(QList<Rect *> children){
    foreach(Rect * r, children){
      this->add(r);
    }
  }

  void Route::addStartCuts(){
    if(!this->options_.contains(QRegularExpression("nostartcut"))){


      int lcuts = startCuts_ > 0 ? startCuts_: cuts_;
      int lvcuts = startVCuts_ > 0 ? startVCuts_: vcuts_;

      QList<Rect*> cuts = this->addCuts(start_rects_,startcuts,lcuts,lvcuts);
      if(startOffsetCut_ == HIGH){
        foreach(auto *cut, cuts){
          cut->translate(0,cut->height()/2);
        }
        foreach(auto *r,start_rects_){
          r->translate(0,r->height()/2);
        }
      }else if(startOffsetCut_ == LOW){
        foreach(auto *cut, cuts){
          cut->translate(0,-cut->height()/2);
        }
        foreach(auto *r,start_rects_){
          r->translate(0,-r->height()/2);
        }
      }

      this->add(cuts);

    }
  }

  void Route::addEndCuts(){
    if(!this->options_.contains(QRegularExpression("noendcut"))){

      int lcuts = endCuts_ > 0 ? endCuts_: cuts_;
      int lvcuts = endVCuts_ > 0 ? endVCuts_: vcuts_;

      QList<Rect*> cuts = this->addCuts(stop_rects_,endcuts,lcuts,lvcuts);

      if(endOffsetCut_ == HIGH){
        foreach(auto *cut, cuts){
          cut->translate(0,cut->height()/2);
        }
        foreach(auto *r,stop_rects_){
          r->translate(0,r->height()/2);
        }
      }else if(endOffsetCut_ == LOW){
        foreach(auto *cut, cuts){
          cut->translate(0,-cut->height()/2);
        }
        foreach(auto *r,stop_rects_){
          r->translate(0,-r->height()/2);
        }
      }



      this->add(cuts);

    }
  }



  //    QList<Rect*> Route::addCuts(QList<Rect*> rects,QList<Rect*>& allcuts){
  //        QList<Rect*> cuts = this->addCuts(rects,allcuts,cuts_,vcuts_);
  //        return cuts;
  //    }

  QList<Rect*> Route::addCuts(QList<Rect*> rects,QList<Rect*>& allcuts,int cuts_, int vcuts_){
    QList<Rect*> cuts;
    //TODO: Make poly routing work, right now all routing must happen in > M1
    if(routeLayer_ == "PO"){
      return cuts;
    }


    cuts = Cut::getCutsForRects(routeLayer_,rects,cuts_,vcuts_,leftAlignCut);
    foreach(Rect* r,cuts){
      allcuts.append(r);
    }

    return cuts;
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


    //- TODO To make it horribly confusing, it matters for the routing which
    //sequence the cuts are added. Should they be added first, or after.
    //
    //For LEFT/RIGHT routes, the bounding box for routing will include the
    //cuts, which may lead to a extra nub on the routes
    //
    //If the cuts are added after, then the routing might not reach the cuts.
    //

    this->addStartCuts();
    this->addEndCuts();

    switch(routeType_){
      case LEFT:
        this->routeOne();

        break;
      case RIGHT:
        this->routeOne();
        break;
      case STRAIGHT:
        this->routeStraight();
        break;
      case VERTICAL:
        this->routeVertical();
        break;
      case U_RIGHT:
        this->routeU();
        break;
      case U_LEFT:
        this->routeU();
        break;
      case U_TOP:
        this->routeUHorizontal();

        break;
      case U_BOTTOM:
        this->routeUHorizontal();
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



    if(start_rects_.count() > 0){
      Rect * r = start_rects_[0];
      Text* t = new Text(this->name());
      t->moveTo(r->x1(),r->y1());
      this->add(t);
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
    int width = rules->get(routeLayer_,routeWidthRule_);
    int space = rules->get(routeLayer_,"space");

    if(this->options_.contains(QRegularExpression("noSpace"))){
      space = -width;
    }


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

    if(startTrim_ == TRIM_START_LEFT){
      foreach(auto rect, start_rects_){
        rect->setLeft(rect->x2() - 2* width);
      }
    }else if(startTrim_ == TRIM_START_RIGHT){
      foreach(auto rect, start_rects_){
        rect->setRight(rect->x1() + 2* width);
      }
    }

    if(endTrim_ == TRIM_END_LEFT){
      foreach(auto rect, stop_rects_){
        rect->setLeft(rect->x2() - 2* width);
      }
    }else if(endTrim_ == TRIM_END_RIGHT){
      foreach(auto rect, stop_rects_){
        rect->setRight(rect->x1() + 2* width);
      }
    }



    this->addHorizontalTo(x,start_rects_,startOffset_);
    this->addHorizontalTo(x,stop_rects_,stopOffset_);
    this->updateBoundingRect();
    this->addVertical(x);

    //TODO:Probably need a better hack than this, not that pretty
    foreach(auto r, add_after_route){
      this->add(r);
    }
  }

  void Route::routeLeftDownLeftUp(){
    int width = rules->get(routeLayer_,routeWidthRule_);
    int space = rules->get(routeLayer_,"space");

    Rect start_bound = this->calcBoundingRect(start_rects_);
    //int hgrid = this->rules->get("ROUTE","horizontalgrid");

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

      rb = new Rect(routeLayer_,ra->x2()-width,ra->y1(),width,rect->y2() - ra->y1());
      this->add(ra);
      this->add(rb);
    }

    this->updateBoundingRect();


    auto r = new Rect(routeLayer_,x,this->y1(),width,this->height());
    this->add(r);
  }

  void Route::routeLeftUpLeftDown(){
    int width = rules->get(routeLayer_,routeWidthRule_);
    int space = rules->get(routeLayer_,"space");


    Rect start_bound = this->calcBoundingRect(start_rects_);
    //int hgrid = this->rules->get("ROUTE","horizontalgrid");

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
        ra = new Rect(routeLayer_,x,y,x1-x  ,width);
      } else {
        ra = new Rect(routeLayer_,x1,y,x-x1  ,width);
      }

      rb = new Rect(routeLayer_,rect->x1()-width,rect->y1(),width,ra->y2() - rect->y1());

      if (yca < ra->y1()) {
        yca = ra->y1();
      }

      this->add(ra);
      this->add(rb);
    }

    this->updateBoundingRect();

  }

  void Route::routeStrapRects(Rect * sr,QList<Rect*> rects,bool start){



  }

  void Route::routeStrap(){

    int lcuts = startCuts_ > 0 ? startCuts_: 1;
    int lvcuts = startVCuts_ > 0 ? startVCuts_: 2;
    int width = rules->get(routeLayer_,routeWidthRule_);

    //Warning: it may look like the code below can be easily
    //refractored, however, the decision where to route (x1,x2, etc)
    //Depends on the start and stop (see rc line)

    //Check if strap is horizontal or vertical
    if(this->options_.contains(QRegularExpression("vertical(,|\\s+|$)"))){
      if(start_rects_.count() == 1){
        auto sr = start_rects_[0];
        foreach(auto r, stop_rects_){
          if(!r){continue;}

          auto rc = new Rect(routeLayer_,r->x1(),sr->y1(),width,r->y1() - sr->y1());
          this->add(rc);

          if(routeLayer_ != sr->layer()){
            auto cs = Cut::getInstance(sr->layer(),routeLayer_,lcuts,lvcuts);
            cs->moveTo(rc->x1(),rc->y2());
            this->add(cs);
          }

        }
      }else if (stop_rects_.count() == 1){
        qDebug() << "Error: Not implemented";
        /*
          auto sr = stop_rects_[0];
          foreach(Rect* r, start_rects_){
          if(!r){continue;}
          auto rc = new Rect(routeLayer_,r->x2(),r->y1(),sr->x2()-r->x2(),width);

          if(routeLayer_ != sr->layer()){
          auto cs = Cut::getInstance(sr->layer(),routeLayer_,lcuts,lvcuts);
          cs->moveTo(rc->x2()-cs->width(),rc->y1());
          this->add(cs);
          }
          this->add(rc);
          }
        */
      }else{
        qDebug() << "Error: Cannot route strap!";
      }
    }else{

      if(start_rects_.count() == 1){
        auto sr = start_rects_[0];
        foreach(auto r, stop_rects_){
          if(!r){continue;}

          auto rc = new Rect(routeLayer_,sr->x1(),r->y1(),r->x1()-sr->x1(),width);
          this->add(rc);

          if(routeLayer_ != sr->layer()){
            auto cs = Cut::getInstance(sr->layer(),routeLayer_,lcuts,lvcuts);
            cs->moveTo(rc->x1(),rc->y2());
            this->add(cs);
          }

        }
      }else if (stop_rects_.count() == 1){
        auto sr = stop_rects_[0];
        foreach(Rect* r, start_rects_){
          if(!r){continue;}
          auto rc = new Rect(routeLayer_,r->x2(),r->y1(),sr->x2()-r->x2(),width);

          if(routeLayer_ != sr->layer()){
            auto cs = Cut::getInstance(sr->layer(),routeLayer_,lcuts,lvcuts);
            cs->moveTo(rc->x2()-cs->width(),rc->y1());
            this->add(cs);
          }
          this->add(rc);
        }
      }else{
        qDebug() << "Error: Cannot route strap!";
      }
    }




  }




  void Route::addVertical(int x){

    int width = rules->get(routeLayer_,routeWidthRule_);
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

      int y1_m =0;
      int y2_m = 0;

      //- Don't include cuts into the equation for start and stop rects
      Rect route_bound = Cell::calcBoundingRect(routes_);
      y1_m = route_bound.y1();
      y2_m = route_bound.y2();


      Rect * r = Rect::getVerticalRectangleFromTo(routeLayer_,x,y1_m,y2_m,width);
      if(r){
        this->add(r);
      }
    }
  }


  void Route::routeVertical(){


    if( !(start_rects_.count() > 0 && stop_rects_.count() > 0)) return;



    Rect start_bound = Cell::calcBoundingRect(start_rects_);
    Rect stop_bound = Cell::calcBoundingRect(stop_rects_);
    int width = this->rules->get(routeLayer_,routeWidthRule_);

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

        //Exception, poly should only be routed minimum??
//        if(routeLayer_ == "PO"){
//            height = rules->get(routeLayer_,"width");

//        }


        if(r1->height() > r2->height()){
          height = r2->height();
        }

        int center = r1->centerY();
        Rect * r = new Rect(routeLayer_, r1->x1(),center - height/2.0,r2->x2() - r1->x1(),height);
        this->add(r);

      }

    }


  }

  void Route::routeUHorizontal(){
    int width = rules->get(routeLayer_,routeWidthRule_);
    int space = rules->get(routeLayer_,"space");



    int vgrid = this->rules->get("ROUTE","verticalgrid");

    QList<Rect*> allrect = start_rects_ + stop_rects_;
    Rect all_bound = Cell::calcBoundingRect(allrect);

    int y = 0;
    if(routeType_ == U_TOP){
      y = all_bound.top() +space;
      if(hasTrack_)
        y = y + vgrid*track_ + space;

    }else if(routeType_ == U_BOTTOM){
      y = all_bound.bottom() - space - width;
      if(hasTrack_)
        y = y - vgrid*track_ - space;

    }else{
      cerr << "Error(route.cpp): Unknown U route " << routeType_ << "\n";
    }

    Rect * rect  = Rect::getHorizontalRectangleFromTo(routeLayer_,all_bound.left(),all_bound.right(),y,width);
    if(rect){this->add(rect);}

    foreach(auto * r,allrect){
      Rect * ra;
      if(routeType_ == U_BOTTOM){
        ra = Rect::getVerticalRectangleFromTo(routeLayer_,r->x1()-width,rect->y1(),r->y2(),width);
      }
      if(routeType_ == U_TOP){
        ra = Rect::getVerticalRectangleFromTo(routeLayer_,r->x1()-width,r->y1(),rect->y2(),width);
      }


      if(ra){this->add(ra);}
    }
    this->updateBoundingRect();

  }


  void Route::routeU(){
    int width = rules->get(routeLayer_,routeWidthRule_);
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
    int width = rules->get(routeLayer_,routeWidthRule_);
    int minwidth = width;
    if(this->rules->hasRule(routeLayer_,"minwidth")){
      minwidth = this->rules->get(routeLayer_,"minwidth");
    };

    foreach(Rect *r, rects){
      if(offset == HIGH or offset == LOW){
        int x1 = r->centerX();
        if(x1 < x){ // Left to right
          Rect * rect1  = Rect::getHorizontalRectangleFromTo(routeLayer_,r->centerX(),r->x2(),r->y1(),width);
          Rect * rect2  = Rect::getHorizontalRectangleFromTo(routeLayer_,r->x2() - minwidth,x,r->y1(),width);
          if(rect1 && rect2){

            this->add_after_route.append(rect1);
            this->add(rect2);
            this->applyOffset(width,rect2,offset);
          }
        }else{//Right to left
          Rect * rect1  = Rect::getHorizontalRectangleFromTo(routeLayer_,r->x1(),r->centerX(),r->y1(),width);
          Rect * rect2  = Rect::getHorizontalRectangleFromTo(routeLayer_,r->x1() + minwidth,x,r->y1(),width);
          if(rect1 && rect2){
            this->add_after_route.append(rect1);
            this->add(rect2);
            this->applyOffset(width,rect2,offset);
          }

        }
      }else{
        Rect * rect  = Rect::getHorizontalRectangleFromTo(routeLayer_,r->centerX(),x,r->y1(),width);
        if(rect){
          this->add(rect);
        }
      }

    }
  }

  Rect * Route::cellFromJson(QJsonObject co){

    auto c = Cell::cellFromJson(co);

    if(c == 0){
      QString cl = co["class"].toString();
      if(cl == "Instance"){
        Instance * i = new Instance();
        i->fromJson(co);
        return i;
      }else{
        qDebug() << "Could not read class " << cl;
        return 0;
      }
    }

    return c;
  }




}
