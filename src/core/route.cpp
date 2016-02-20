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

    Route::Route(QString net, QString layer, QList<Rect*> start, QList<Rect*> stop, QString options,QString routeType){
        net_ = net;
        routeLayer_ = layer;
        routeType_ = ROUTE_UNKNOWN;
        options_ = options;
        sortDirection_ = SORT_RIGHT;
        startOffset_ = NO_OFFSET;
        stopOffset_ = NO_OFFSET;
        track_ = 0;
        cuts_ = 2;
        vcuts_ = 1;
        antenna_ = false;
        start_rects_ = start;
        stop_rects_ = stop;

        if(options.contains(QRegularExpression("antenna"))){ startOffset_ = HIGH ;}

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

        //- Track
        QRegularExpressionMatch * m = 0;
        if(options.contains(QRegularExpression("track(\\d+)"),m)){
              //if(m && m->has)
            //TODO: Fix trac capture
            //track_ = m->captured(0).toInt();
        }

        //- Cuts
        if(options.contains(QRegularExpression("(\\d+)cuts"),m) ){
           // cuts_ = m->captured(0).toInt();
           //TODO: Fix cuts
        }

        //- Vertical cuts
        if(options.contains(QRegularExpression("(\\d+)vcuts"),m) ){
           // vcuts_ = m->captured(0).toInt();
            //TODO: fix vcuts_;
        }



        if(routeType == "-|--"){routeType_ = LEFT;}
        else if(routeType == "--|-"){routeType_ = RIGHT;}
        else if(routeType == "-"){routeType_ = STRAIGHT;}
        else if(routeType == "->"){routeType_ = STRAIGHT;}
        else if(routeType == "||"){routeType_ = VERTICAL;}
        else{
            routeType_ = ROUTE_UNKNOWN;
            qDebug() << "Unknown route" << routeType << net << layer << options;
          }

        //- Route
        if(options.contains(QRegularExpression("leftdownleftup"))){
            routeType_ = LEFT_DOWN_LEFT_UP;
        }
    }

    Route::Route(const Route&){

    }

    Route::~Route(){

    }

    void Route::addStartCuts(){
        if(!this->options_.contains(QRegularExpression("nostartcut"))){
            this->addCuts(start_rects_);
        }
    }

    void Route::addEndCuts(){
        if(!this->options_.contains(QRegularExpression("noendcut"))){
            this->addCuts(stop_rects_);
        }
    }

    void Route::addCuts(QList<Rect*> rects){
        //TODO: Make poly routing work, right now all routing must happen in > M1
        if(routeLayer_ == "PO"){
            return;
        }

        foreach(Rect *r, rects){
            if(routeLayer_ != r->layer()){
                Instance * inst= Cut::getInstance(routeLayer_,r->layer(),this->cuts_,this->vcuts_);
                inst->moveTo(r->centerX() - inst->width()/2.0,r->y1());
                this->add(inst);
                r->setWidth(inst->width());
                r->moveTo(inst->x1(),inst->y1());
            }else{
              this->add(r);
          }
        }
    }

    void Route::route(){
        this->addStartCuts();
        this->addEndCuts();
        switch(routeType_){
          case LEFT:
            this->routeLeft();
            break;
          case RIGHT:
            this->routeRight();
            break;
          case LEFT_DOWN_LEFT_UP:
            break;
          case STRAIGHT:
            this->routeStraight();
            break;
          case VERTICAL:
            this->routeVertical();
            break;
          default:
                break;

        }

    }

    void Route::applyOffset(int width, Rect* rect,Offset offset)
    {
      switch(offset){
        case HIGH:
          rect->translate(0,-width);
          break;
        case LOW:
          rect->translate(0,width);
          break;

        }
    }

    void Route::routeRight(){
      Rules * rules = Rules::getRules();
      int width = rules->get(routeLayer_,"width");
      int space = rules->get(routeLayer_,"space");

      Rect start_bound = this->calcBoundingRect(start_rects_);

      int hgrid = this->rules->get("ROUTE","horizontalgrid");
      start_bound.adjust(-space - width +  -hgrid*track_,0,0,0);

      //- Extend left rectangles
      foreach(Rect *r, start_rects_){
          Rect * start = new Rect(routeLayer_,start_bound.left(),r->y1(),r->x1() - start_bound.left() ,width);
          if(start){
              this->applyOffset(width, start,startOffset_);
              this->add(start);
            }
      }

      //- Extend right rectangles
            foreach(Rect *r, stop_rects_){
                Rect * stop ;
                if(r->x1() > start_bound.left()){
                   stop =  new Rect(routeLayer_,start_bound.left(),r->y1(), r->x1()-start_bound.left() ,width);
                  }else{
                    stop = new Rect(routeLayer_,r->x1(),r->y1(),start_bound.left() - r->x1(),width);
                  }
                if(stop){
                    this->applyOffset(width, stop,stopOffset_);
                    this->add(stop);
                  }
            }

    this->updateBoundingRect();
    this->addVertical(start_bound.left(),width,routeLayer_);
    }


    void Route::routeLeft(){
      Rules * rules = Rules::getRules();
      int width = rules->get(routeLayer_,"width");
      int space = rules->get(routeLayer_,"space");

      Rect start_bound = this->calcBoundingRect(start_rects_);
      Rect stop_bound = this->calcBoundingRect(stop_rects_);

      int hgrid = this->rules->get("ROUTE","horizontalgrid");
      start_bound.adjust(0,0,space +  hgrid*track_,0);

      //- Extend left rectangles
      foreach(Rect *r, start_rects_){
          Rect * start = new Rect(routeLayer_,r->x1(),r->y1(),start_bound.right() - r->x1(),width);
          if(start){
              this->applyOffset(width, start,startOffset_);
              this->add(start);
            }
      }

      //- Extend right rectangles
            foreach(Rect *r, stop_rects_){
                Rect * stop ;
                if(r->x1() > start_bound.right()){
                   stop =  new Rect(routeLayer_,start_bound.right(),r->y1(), r->x1()-start_bound.right() ,width);
                  }else{
                    stop = new Rect(routeLayer_,r->x1(),r->y1(),start_bound.right() - r->x1(),width);
                  }
                if(stop){
                    this->applyOffset(width, stop,stopOffset_);
                    this->add(stop);
                  }
            }

    this->updateBoundingRect();
    this->addVertical(start_bound.right(),width,routeLayer_);
    }

    void Route::addVertical(int x, int width, QString layer){

        if(this->options_.contains(QRegularExpression("novert"))) return;
        if(this->options_.contains(QRegularExpression("antenna"))){
//            QString next = t

//            my $nextlayer = $self->rules->getNextLayer($self->rules->getNextLayer($layer));
//            my $c1 = new Gds::GdsFixedContact($layer,$nextlayer,1,2);
//            my $c2 = new Gds::GdsFixedContact($layer,$nextlayer,1,2);


//            my $r ;
//            if($self->height > $c1->height*2 + $mw*2){
//              $r = new Gds::GdsRect($nextlayer,$xm,$self->yc,$mw,$self->height);
//              $c1->moveTo($xm,$self->yc);
//              $c2->moveTo($xm,$r->{y2} - $c2->height);
//              $self->addChild($c1,$c2);
//            }else{
//              $r = new Gds::GdsRect($layer,$xm,$self->yc,$mw,$self->height);

//            }

//            $self->addChild($r);
//            $self->portrect($r);
          }else{

             Rect * r = new Rect(layer,x,this->y1(),width,this->height());
             this->add(r);
          }
      }


    void Route::routeVertical(){

      if( !(start_rects_.count() > 0 && stop_rects_.count() > 0)) return;

        Rect start_bound = Cell::calcBoundingRect(start_rects_);
        Rect stop_bound = Cell::calcBoundingRect(stop_rects_);
        int width = this->rules->get(routeLayer_,"width");

        int xc = start_bound.centerX() - width/2;
        int yc = start_bound.y1();
        int height = stop_bound.y2() - yc;


        Rect *r = new Rect(routeLayer_,xc,yc,width,height);
        this->add(r);


        }

    void Route::routeStraight(){
	   if(start_rects_.count() != stop_rects_.count()){
	      qWarning() << "Can't route straight ";
	       return;
	     }
	   
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
	     }
		
	}

	


}
