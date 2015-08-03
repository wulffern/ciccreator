#ifndef CIC_GUI_WINDOW_H
#define CIC_GUI_WINDOW_H

//#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <QComboBox>
#include <QListWidget>
#include <QSlider>
#include <QGridLayout>
#include <QSplitter>
#include "core/design.h"

#include "renderarea.h"

QT_BEGIN_NAMESPACE
class QComboBox;
class QSlider;
QT_END_NAMESPACE


namespace cIcGui{
  using namespace cIcCore;
  //using cIcCore;

class Window : public QWidget
{
  Q_OBJECT
public:
  explicit Window(QWidget *parent = 0);
  void loadDesign(Design *d);
  ~Window();

signals:

public slots:
    void shapeSelected(int index);
    void zoomChanged(int zoom);

private:
  RenderArea *originalRenderArea;
  QComboBox *shapeComboBox;
  QListWidget * listCells;
  QListWidget * listLayers;
  QSlider *zoom;
  Design * designs;
  QSplitter * splitter;
  QWidget * leftside;


};

}

#endif // WINDOW_H
