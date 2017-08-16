/*********************************************************************
 *        Copyright (c) 2010 Carsten Wulff Software, Norway
 * *******************************************************************
 * Created       : wulff at 2010-9-13
 * Modified      : $Author$ $Date$
 * Version       : $Revision$ $HeadURL$
 * Description   :
 ********************************************************************/

#ifndef _COLORS_H
#define _COLORS_H

#include "defines/defines.h"

  class GColors
  {

  private:
    GColors(){
      colors_.insert(BACKGROUND_COLOR,Qt::green);
      colors_.insert(PIN_COLOR,Qt::green);
      colors_.insert(NET_ENDPOINT_COLOR,Qt::darkRed);
      colors_.insert(GRAPHIC_COLOR,Qt::green);
      colors_.insert(NET_COLOR,QColor(0,100,0));
      colors_.insert(ATTRIBUTE_COLOR,Qt::gray);
      colors_.insert(LOGIC_BUBBLE_COLOR,Qt::gray);
      colors_.insert(DOTS_GRID_COLOR,Qt::gray);
      colors_.insert(DETACHED_ATTRIBUTE_COLOR,Qt::gray);
      colors_.insert(TEXT_COLOR,Qt::green);
      colors_.insert(BUS_COLOR,Qt::yellow);
      colors_.insert(SELECT_COLOR,Qt::white);
      colors_.insert(BOUNDINGBOX_COLOR,Qt::white);
      colors_.insert(ZOOM_BOX_COLOR,Qt::white);
      colors_.insert(STROKE_COLOR,Qt::red);
      colors_.insert(LOCK_COLOR,Qt::white);
      colors_.insert(OUTPUT_BACKGROUND_COLOR,Qt::white);
      colors_.insert(FREESTYLE1_COLOR,Qt::white);
      colors_.insert(FREESTYLE2_COLOR,Qt::white);
      colors_.insert(FREESTYLE3_COLOR,Qt::white);
      colors_.insert(FREESTYLE4_COLOR,Qt::white);
      colors_.insert(JUNCTION_COLOR,Qt::white);
      colors_.insert(MESH_GRID_MAJOR_COLOR,Qt::gray);
      colors_.insert(MESH_GRID_MINOR_COLOR,Qt::gray);
      //      colors_.insert(OVERRIDE_NET_COLOR,Qt::white);
      //      colors_.insert(OVERRIDE_PIN_COLOR,Qt::white);
      //      colors_.insert(OVERRIDE_PIN_COLOR,Qt::white);
    }
    //  static GColors * myinst;
    QMap<int,QColor> colors_;

  public:
    //--------------------------------------------------------
    // ctor and dtor
    //--------------------------------------------------------
    ~GColors(){

    }

    QColor getColor(int col){
      if(colors_.contains(col)){
        return colors_.value(col);
      }
      return colors_.value(DEFAULT_COLOR);
    }

    static GColors * instance(){
      //      if(myinst == 0){
      GColors * myinst = new GColors();
      //      }
      return myinst;
    }


  };

  // GColors * GColors::myinst = 0;


#endif // COLORS
