//====================================================================
//        Copyright (c) 2010 Carsten Wulff Software, Norway
// ===================================================================
// Created       : wulff at 2010-9-22
// Modified      : $Author$ $Date$
// Version       : $Revision$ $HeadURL$
// Description   :
//====================================================================

#include "captureitem.h"

//****************************************************************
// CaptureItem
//****************************************************************
CaptureItem::CaptureItem(GObject* dataSource,bool isInSymbol){
    isInSymbol_ = isInSymbol;
    dataSource_ = dataSource;
    doMove = false;
    m_pen = QPen(Qt::black, 1, Qt::SolidLine);

}
CaptureItem::~CaptureItem(){
}

GObject * CaptureItem::dataSource(){
    return dataSource_;
}

void    CaptureItem::paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget ) {
    Q_UNUSED(widget)
        Q_UNUSED(option)
        QRectF bb = boundingRect();
    if(!isInSymbol_){
        //    painter->setBrush(Qt::NoBrush);
        //    QPen pen(Qt::green,1,Qt::DotLine);
        //    painter->setPen(pen);
        //    painter->drawRect(bb);
    }
}



//****************************************************************
// CBox
//****************************************************************
CBox::CBox(GBox * data,bool isInSymbol):CaptureItem(data,isInSymbol){
    d = QRectF(data->x,data->y,data->width,data->height);
    GColors * cols = GColors::instance();
    m_color = cols->getColor(data->color);
    m_pen = QPen(m_color, data->linewidth, Qt::SolidLine);

    setFlag(QGraphicsItem::ItemIsSelectable);
    //  setFlag(QGraphicsItem::ItemIsMovable);
    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
}

CBox::~CBox(){
}

QRectF  CBox::boundingRect() const{
    QRectF r = d.adjusted(-m_pen.width(),-m_pen.width(),m_pen.width(),m_pen.width());
    r.moveCenter(d.center());
    return r;
}

void CBox::setBox(int x1, int y1,int x2, int y2){
    prepareGeometryChange();
    d.setCoords(x1,y1,x2,y2);
    update();
}

void CBox::paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget ) {
    Q_UNUSED(option)
        Q_UNUSED(widget)
        painter->setPen(m_pen);
    painter->drawRect(d);
}

//****************************************************************
// CLine
//****************************************************************
CLine::CLine(GLine * data,bool isInSymbol):CaptureItem(data,isInSymbol){
    d = data;

    GColors * cols = GColors::instance();
    QColor col = cols->getColor(d->color);
    m_pen = QPen(col, d->width, Qt::SolidLine);

    if(!isInSymbol){
        setFlag(QGraphicsItem::ItemIsSelectable);
        //    setFlag(QGraphicsItem::ItemIsMovable);
    }
}

CLine::~CLine(){
}

QLineF CLine::line() const{
    return d->line;
}

void CLine::changeEnd(QPointF p){
    prepareGeometryChange();
    d->line.setP2(p);
    update();
}

QRectF  CLine::boundingRect() const{
    const qreal x1 = d->line.p1().x();
    const qreal x2 = d->line.p2().x();
    const qreal y1 = d->line.p1().y();
    const qreal y2 = d->line.p2().y();
    qreal lx = qMin(x1, x2);
    qreal rx = qMax(x1, x2);
    qreal ty = qMin(y1, y2);
    qreal by = qMax(y1, y2);
    return QRectF(lx, ty, rx - lx, by - ty);
}

void CLine::paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget) {
    Q_UNUSED(widget)

        painter->setPen(m_pen);
    painter->drawLine(d->line);
    CaptureItem::paint(painter,option,widget);
}


//****************************************************************
// CPin
//****************************************************************
CPin::CPin(GPin * data,bool isInSymbol):CaptureItem(data,isInSymbol){
    data_ = data;
    size_ = 6;
    drawPin_ = true;
}

CPin::~CPin(){

}

QRectF CPin::boundingRect() const{

    int x = 0;
    int y = 0;
    int width = data_->x2-data_->x1;
    int height = data_->y2-data_->y1;

    if(width < 0){
        x = width;
        width = qAbs(width);
    }

    if(height < 0){
        y = height;
        height = qAbs(height);
    }

    QRectF b(x,y,width,height);

    //      if(data_->whichend == 0){
    b.adjust(-size_/2,-size_/2,size_/2,size_/2);
    //      }else{
    //  b.adjust(0,0,size_/2,size_/2);
    //      }
    return b;
}

