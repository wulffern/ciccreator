//====================================================================
//        Copyright (c) 2010 Carsten Wulff Software, Norway 
// ===================================================================
// Created       : wulff at 2010-10-1
// Modified      : $Author$ $Date$
// Version       : $Revision$ $HeadURL$
// Description   :  
//====================================================================
#include "gmath.h"

//********************************************************
// GMath
//********************************************************
QPointF snapToGrid(QPointF p){
  int x = qRound(p.x()/G_GRID)*G_GRID;
  int y = qRound(p.y()/G_GRID)*G_GRID;
  p.setX(x);
  p.setY(y);
  return p;
}
