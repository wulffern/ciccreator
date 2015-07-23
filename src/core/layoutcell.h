#ifndef CIC_CORE_LAYOUTCELL_H
#define CIC_CORE_LAYOUTCELL_H

#include <QObject>
#include "instance.h"

namespace cIcCore{

class LayoutCell : public Cell
{
public:
  LayoutCell();
  ~LayoutCell();
  virtual void place();
};

}

#endif // LAYOUTCELL_H
