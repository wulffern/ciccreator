/*********************************************************************
 *        Copyright (c) 2010 Carsten Wulff Software, Norway
 * *******************************************************************
 * Created       : wulff at 2010-9-18
 * Modified      : $Author$ $Date$
 * Version       : $Revision$ $HeadURL$
 * Description   :
 ********************************************************************/
#include "scenemodifier.h"

SceneModifier::SceneModifier(QGraphicsScene * scene){
  scene_ = scene;
  count_ = 0;
  isStarted = false;
  isContinuous = false;
}

SceneModifier::~SceneModifier(){

}

void SceneModifier::run(){

}

//Property count
int SceneModifier::count(){ return count_;}
void SceneModifier::setCount( int count){ count_ = count;}


//Property dataSource
GObject * SceneModifier::dataSource(){ return dataSource_;}
void SceneModifier::setDataSource( GObject * dataSource){ dataSource_ = dataSource;}

void SceneModifier::mousePressEvent ( QGraphicsSceneMouseEvent * mouseEvent )
{
  Qt::MouseButton b = mouseEvent->button();
  if(b == Qt::LeftButton){


    if(isStarted && isContinuous){
      m_end  = snapToGrid(mouseEvent->scenePos());
      end();
      isStarted = false;
    }

    if(!isStarted){
      m_start = snapToGrid(mouseEvent->scenePos());
      start();
      isStarted = true;
    }else{
      if(!isContinuous){
        m_end  = snapToGrid(mouseEvent->scenePos());
        end();
        isStarted = false;
      }

    }
  }

};
void SceneModifier::mouseMoveEvent ( QGraphicsSceneMouseEvent * mouseEvent )
{
  updatePosition(mouseEvent->scenePos());

};
void SceneModifier::mouseReleaseEvent ( QGraphicsSceneMouseEvent * mouseEvent )
{
  Q_UNUSED(mouseEvent);
};

void SceneModifier::mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * mouseEvent )
{
  Qt::MouseButton b = mouseEvent->button();
  if(b == Qt::LeftButton){
    if(isContinuous){
      m_end  = snapToGrid(mouseEvent->scenePos());
      end();
      isStarted = false;
    }

  }
};


//********************************************************
// SceneAddCommentLine
//********************************************************
SceneAddCommentLine::SceneAddCommentLine(QGraphicsScene * scene):SceneModifier(scene){
  m_line = 0;
  isContinuous = true;
}

SceneAddCommentLine::~SceneAddCommentLine(){
}

void SceneAddCommentLine::cancel(){
  if(m_line != 0){
    scene_->removeItem(m_line);
    m_line = 0;
  }
}

void SceneAddCommentLine::start(){
  GLine *g = GLine::commentLine(m_start,m_start);
  m_line = new CLine(g);
  scene_->addItem(m_line);
}

void SceneAddCommentLine::updatePosition(QPointF point){
  if(m_line != 0){
    m_line->changeEnd(snapToGrid(point));
  }
}

void SceneAddCommentLine::end(){
  if(m_line != 0){
    if(m_line->line().p1() != m_line->line().p2()){
      CmdAddCommentLine * cac = new CmdAddCommentLine(scene_,m_line);
      emit newUndoCommand(cac);

    }
    scene_->removeItem(m_line);
    m_line = 0;
  }
}

//********************************************************
// SceneAddNet
//********************************************************
SceneAddNet::SceneAddNet(QGraphicsScene * scene):SceneModifier(scene){
  l1 = 0;
  l2 = 0;
  lockHorizontalP2 = false;
  lockVerticalP2 = false;
  isContinuous = true;
}

SceneAddNet::~SceneAddNet(){
}

void SceneAddNet::cancel(){
  if(l1 != 0 && l2 != 0){
    scene_->removeItem(l1);
    scene_->removeItem(l2);
    l1 = 0;
    l2 = 0;
    lockHorizontalP2 = 0;
    lockVerticalP2 = 0;
  }
}

void SceneAddNet::start(){
  l1 = new QGraphicsLineItem(m_start.x(),m_start.y(),m_start.x(),m_start.y());
  l2 = new QGraphicsLineItem(m_start.x(),m_start.y(),m_start.x(),m_start.y());
  p1 = m_start;
  p2 = m_start;
  p3 = m_start;
  scene_->addItem(l1);
  scene_->addItem(l2);
}

void SceneAddNet::updatePosition(QPointF point){
  if(l1 != 0 && l2 != 0){
    QPointF p = snapToGrid(point);

    int xdt = qAbs(m_start.x() - p.x());
    int ydt = qAbs(m_start.y() - p.y());

    if(lockHorizontalP2 || lockVerticalP2)
      {
        if(lockHorizontalP2){
          p2.setX(p.x());
          p2.setY(p1.y());
        }

        if(lockVerticalP2){
          p2.setX(p1.x());
          p2.setY(p.y());
        }

      }
    else
      {
        if(xdt > ydt)
          {
            p2.setX(p.x());
            p2.setY(p1.y());
            if(xdt/G_GRID > 2){
              lockHorizontalP2 = true;
            }
          }
        else{
          p2.setX(p1.x());
          p2.setY(p.y());
          if(ydt/G_GRID > 2){
            lockVerticalP2 = true;
          }
        }
      }

    xdt = qAbs(p2.x()-p.x());
    ydt = qAbs(p2.y()-p.y());


    if(xdt > ydt){
      p3.setX(p.x());
      p3.setY(p2.y());
    }else{
      p3.setX(p2.x());
      p3.setY(p.y());
    }

    l1->setLine(p1.x(),p1.y(),p2.x(),p2.y());
    l2->setLine(p2.x(),p2.y(),p3.x(),p3.y());

  }
}



void SceneAddNet::end(){
  if(l1 != 0 && l2 != 0){
    
    QList<QPointF> points;
    points << p1 << p2 << p3;
    CmdAddNets * cac = new CmdAddNets(scene_,points);
    emit newUndoCommand(cac);
    scene_->removeItem(l1);
    scene_->removeItem(l2);
    l1 = 0;
    l2 = 0;
    lockHorizontalP2 = 0;
    lockVerticalP2 = 0;
  }
}


