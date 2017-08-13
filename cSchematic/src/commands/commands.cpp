/*********************************************************************
 *        Copyright (c) 2010 Carsten Wulff Software, Norway
 * *******************************************************************
 * Created       : wulff at 2010-9-24
 * Modified      : $Author$ $Date$
 * Version       : $Revision$ $HeadURL$
 * Description   :
 ********************************************************************/
#include "commands.h"

//********************************************************
// CmdAddCommentLine
//********************************************************
CmdAddCommentLine::CmdAddCommentLine(QGraphicsScene *scene, CLine *line){
  m_line = line;
  m_scene = scene;
  start = line->line().p1();
  end = line->line().p2();
}

int CmdAddCommentLine::id () const{
  return 1;
}

CmdAddCommentLine::~CmdAddCommentLine(){}

void CmdAddCommentLine::undo(){
  m_scene->removeItem(m_line);
  delete m_line;
  m_line = 0;
}

void CmdAddCommentLine::redo(){
  GLine *g = GLine::commentLine(start,end);
  m_line = new CLine(g);
  m_scene->addItem(m_line);
}


//********************************************************
// CmdAddNets
//********************************************************
CmdAddNets::CmdAddNets(QGraphicsScene *scene, QList<QPointF> & points){
  m_points = points;
  m_scene = scene;
}

int CmdAddNets::id () const{
  return 2;
}

CmdAddNets::~CmdAddNets(){}

void CmdAddNets::undo(){
  foreach(CNet * n, m_nets){
    m_scene->removeItem(n);
  }

  m_nets.clear();
}

void CmdAddNets::redo(){

  if(m_points.count() < 2)
    return;

  GNet *n;
  CNet *cn;
  for(int i=1;i < m_points.count();i++){
    n = GNet::newNet(m_points[i-1],m_points[i]);
    cn = new CNet(n);
    m_scene->addItem(cn);
    m_nets.append(cn);
  }
}


