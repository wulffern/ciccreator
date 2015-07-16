#ifndef CIC_GUI_WINDOW_H
#define CIC_GUI_WINDOW_H

//#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <QComboBox>
#include <QSlider>
#include <QGridLayout>
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
  QSlider *zoom;
  Design * designs;

};

}

#endif // WINDOW_H
