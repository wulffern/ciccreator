#include "gui/window.h"

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
      //shapeComboBox->addItem(tr("Clock"));
     // shapeComboBox->addItem(tr("House"));
      //shapeComboBox->addItem(tr("Text"));
      //shapeComboBox->addItem(tr("Truck"));



      QWidget * leftSide = new QWidget;
      QGridLayout *layout = new QGridLayout;


      layout->addWidget(zoom,1,0);
      layout->addWidget(listCells,2,0);
      //layout->addWidget(listCells, 1, 0);

      leftSide->setLayout(layout);

      QVBoxLayout *top = new QVBoxLayout();

      splitter->addWidget(leftSide);
      splitter->addWidget(originalRenderArea);
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

      setWindowTitle(tr("Carsten's IC Creator"));
}

Window::~Window()
{

}

void Window::loadDesign(Design *d ){
    designs = d;

    foreach(Cell* c ,d->getAllCells()){
        listCells->addItem(c->name());
      }
 //   shapeComboBox->setCurrentIndex(0);
}

void Window::shapeSelected(int index)
{
    Cell * c = designs->getAllCells().at(index);
    originalRenderArea->setCell(c);
    //originalRenderArea->set

 //   QPainterPath shap e = shapes[index];
   // originalRenderArea->setShape(shape);

}

void Window::zoomChanged(int zoom){
   Rules * rules = Rules::getRules();
  originalRenderArea->setZoom(zoom/100.0/rules->gamma());
}

}

