
#ifndef CIC_CORE_LAYOUTCELL_H
#define CIC_CORE_LAYOUTCELL_H

#include <QObject>
#include "cell.h"
#include "instance.h"

namespace cIcCore{

  class LayoutCell : public Cell
  {
    Q_OBJECT

  public:
    LayoutCell(){  useHalfHeight = false;}
    LayoutCell(const LayoutCell&){}
    ~LayoutCell() {}

    Q_INVOKABLE void setYoffsetHalf(QJsonValue obj);

    virtual void place();


  private:
    bool useHalfHeight;
  };

}

Q_DECLARE_METATYPE(cIcCore::LayoutCell)

#endif // LAYOUTCELL_H
