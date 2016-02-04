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
        if(options.contains(QRegularExpression("onTopR"))){ sortDirection_ = SORT_RIGHT ;}
        else if(options.contains(QRegularExpression("onTopL"))){ sortDirection_ = SORT_LEFT; }
        else if(options.contains(QRegularExpression("onTopB"))){ sortDirection_ = SORT_BOTTOM; }
        else if(options.contains(QRegularExpression("onTopT"))){ sortDirection_ = SORT_TOP; }

        //- Start offset
        if(options.contains(QRegularExpression("offsethigh(,|\\s+|$)"))){ startOffset_ = HIGH ;}
        else if(options.contains(QRegularExpression("offsetlow(\\s+|,|$)"))){ startOffset_ = LOW ;}

        //- Stop offset;
        if(options.contains(QRegularExpression("offsethighend"))){ stopOffset_ = HIGH ;}
        else if(options.contains(QRegularExpression("offsetlowend"))){ stopOffset_ = LOW ;}

        //- Track
        QRegularExpressionMatch * m = 0;
        if(options.contains(QRegularExpression("track(\\d+)"),m) && m->hasMatch()){
            track_ = m->captured(0).toInt();
        }

        //- Cuts
        if(options.contains(QRegularExpression("(\\d+)cuts"),m) && m->hasMatch()){
            cuts_ = m->captured(0).toInt();
        }

        //- Vertical cuts
        if(options.contains(QRegularExpression("(\\d+)vcuts"),m) && m->hasMatch()){
            vcuts_ = m->captured(0).toInt();
        }


        if(routeType == "-|-"){routeType_ = LEFT;}
        else if(routeType == "-|--"){routeType_ = LEFT;}
        else if(routeType == "--|-"){routeType_ = RIGHT;}
        else if(routeType == "-"){routeType_ = STRAIGHT;}
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
                inst->moveTo(r->x1(),r->y1());
                this->add(inst);
                r->setWidth(inst->width());
                //r->setRect(inst);
                // qDebug() << inst->toString();
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
            break;

		case LEFT_DOWN_LEFT_UP:
			break;

		case STRAIGHT:
			this->routeStraight();
			break;

		case VERTICAL:
			
			break;

        }

    }

    void Route::routeLeft(){
      Rules * rules = Rules::getRules();
      int width = rules->get(routeLayer_,"width");
      int space = rules->get(routeLayer_,"space");

      Rect start_bound = this->calcBoundingRect(start_rects_);

      Rect stop_bound = this->calcBoundingRect(stop_rects_);

      QList<Rect*> allrects;
      allrects.append(new Rect(start_bound));
      allrects.append(new Rect(stop_bound));
      Rect all_bound = this->calcBoundingRect(allrects);

      start_bound.adjust(0,0,space,0);

      //- Extend left rectangles
      foreach(Rect *r, start_rects_){
          Rect * start = new Rect(routeLayer_,r->x2(),r->y1(),start_bound.right() - r->x2(),width);
          this->add(start);
      }

      Rect * vert = new Rect(routeLayer_,start_bound.right(),all_bound.bottom(),width,all_bound.height());
      this->add(vert);

      //- Extend right rectangles
      foreach(Rect *r, stop_rects_){
         Rect * stop = new Rect(routeLayer_,vert->x2(),r->y1(),r->x1() - vert->x2(),width);
         this->add(stop);
      }





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
	    Rect * r = new Rect(routeLayer_, r1->x1(),r1->y1(),r2->x2() - r1->x1(),r1->height());
	    this->add(r);
	     }
		
	}

	


}
