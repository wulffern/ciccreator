/*********************************************************************
 *        Copyright (c) 2010 Carsten Wulff Software, Norway
 * *******************************************************************
 * Created       : wulff at 2010-9-1
 * Modified      : $Author$ $Date$
 * Version       : $Revision$ $HeadURL$
 * Description   :
 ********************************************************************/

#include "capturescene.h"


CaptureScene::CaptureScene(QObject *parent):QGraphicsScene(parent){
  scenemode_ = DefaultMode;
    addModifier(AddCommentLine,new SceneAddCommentLine(this));
  addModifier(AddNet,new SceneAddNet(this));
  //addModifier(AddCommentText,new SceneAddCommentText(this));
  //addModifier(AddSymbol,new SceneAddSymbol(this));
//  dataSource_ = new GObject();
  m_undoStack = new QUndoStack();
}

void CaptureScene::addModifier(SceneMode mode,SceneModifier * modifier){
  connect(modifier,SIGNAL(newUndoCommand(QUndoCommand*)),this,SLOT(pushUndoCommand(QUndoCommand *)));
  modifiers_.insert(mode,modifier);
  }

CaptureScene::~CaptureScene(){
}

void CaptureScene::pushUndoCommand(QUndoCommand * c){
  
  m_undoStack->push(c);
}

QUndoStack * CaptureScene::undoStack(){
  return m_undoStack;
}

void CaptureScene::setSceneMode(SceneMode sc){
    scenemode_ = sc;
    if(modifiers_.contains(sc)){
      if(modifier_ != 0){
	modifier_->cancel();
      }
      modifier_ = modifiers_.value(sc);
      modifier_->run();
    }else{
    modifier_ = 0;
  }  
}

void CaptureScene::mousePressEvent ( QGraphicsSceneMouseEvent * mouseEvent ){
  
  if(modifier_ != 0){
      modifier_->mousePressEvent(mouseEvent);
  }else{
        QGraphicsScene::mousePressEvent(mouseEvent);
  }
  
}


void CaptureScene::mouseMoveEvent ( QGraphicsSceneMouseEvent * mouseEvent ){
    if(modifier_ != 0){
    modifier_->mouseMoveEvent(mouseEvent);
  }else{
    QGraphicsScene::mouseMoveEvent(mouseEvent);
  }
  
}

void CaptureScene::mouseReleaseEvent ( QGraphicsSceneMouseEvent * mouseEvent ){
    if(modifier_ != 0){
    modifier_->mouseReleaseEvent(mouseEvent);
  }else{
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
  }
  
}

void CaptureScene::mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * mouseEvent ){
    if(modifier_ != 0){
    modifier_->mouseDoubleClickEvent(mouseEvent);
  }else{
    QGraphicsScene::mouseDoubleClickEvent(mouseEvent);
  }
  
}



void CaptureScene::addGObject(GObject * g){
  populateScene(0,0,g,false);
}


void CaptureScene::populateScene(QGraphicsItemGroup * group,QGraphicsItem * parent,GObject * g, bool isInSymbol){

	if(g == 0)
	  return;
	
	QGraphicsItem * gi = getItem(g,isInSymbol);
	if(gi != 0){
	  addItem(gi);
		if(parent != 0){
			gi->setParentItem(parent);

		}	  
		
		if(group != 0){
			group->addToGroup(gi);
		}
		
		if(g->type == OBJ_COMPLEX){
			GComponent * c = (GComponent *) g;
			populateScene(0,gi,c->symbol,true);
			group = (QGraphicsItemGroup *) gi;
		}
		
	}	

	foreach(GObject *child, g->children()){
	  populateScene(group,parent,child,isInSymbol);  
	}

}

QGraphicsItem * CaptureScene::getItem(GObject *g, bool isInSymbol){
      QGraphicsItem  * gi = 0;
      if(g == 0){
        return gi;
      }
      else if(g->type == OBJ_VERSION){
	return 0;
      }
      else if(g->type == OBJ_LINE){
        gi =  new CLine((GLine *) g, isInSymbol);
      }
      else if(g->type == OBJ_NET){
        gi =  new CNet((GNet *) g);
      }
      else if(g->type == OBJ_PIN){
        gi = new CPin((GPin *) g, isInSymbol);
      }
      else if(g->type == OBJ_TEXT){
        gi = new CText((GText *) g, isInSymbol);
      }
      else if(g->type == OBJ_BOX){
        gi = new CBox((GBox *) g,isInSymbol);
      }
      else if(g->type == OBJ_COMPLEX){
	gi =  new CSymbol((GComponent *)g);
      }
      else {
	//        qDebug() << "gObject type " << g->type;
      }
      
      return gi;
    }


void CaptureScene::drawForeground(QPainter* painter, const QRectF& rect)
{

    int gridInterval = G_GRID; //interval to draw grid lines at
	painter->setWorldMatrixEnabled(true);
	painter->setPen(QPen(Qt::blue,1,Qt::SolidLine));
    
	qreal left = int(rect.left()) - (int(rect.left()) % gridInterval );
	qreal top = int(rect.top()) - (int(rect.top()) % gridInterval );

	int gridcount = int((rect.bottom() - top)/gridInterval/400);

	if(gridcount == 0){
	QVarLengthArray<QPointF, 100> linesX;
	for (qreal y = top; y < rect.bottom(); y += gridInterval ){
	  for (qreal x = left; x < rect.right(); x += gridInterval ){
	    painter->drawPoint(x,y);
	  }
	}
	}	
}
