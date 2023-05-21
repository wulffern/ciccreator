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

  Q_INVOKABLE
  void rotateAngle(QJsonValue s);
  void rotateAngle(QString s);
  void place() override;

  void paint() override;

protected:
  QString rotateAngle_;
};

}

Q_DECLARE_METATYPE(cIcCore::LayoutRotateCell)

#endif // LAYOUTROTATECELL_H
