/*********************************************************************
 *        Copyright (c) 2010 Carsten Wulff Software, Norway
 * *******************************************************************
 * Created       : wulff at 2010-9-1
 * Modified      : $Author$ $Date$
 * Version       : $Revision$ $HeadURL$
 * Description   :
 ********************************************************************/

#ifndef CAPTURESCENE_H
#define CAPTURESCENE_H

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QPointF>
#include <QMap>
#include <QVarLengthArray>
#include <QLineF>
#include <QDebug>
#include "geda/gobject.h"
#include "items/captureitem.h"
#include "commands/scenemodifier.h"
#include <QUndoStack>
#include <QUndoCommand>

class QString;


class CaptureScene: public QGraphicsScene
{
 Q_OBJECT

   public:
  enum SceneMode {AddCommentLine, AddCommentBox, AddNet, AddCommentText, AddSymbol,DefaultMode};
   private:
  SceneMode scenemode_;
  SceneModifier * modifier_;
  QUndoStack * m_undoStack;
  QMap<SceneMode,SceneModifier*> modifiers_;

  void addModifier(SceneMode mode,SceneModifier * modifier);


 public:
  CaptureScene(QObject *parent = 0);
  ~CaptureScene();
  void setSceneMode(SceneMode sc);
  QUndoStack * undoStack();
  QGraphicsItem * getItem(GObject *g,bool isInScene = false);
  void populateScene(QGraphicsItemGroup * group,QGraphicsItem * parent,GObject * g,bool isInSymbol = false);
  void drawForeground(QPainter* painter, const QRectF& rect);

 protected:
  void mousePressEvent ( QGraphicsSceneMouseEvent * mouseEvent );
  void mouseMoveEvent ( QGraphicsSceneMouseEvent * mouseEvent );
  void mouseReleaseEvent ( QGraphicsSceneMouseEvent * mouseEvent );
  void mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * mouseEvent );

 public slots:
  void addGObject(GObject * g);
  void pushUndoCommand(QUndoCommand * c);

};
#endif // CAPTURESCENE
