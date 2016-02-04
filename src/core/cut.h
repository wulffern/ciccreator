#ifndef CIC_CORE_CUT_H
#define CIC_CORE_CUT_H

#include "instance.h"

namespace cIcCore{

  class Cut: public Cell
{
public:
  Cut(QString layer1,QString layer2,int horizontal_cuts, int vertical_cuts);


   static QString makeName(QString layer1, QString layer2, int horizontal_cuts, int vertical_cuts);

  static Instance * getInstance(QString layer1, QString layer2, int horizontal_cuts, int vertical_cuts);
  ~Cut();

  static QList<Cut*> getCuts();

  protected:
    static QMap<QString,Cut*> cuts_;
};

}

#endif // CUT_H
