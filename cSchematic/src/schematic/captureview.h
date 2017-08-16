/*********************************************************************
 *        Copyright (c) 2010 Carsten Wulff Software, Norway 
 * *******************************************************************
 * Created       : wulff at 2010-9-2
 * Modified      : $Author$ $Date$
 * Version       : $Revision$ $HeadURL$
 * Description   :  
 ********************************************************************/

#ifndef CAPTUREVIEW_H
#define CAPTUREVIEW_H

#include <QWidget>
#include <QGraphicsView>
#include "geda/gEdaFileReader.h"
#include "capturescene.h"
#include <QFileDialog>
#include <QDebug>
//#include "qgl.h"


class CaptureView : public QGraphicsView
{
  Q_OBJECT
private:
  CaptureScene * scene_;
  float scale_;

public:
    //--------------------------------------------------------
    // ctor and dtor
    //--------------------------------------------------------
  CaptureView(QWidget *parent=0);
  ~CaptureView();

  void openFile(QString & file);
    void writeFile(QString & file);
    

  CaptureScene * scene();

 public slots:
  void zoomIn();
  void zoomOut();

};


#endif // CAPTUREVIEW
