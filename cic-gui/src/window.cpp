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

    originalRenderArea = new RenderArea;

    //    originalRenderArea->setZoom(100/rules->gamma());


    shapeComboBox = new QComboBox;
    listCells = new QListWidget();
    listLayers = new QListWidget();
    splitter = new QSplitter();
    zoom = new QSlider;
    scroll = new QScrollArea();

    scroll->setWidget(originalRenderArea);

    QWidget * leftSide = new QWidget;
    leftSide->resize(150,200);
    QGridLayout *layout = new QGridLayout;

    layout->addWidget(zoom,1,0);
    layout->addWidget(listCells,2,0);
    layout->addWidget(listLayers, 3, 0);

    leftSide->setLayout(layout);

    QVBoxLayout *top = new QVBoxLayout();

    splitter->addWidget(leftSide);
    splitter->addWidget(scroll);
    top->addWidget(splitter);

    setLayout(top);
    //setupShapes();
    //shapeSelected(0);

    zoom->setMaximum(300);
    zoom->setMinimum(1);
    zoom->setValue(100);
    zoom->setOrientation(Qt::Horizontal);

    connect(listCells,SIGNAL(currentRowChanged(int)),this,SLOT(shapeSelected(int)));

    connect(zoom,SIGNAL(valueChanged(int)),this,SLOT(zoomChanged(int)));

    connect(listLayers,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(layerClicked(QModelIndex)));

    setWindowTitle(tr("Custom IC Creator"));

    shift_z= new QShortcut(QKeySequence(Qt::SHIFT + Qt::Key_Z),this);
    ctrl_z= new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Z),this);
        shift_r= new QShortcut(QKeySequence(Qt::SHIFT + Qt::Key_R),this);

    connect(shift_z,SIGNAL(activated()),originalRenderArea,SLOT(zoomIn()));
    connect(ctrl_z,SIGNAL(activated()),originalRenderArea,SLOT(zoomOut()));

      connect(shift_r,SIGNAL(activated()),this,SLOT(reloadFile()));
         listLayers->clear();

    Rules * rules = Rules::getRules();
    QMap<QString,Layer *> layers = rules->layers();
    QList<Layer *> layerList = layers.values();
    qSort(layerList);
    foreach(Layer * l, layerList){
        //if(l->material == Layer::metalres || l->material == Layer::marker || l->material == Layer::metalres){
        //    l->visible = false;
        //    continue;
        //  }
        QListWidgetItem * item = new QListWidgetItem(l->name);
        item->setIcon(l->icon());

        listLayers->addItem(item);
      }

  }

  Window::~Window()
  {

  }


   void Window::loadFile(QString f){

	   //	  if(designs)
	   //		  delete(designs);

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
			//QModelIndex modelIndex = listCells->rootIndex(); // u have to find the model index of the first item here
			//listCells->setCurrentIndex(modelIndex);
		    //shapeSelected(index);
		}
	}

  void Window::loadDesign(Design *d ){
    designs = d;

    listCells->clear();

    foreach(Cell* c ,d->getAllCells()){
        listCells->addItem(c->name());
      }


  }

  void Window::layerClicked(QModelIndex index){

	  if(!designs)
		  return;
    QListWidgetItem * item = listLayers->item(index.row());
    Layer *l = Rules::getRules()->getLayer(item->text());
    l->visible = !l->visible;
    if(l->visible){
        item->setBackgroundColor(QColor("white"));
      }else{
        item->setBackgroundColor(QColor("gray"));
      }
    originalRenderArea->update();
  }

  void Window::shapeSelected(int index)
  {
                  if(designs == NULL)
                  return;
               QList<Cell*> list = designs->getAllCells();
               if(list.count() > index && index >= 0){

    Cell * c = designs->getAllCells().at(index);
    originalRenderArea->setCell(c);
                 }
    //originalRenderArea->set

    //   QPainterPath shap e = shapes[index];
    // originalRenderArea->setShape(shape);

  }

  void Window::zoomChanged(int zoom){
                  if(!designs)
                  return;
    Rules * rules = Rules::getRules();
    //originalRenderArea->setBa
    originalRenderArea->setZoom(zoom/100.0/rules->gamma());
  }

}

