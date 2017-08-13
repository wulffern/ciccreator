/*********************************************************************
 *        Copyright (c) 2010 Carsten Wulff Software, Norway
 * *******************************************************************
 * Created       : wulff at 2010-9-24
 * Modified      : $Author$ $Date$
 * Version       : $Revision$ $HeadURL$
 * Description   :
 ********************************************************************/

#ifndef _COMMANDS_H
#define _COMMANDS_H

#include <QGraphicsScene>
#include <QUndoCommand>
#include <QPointF>
#include <QDebug>
#include "items/captureitem.h"
#include "lib/gobject.h"

//********************************************************
// CmdAddCommentLine
//********************************************************
class CmdAddCommentLine: public QUndoCommand
{
 private:
  CLine * m_line;
  QGraphicsScene * m_scene;
  QPointF start;
  QPointF end;
 public:
  CmdAddCommentLine(QGraphicsScene *scene, CLine *line);
  ~CmdAddCommentLine();

  int id () const;
  virtual void undo();
  virtual void redo();

};

//********************************************************
// CmdAddNets
//********************************************************
class CmdAddNets: public QUndoCommand
{
 private:
  QList<CNet*> m_nets;
  QGraphicsScene * m_scene;
  QList<QPointF> m_points;
 public:
  CmdAddNets(QGraphicsScene *scene, QList<QPointF> &points);
  ~CmdAddNets();

  int id () const;
  virtual void undo();
  virtual void redo();

};

#endif
