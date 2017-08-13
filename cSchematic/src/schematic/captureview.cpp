/*********************************************************************
 *        Copyright (c) 2010 Carsten Wulff Software, Norway
 * *******************************************************************
 * Created       : wulff at 2010-9-2
 * Modified      : $Author$ $Date$
 * Version       : $Revision$ $HeadURL$
 * Description   :
 ********************************************************************/
#include "captureview.h"

CaptureView::CaptureView(QWidget *parent):QGraphicsView(parent){
  scene_ = new CaptureScene(0);
  scene_->setSceneRect(QRectF(0,0 ,G_X_MAX,G_Y_MAX));
  scene_->setSceneMode(CaptureScene::DefaultMode);
	scene_->setBackgroundBrush(Qt::white);
	//setDragMode(QGraphicsView::RubberBandDrag);
    setRenderHints(QPainter::Antialiasing
                            | QPainter::TextAntialiasing);
  //  scene_->setForegroundBrush(QBrush(Qt::lightGray, Qt::CrossPattern));
  scale_ = 0.5;
  scale(scale_,scale_);
  setScene(scene_);
	setMouseTracking(true);
  //  setCacheMode(QGraphicsView::CacheBackground);

#ifdef OPENGL_RENDERING
  setViewport(new QGLWidget(QGLFormat(QGL::DoubleBuffer)));
#endif
  scale(1,-1);
}

CaptureView::~CaptureView(){

}

CaptureScene * CaptureView::scene(){
  return scene_;
}

void CaptureView::openFile(QString & file){
  gEdaFileReader gdf;
  GObject *g = gdf.readFile(file);
  scene_->clear();
  
  scene_->addGObject(g);
  QRectF r = scene_->itemsBoundingRect();
  fitInView(r,Qt::KeepAspectRatio);
}


void CaptureView::zoomOut(){
  scale(0.5,0.5);

}

void CaptureView::zoomIn(){
  scale(2,2);

}
