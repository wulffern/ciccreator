#ifndef CIC_CORE_LAYOUTROTATECELL_H
#define CIC_CORE_LAYOUTROTATECELL_H

#include "layoutcell.h"

namespace cIcCore{


class LayoutRotateCell : public LayoutCell
{

   Q_OBJECT


public:
  LayoutRotateCell();
  ~LayoutRotateCell();

  Q_INVOKABLE void rotateAngle(QString s);
  virtual void place();

    virtual void paint();

protected:
  QString rotateAngle_;
};

}

Q_DECLARE_METATYPE(cIcCore::LayoutRotateCell)

#endif // LAYOUTROTATECELL_H
