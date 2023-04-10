
//====================================================================
//        Copyright (c) 2017 Carsten Wulff Software, Norway
// ===================================================================
// Created       : wulff at 2017-1-30
// ===================================================================
//   This program is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation, either version 3 of the License, or
//   (at your option) any later version.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with this program.  If not, see <http://www.gnu.org/licenses/>.
//====================================================================

#include "widget.h"

QPixmap Widget::getIcon(Layer *l){
    return QPixmap::fromImage(drawColorIconProof(l->color, icon(200)));
}

QImage Widget::icon(int size) {
    QImage image(size, size, QImage::Format_ARGB32_Premultiplied);
    image.fill(Qt::transparent);
    QPainter p(&image);
    p.setRenderHint(QPainter::Antialiasing);
    p.setPen(Qt::NoPen);
    p.translate(image.rect().center());
    p.scale(image.width()/2.2, image.height()/2.2);
    p.setBrush(Qt::white);
    p.drawRect(0,0,image.width(),image.height());
    return image;
}

void Widget::drawColorIcon(QPainter & p, QColor color, const QImage & alpha)
{
    p.save();
    p.setCompositionMode(QPainter::CompositionMode_SourceOver);
    p.fillRect(QRect(0, 0, alpha.width(), alpha.height()), color);
    p.setCompositionMode(QPainter::CompositionMode_DestinationIn);
    p.drawImage(0, 0, alpha);
    p.restore();
}

QImage Widget::drawColorIconProof(QColor color, const QImage & alpha) {
    QImage result(alpha.size(), alpha.format());
    QPainter p(&result);
    drawColorIcon(p, color, alpha);
    p.setPen(Qt::NoPen);
    QBrush brush;
    //brush.setTextureImage(checkers(10));
    p.setCompositionMode(QPainter::CompositionMode_DestinationAtop);
    p.fillRect(alpha.rect(), brush);
    return result;
}

Widget::Widget( QWidget *parent)
    : QWidget(parent)
{
    qApp->installEventFilter(this);
    cell = new Cell();
    cellPainter = new CellPainter();
    ycenter = 0;
    xcenter = 0;
    step = 100;

    QPalette pal = palette();

// set black background
    pal.setColor(QPalette::Window, Qt::white);
    this->setAutoFillBackground(true);
    this->setPalette(pal);

    //setAttribute(Qt::WA_PaintOnScreen);
    //setAttribute(Qt::WA_NoSystemBackground);



}

void Widget::update()
{
//    qDebug() << visibleArea.zoom << visibleArea.rect.toString();
    QWidget::update();
}



void Widget::zoomOut()
{
    visibleArea.zoom /= 1.2;
//    step /=1.2;
    visibleArea.rect.moveTo(visibleArea.rect.x1()/1.2,visibleArea.rect.y1()/1.2);
    this->update();
}

void Widget::zoomIn()
{
    visibleArea.zoom *= 1.2;
//    step *=1.2;
    visibleArea.rect.moveTo(visibleArea.rect.x1()*1.2,visibleArea.rect.y1()*1.2);
    this->update();
}

void Widget::moveLeft()
{
    visibleArea.rect.translate(-step,0);
    this->update();
}


void Widget::moveRight()
{
    visibleArea.rect.translate(+step,0);
    this->update();
}

void Widget::moveUp()
{
    visibleArea.rect.translate(0,step);
    this->update();
}

void Widget::moveDown()
{
    visibleArea.rect.translate(0,-step);
    this->update();
}


void Widget::fit()
{

    if(!cell || !this) return;

    xcenter = this->width()/2;
    ycenter = this->height()/2;

    double margin = 0;

    double width = cell->width()*(1 + margin);
    double height = cell->height()*(1 + margin);
    double wratio = width/this->width();
    double hratio = height/this->height();

    bool isHigh = hratio > wratio ? true: false;
    double ratio = wratio > hratio ? wratio : hratio;

    visibleArea.zoom = 1/ratio;

    double x = 0;
    double y = 0;


    y = cell->centerY()/ratio;
    x = cell->centerX()/ratio;

    if(isHigh){
//        y -= height*margin/2/ratio;
    }else{
        //      x += width*margin/2/ratio;
    }

    visibleArea.rect.moveTo(x,y);
    this->update();
}




