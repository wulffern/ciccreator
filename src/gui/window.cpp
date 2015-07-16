#include "gui/window.h"

namespace cIcGui{

Window::Window(QWidget *parent) : QWidget(parent)
{

  originalRenderArea = new RenderArea;

      shapeComboBox = new QComboBox;
      zoom = new QSlider;
      //shapeComboBox->addItem(tr("Clock"));
     // shapeComboBox->addItem(tr("House"));
      //shapeComboBox->addItem(tr("Text"));
      //shapeComboBox->addItem(tr("Truck"));

      QGridLayout *layout = new QGridLayout;
      layout->addWidget(originalRenderArea, 0, 0);
      layout->addWidget(zoom,1,0);
      layout->addWidget(shapeComboBox, 2, 0);

      setLayout(layout);
      //setupShapes();
      //shapeSelected(0);

      zoom->setMaximum(200);
      zoom->setMinimum(1);
      zoom->setValue(100);
      zoom->setOrientation(Qt::Horizontal);


          connect(shapeComboBox, SIGNAL(activated(int)), this, SLOT(shapeSelected(int)));
          connect(zoom,SIGNAL(valueChanged(int)),this,SLOT(zoomChanged(int)));

      setWindowTitle(tr("Carsten's IC Creator"));
}

Window::~Window()
{

}

void Window::loadDesign(Design *d ){
    designs = d;
    foreach(Cell* c ,d->getAllCells()){
        shapeComboBox->addItem(c->name());
      }
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
  originalRenderArea->setZoom(zoom/100.0);
}

}

