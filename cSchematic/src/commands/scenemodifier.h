/*********************************************************************
 *        Copyright (c) 2010 Carsten Wulff Software, Norway 
 * *******************************************************************
 * Created       : wulff at 2010-9-18
 * Modified      : $Author$ $Date$
 * Version       : $Revision$ $HeadURL$
 * Description   :  
 ********************************************************************/

#ifndef SCENEMODIFIER_H
#define SCENEMODIFIER_H

#include <QObject>
#include <QGraphicsLineItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QUndoCommand>
#include "commands/commands.h"
#include "geda/gobject.h"
#include "items/captureitem.h"
#include "math/gmath.cpp"


  //#########################################################
  // SceneModifier
  //########################################################
 class SceneModifier: public QObject
{
  Q_OBJECT
 private:

  
 protected:
    QGraphicsScene * scene_;
    GObject * dataSource_;
    bool isStarted;
    bool isContinuous;
    //    bool oneShotMode_;
    int count_;
    QPointF m_start;
    QPointF m_end;
public:
    //--------------------------------------------------------
    // ctor and dtor
    //--------------------------------------------------------
    SceneModifier(QGraphicsScene * scene);    
    ~SceneModifier();

    
    //    void reset();

    virtual void run();
    virtual void cancel() = 0;
    //    bool isDoneExecuting();

    //Property oneShotMode 
    //    bool oneShotMode();
    ///    void setOneShotMode(bool value);

    GObject * dataSource();
    void setDataSource(GObject * dataSource);


    //Property count 
    int count();
    void setCount( int count);


    virtual void mousePressEvent ( QGraphicsSceneMouseEvent * mouseEvent );
    virtual void mouseMoveEvent ( QGraphicsSceneMouseEvent * mouseEvent );
    virtual void mouseReleaseEvent ( QGraphicsSceneMouseEvent * mouseEvent );
    virtual void mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * mouseEvent );

    virtual void start() = 0;
    virtual void updatePosition(QPointF point) = 0;
    virtual void end() = 0;

 signals:
    void newGEdaObject(GObject * g,bool addToScene); 
    void newUndoCommand(QUndoCommand *c);
};

  //********************************************************
  // SceneAddCommentLine
  //********************************************************
  class SceneAddCommentLine : public SceneModifier
  {
    Q_OBJECT
  private:

    CLine * m_line;
    QPointF prev;

  public:
    SceneAddCommentLine(QGraphicsScene * scene);
    ~SceneAddCommentLine();
    virtual void cancel();
  protected:

    virtual void start();
    virtual void updatePosition(QPointF point);
    virtual void end();
    
  };

  //********************************************************
  // SceneAddNet
  //********************************************************
  class SceneAddNet : public SceneModifier
  {
    Q_OBJECT
  private:
    QGraphicsLineItem *l1;
    QGraphicsLineItem *l2;
    QPointF p1;
    QPointF p2;
    QPointF p3;
    bool lockHorizontalP2;
    bool lockVerticalP2;
  public:
    SceneAddNet(QGraphicsScene * scene);
    ~SceneAddNet();
    virtual void cancel();


  protected:
    virtual void start();
    virtual void updatePosition(QPointF point);
    virtual void end();
    
  };


#endif // SCENEMODIFIER