void CPin::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                 QWidget *widget){
    Q_UNUSED(option)
        Q_UNUSED(widget)
        int x_ = data_->x2-data_->x1;
    int y_ = data_->y2-data_->y1;
    GColors * cols = GColors::instance();
    QColor col = cols->getColor(data_->color);

    QPen pen(col, 1, Qt::SolidLine);

    setPos(data_->x1,data_->y1);
    painter->setPen(pen);
    painter->drawLine(0,0,x_,y_);

    if(drawPin_){

        painter->setPen(Qt::red);
        painter->setBrush(Qt::NoBrush);

        int x,y;
        if(data_->whichend == 0){
            x = 0;
            y = 0;
        }else{
            x = x_;
            y = y_;
        }

        QRectF r(0,0,size_,size_);
        r.moveCenter(QPointF(x,y));
        painter->setBrush(Qt::red);
        painter->drawRect(r);


        //    painter->drawLine(x-size_,y,x,y+size_);
        //    painter->drawLine(x+size_,y,x,y+size_);
        //    painter->drawLine(x,y-size_,x+size_,y);
        //    painter->drawLine(x,y-size_,x-size_,y);
    }

    CaptureItem::paint(painter,option,widget);
}

//****************************************************************
// CNet
//****************************************************************
CNet::CNet(GNet * data){
    d = data;
    //   setMin(data_->x1,data_->y1,data_->x2,data_->y2);
    reloadData();
    setFlag(QGraphicsItem::ItemIsSelectable);
    //    setFlag(QGraphicsItem::ItemIsMovable);
    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
}

CNet::~CNet(){

}

void CNet::reloadData(){
    GColors * cols = GColors::instance();
    setLine(d->line);

    QColor col = cols->getColor(d->color);
    QPen pen(col, 1, Qt::SolidLine);
    setPen(pen);
}


//TODO:: Load symbols

//****************************************************************
// CSymbol
//****************************************************************
CSymbol::CSymbol(GComponent *data):CaptureItem(data,false){
    data_ = data;
    reloadData();
    //      setFlag(QGraphicsItem::ItemIsMovable);
    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
}

CSymbol::~CSymbol(){

}

QRectF CSymbol::boundingRect() const{
    return QGraphicsItemGroup::boundingRect();
}

QString CSymbol::name(){
    return data_->basename;
}

void CSymbol::paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget) {
    Q_UNUSED(widget)
 QRectF r(0,0,100,100);
    r.moveTo(QPointF(data_->x,data_->y));
    painter->setBrush(Qt::red);
    painter->drawRect(r);
    CaptureItem::paint(painter,option,widget);
}

void CSymbol::reloadData(){

    setPos(data_->x,data_->y);
    

    if(data_->mirror == 1){
        QTransform trans;
        trans.scale(1,-1);
        setTransform(trans);
    }

    switch(data_->angle){
    case 0:
        break;
    case 90:
        setRotation(90.0);
        break;
    case 180:
        setRotation(180.0);
        break;
    case 270:
        setRotation(270.0);
        break;
    }

    if(data_->selectable == 1){
        setFlag(QGraphicsItem::ItemIsSelectable);
        setFlag(QGraphicsItem::ItemIsMovable);
    }
}

//****************************************************************
// CText
//****************************************************************
CText::CText(GText * data,bool isInSymbol){
    Q_UNUSED(isInSymbol)
        data_ = data;
    size_multiplier_ = 1;
    //  if(!isInSymbol){
    reloadData();
    //  }
    setFlag(QGraphicsItem::ItemIsSelectable);
    //  setFlag(QGraphicsItem::ItemIsMovable);
    //  setFlag(QGraphicsItem::ItemIgnoresTransformations);
    setFlag(QGraphicsItem::ItemIsFocusable);
    // QTransform trans;
//  trans.scale(1,-1);
//  setTransform(trans);

//TODO: figure out   scale(1,-1);
    //  setCacheMode(QGraphicsItem::DeviceCoordinateCache);
}

CText::~CText(){

}

void CText::reloadData(){

    //Set text
    setText(data_->filteredStrings.join("\n"));
    GColors * cols = GColors::instance();
    QColor col = cols->getColor(data_->color);
    setBrush(col);
    QFont fo("Helvetica", data_->size*size_multiplier_);
    setFont(fo);

    //Calculate position
    int x,y;
    x = data_->x;
    y = data_->y;
    int size = data_->size;

    QRectF rect= boundingRect();
    switch(data_->alignment){
    case(LOWER_LEFT):
        y = y+size;
        break;
    case MIDDLE_LEFT:
        y = y+ size/2;
        break;
    case UPPER_LEFT:
        break;
    case LOWER_MIDDLE:
        y = y + size;
        x = x - rect.width()/2;
        break;
    case MIDDLE_MIDDLE:
        y = y + size/2;
        x = x - rect.width()/2;
        break;
    case UPPER_MIDDLE:
        x = x - rect.width()/2;
        break;
    case LOWER_RIGHT:
        y = y + size;
        x = x - rect.width();
        break;
    case MIDDLE_RIGHT:
        y = y + size;
        x = x - rect.width();
        break;
    case UPPER_RIGHT:
        x = x - rect.width();
        break;

    }
    setPos(x,y);
    QTransform trans;
    trans.scale(1,-1);
    setTransform(trans,true);
    //   setFlag(QGraphicsItem::ItemIgnoresTransformations);
}

void CText::addText(QString & str){
    data_->strings.append(str);
    data_->filterStrings();
    reloadData();
}
