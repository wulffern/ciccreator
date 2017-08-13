/*********************************************************************
 *        Copyright (c) 2010 Carsten Wulff Software, Norway
 * *******************************************************************
 * Created       : wulff at 2010-9-22
 * Modified      : $Author$ $Date$
 * Version       : $Revision$ $HeadURL$
 * Description   :
 ********************************************************************/

#ifndef CAPTUREITEM_H
#define CAPTUREITEM_H

#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItemGroup>
#include <QPainter>
#include <QDebug>
#include <QPen>
#include "geda/gobject.h"
#include "geda/gcolors.h"

  //****************************************************************
  // CaptureItem
  //****************************************************************
 class CaptureItem: public QGraphicsItemGroup
  {

  private:
    bool isInSymbol_;
    GObject * dataSource_;
    QPointF lastPos;
    bool doMove;
  protected:
    QPen m_pen;
  public:
    CaptureItem(GObject * dataSource,bool isInSymbol);
    ~CaptureItem();

    GObject * dataSource();
    virtual QRectF boundingRect() const = 0;

    void        paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );

    void isInSymbol();




  public slots:

  protected:

  };

  //****************************************************************
  // CBox
  //****************************************************************
 class CBox: public CaptureItem
  {

  private:
    //    gEdaBox * d;
    QRectF d;
    QColor m_color;
    QPen m_pen;

  public:
    CBox(GBox * data,bool isInSymbol);
    ~CBox();
    QRectF boundingRect() const;
    void setBox(int x1, int y1,int x2, int y2);
    void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );

  };

  //****************************************************************
  // CLine
  //****************************************************************
 class CLine: public CaptureItem
  {

  private:
    GLine * d;
  protected:

  public:
    CLine(GLine * data,bool isInSymbol = false);
    ~CLine();
    void changeEnd(QPointF p);
    QLineF line() const;
    QRectF boundingRect() const;
    void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );

  };

  //****************************************************************
  // CPin
  //****************************************************************
   class CPin: public CaptureItem
  {

  private:
	  GPin * data_;
    int size_;
    bool drawPin_;

  public:
    CPin(GPin * data,bool isInSymbol = false);
    ~CPin();
    QRectF boundingRect() const;
	  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
				 QWidget *widget = 0);
	  
  };


  //****************************************************************
  // CNet
  //****************************************************************
 class CNet: public QGraphicsLineItem
{

 private:
 GNet * d;
 protected:

 public:
 CNet(GNet * data);
	~CNet();

    void reloadData();

};

//****************************************************************
  // CSymbol
  //****************************************************************
  class CSymbol: public CaptureItem
  {

  private:
    GComponent *data_;
  public:
    //--------------------------------------------------------
    // ctor and dtor
    //--------------------------------------------------------
    CSymbol(GComponent *data);
    ~CSymbol();
    QString name();
    void reloadData();
    QRectF boundingRect() const;
  protected:

  };

//****************************************************************
  // CText
  //****************************************************************
class CText: public QGraphicsSimpleTextItem
{

 protected:
  GText * data_;
  int size_multiplier_;
 public:
  CText(GText * data,bool isInSymbol = false);
  ~CText();

  void reloadData();
  void addText(QString & str);
};

#endif // CAPTUREITEM