void Widget::zoomArea(Rect r)
{
    if(r.width() < 10 || r.height() < 10|| visibleArea.zoom == 0) return;


    //- This zoom took me fucking forever to figure out, so some comments
    //- Figure out new ratio
    double wratio = ((double)r.width())/this->width();
    double hratio = ((double)r.height())/this->height();
    double nratio = wratio > hratio ? wratio : hratio;
    visibleArea.zoom /= nratio;

    //- Move the visibleArea to the new zoom location scaled by the new ratio scale
    visibleArea.rect.moveTo(visibleArea.rect.x1()/nratio,visibleArea.rect.y1()/nratio);

    //visibleArea is in screen space, but referred to center of cell
    //Rubberband is referred to screen space, but absolute
    //1. Figure out distance from center screen to rubber band center
    //2. Scale to visibleArea space with the new ratio
    //2. Translate the view area, but flip Y direction

    double xc = (r.centerX() - xcenter)/nratio;
    double yc = (r.centerY() - ycenter)/nratio;

    visibleArea.rect.translate(xc,-yc);

    this->update();
}



void Widget::wheelEvent(QWheelEvent *event)
{

    if (event->modifiers().testFlag(Qt::ControlModifier) ||event->modifiers().testFlag(Qt::MetaModifier) )
    {
        if(event->angleDelta().y() < 0){
            zoomIn();
        }
        else{
            zoomOut();

        }
    }else  if (event->modifiers().testFlag(Qt::ShiftModifier))
    {
        if(event->angleDelta().y() < 0){
            moveLeft();
        }
        else{
            moveRight();
        }


    } else {
        if(event->angleDelta().y() < 0){
            moveDown();
        }
        else{
            moveUp();
        }

    }

}







void Widget::setCell(Cell* c)
{
    if(!c) return;

    //Clear rubberband if we're jumping to a new cell
    if(cell && cell->name() != c->name()){
        rubberBand = 0;
    }

    cell = c;


    cell->setBoundaryIgnoreRouting(false);
    cell->updateBoundingRect();
    if(!rubberBand)
        this->fit();

}



void Widget::paintEvent(QPaintEvent *event)
{


    QPainter painter;
    painter.begin(this);
//    glClearColor(1.0,1.0,1.0,1.0);
//    glClear(GL_COLOR_BUFFER_BIT); // << If I don't do this it clears out my viewer white
// and I want it black
    //   glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//    painter.setBackgroundMode(Qt::TranspaarentMode); // < was a test, doesn't seem to do anything
    painter.setBackground(QColor(1,1,1,1));


    painter.save();
    if(cell){

        //Zoom and invert
        painter.translate(xcenter, ycenter);
        painter.translate(-visibleArea.rect.x1(),visibleArea.rect.y1());
        painter.scale(visibleArea.zoom,-visibleArea.zoom);
        cellPainter->paintCell(painter,cell,"");
    }

    painter.restore();

    painter.end();
}

bool isSHIFT;




bool Widget::eventFilter(QObject *obj, QEvent *event)
{

//        if (obj != widget) return false;


    Qt::KeyboardModifiers keyMod = QApplication::keyboardModifiers ();
    isSHIFT = keyMod.testFlag(Qt::ShiftModifier);
    //bool isCTRL = keyMod.testFlag(Qt::ControlModifier);

    if(event->type() == QEvent::KeyPress){

        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

        int keyInt = keyEvent->key();

        Qt::Key key = static_cast<Qt::Key>(keyInt);

        if(keyEvent->key() == Qt::Key_F){
            this->fit();

        }


        if(keyEvent->key() == Qt::Key_Z){
            if (isSHIFT)
            {
                this->zoomOut();
                return true;
            }else{
                this->zoomIn();
                return true;
            }
        }

        if (keyEvent->key() == Qt::Key_Up)
        {
            this->moveUp();
            return true;
        }
        else if(keyEvent->key() == Qt::Key_Down)
        {
            this->moveDown();
            return true;
        }else if(keyEvent->key() == Qt::Key_Left)
        {
            this->moveLeft();
            return true;
        }
        else if(keyEvent->key() == Qt::Key_Right)
        {
            this->moveRight();
            return true;
        }
    }

    return QObject::eventFilter(obj, event);

}



void Widget::mousePressEvent(QMouseEvent *event)
{
    //if(static_cast<QMouseEvent*>(event)->button() == Qt::RightButton)  {
    origin = event->pos();
    if (!rubberBand)
        rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
    rubberBand->setGeometry(QRect(origin, QSize()));
    rubberBand->show();
    rubberBandActive = true;

}

void Widget::mouseMoveEvent(QMouseEvent *event)
{

    if(rubberBand){
        rubberBand->setGeometry(QRect(origin, event->pos()).normalized());
    }
}

void Widget::mouseReleaseEvent(QMouseEvent *event)
{

    if(rubberBand){
        QPoint end = event->pos();

        Rect r1 = Rect("",origin.x(),origin.y(),(end.x() - origin.x()),(end.y() - origin.y()));
        zoomArea(r1);
        rubberBand->hide();
    }



}
