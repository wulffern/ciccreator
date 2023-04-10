//====================================================================
//        Copyright (c) 2015 Carsten Wulff Software, Norway
// ===================================================================
// Created       : wulff at 2015-8-6
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

#include "window.h"


namespace cIcGui{

    Window::Window(QWidget *parent) : QWidget(parent)
    {
        qApp->installEventFilter(this);
        QScreen *screen = QGuiApplication::primaryScreen();
        QRect  screenGeometry = screen->geometry();
        resize(screenGeometry.size() * 0.7);

//        setWindowIcon(
//        setWindowIcon();
        
        
        shapeComboBox = new QComboBox;
        listCells = new QListWidget();
        listLayers = new QListWidget();
        splitter = new QSplitter();

        QWidget * leftSide = new QWidget;
        leftSide->resize(150,200);
        QGridLayout *layout = new QGridLayout;
        layout->addWidget(listCells,3,0);
        layout->addWidget(listLayers, 4, 0);
        leftSide->setLayout(layout);
        QVBoxLayout *top = new QVBoxLayout();
        splitter->addWidget(leftSide);

        widget = new Widget( this);
        widget->resize(this->size()*0.7);

        //- Add renderer
        splitter->addWidget(widget);
        top->addWidget(splitter);
        setLayout(top);

        connect(listCells,SIGNAL(currentRowChanged(int)),this,SLOT(shapeSelected(int)));
        connect(listLayers,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(layerClicked(QModelIndex)));

        setWindowTitle(tr("Custom IC Creator Viewer: Zoom In = Z, Zoom Out = Shift+Z, Fit = F, Move = Arrows, Refresh=Shift+R, Toggle layer visibility=t"));
        shift_r= new QShortcut(QKeySequence(Qt::SHIFT + Qt::Key_R),this);
        connect(shift_r,SIGNAL(activated()),this,SLOT(reloadFile()));
        key_t= new QShortcut(QKeySequence(Qt::Key_T),this);
        connect(key_t,SIGNAL(activated()),this,SLOT(toggleVisible()));


        listLayers->clear();

        Rules * rules = Rules::getRules();
        QMap<QString,Layer *> layers = rules->layers();
        QList<Layer *> layerList = layers.values();
        std::sort(layerList.begin(),layerList.end(),this->layerCompare);
        foreach(Layer * l, layerList){
            Layer::MATERIAL_TYPE m = l->material;
            if(m == Layer::metalres || m == Layer::marker || m == Layer::metalres){
                l->visible = false;
                continue;
            }
            QListWidgetItem * item = new QListWidgetItem(l->name);
            item->setIcon(widget->getIcon(l));
            listLayers->addItem(item);
        }

    }

    bool Window::layerCompare(Layer * a, Layer * b){

        return a->name < b->name;
    }

    Window::~Window()
    {

    }

    void Window::loadFile(QString f){
        filename = f;
        Design *d  = new Design();
        d->readJsonFile(f);
        loadDesign(d);
    }

    void Window::reloadFile(){
        if(!filename.isEmpty()){
            int index = 0;
            QModelIndexList indexes = listCells->selectionModel()->selectedIndexes();

            if(indexes.count() > 0){
                index = indexes[0].row();
            }
            listCells->clear();
            loadFile(filename);
            listCells->setCurrentRow(index);
            this->shapeSelected(index);
        }
    }

    void Window::loadDesign(Design *d ){
        designs = d;
        listCells->clear();
        foreach(Cell* c ,d->getAllCells()){
            listCells->addItem(c->name());
        }


    }

    void Window::toggleVisible(){

        if(!designs)
            return;

        if(showAll){
            showAll = false;
        }else{
            showAll = true;
        }
        for(int i = 0; i < listLayers->count(); ++i)
        {
            QListWidgetItem * item = listLayers->item(i);
            Layer *l = Rules::getRules()->getLayer(item->text());
        l->visible = showAll;
        if(l->visible){
            item->setBackground(QColor("white"));
        }else{
            item->setBackground(QColor("gray"));
        }
        }
        //- TODO: Update renderer
        widget->update();
    }

    void Window::layerClicked(QModelIndex index){

        if(!designs)
            return;
        QListWidgetItem * item = listLayers->item(index.row());
        Layer *l = Rules::getRules()->getLayer(item->text());
        l->visible = !l->visible;
        if(l->visible){
            item->setBackground(QColor("white"));
        }else{
            item->setBackground(QColor("gray"));
        }
        //- TODO: Update renderer
        widget->update();
    }

    void Window::shapeSelected(int index)
    {
        if(designs == NULL)
            return;
        QList<Cell*> list = designs->getAllCells();
        if(list.count() > index && index >= 0){

            Cell * c = designs->getAllCells().at(index);
            widget->setCell(c);
            widget->update();

            //-TODO: Set cell
        }
    }

    // bool Window::eventFilter(QObject *obj, QEvent *event)
    // {

        
    //     if (obj != widget) return false;
        
        
    //     Qt::KeyboardModifiers keyMod = QApplication::keyboardModifiers ();
    //     isSHIFT = keyMod.testFlag(Qt::ShiftModifier);
    //     bool isCTRL = keyMod.testFlag(Qt::ControlModifier);

    //     if(event->type() == QEvent::KeyPress){

    //         QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

    //         int keyInt = keyEvent->key();

    //         Qt::Key key = static_cast<Qt::Key>(keyInt);

    //         if(keyEvent->key() == Qt::Key_Z){
    //             if (isSHIFT)
    //             {
    //                 widget->zoomIn();
    //                 return true;
    //             }else{
    //                 widget->zoomOut();
    //                 return true;
    //             }
    //         }

    //         if (keyEvent->key() == Qt::Key_Up)
    //         {
    //             widget->moveUp();
    //              return true;
    //         }
    //         else if(keyEvent->key() == Qt::Key_Down)
    //         {
    //             widget->moveDown();
    //             return true;
    //         }else if(keyEvent->key() == Qt::Key_Left)
    //         {
    //             widget->moveLeft();
    //             return true;
    //         }
    //         else if(keyEvent->key() == Qt::Key_Right)
    //         {
    //             widget->moveRight();
    //             return true;
    //         }
    //     } 

    //     return QObject::eventFilter(obj, event);

    // }

}
