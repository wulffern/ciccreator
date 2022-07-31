#ifndef CIC_CORE_CUT_H
#define CIC_CORE_CUT_H

#include "instancecut.h"

namespace cIcCore{

    class Cut: public Cell
    {
        public:
            Cut(QString layer1,QString layer2,int horizontal_cuts, int vertical_cuts);
            Cut(QString layer1,QString layer2,Rect* r);


            static QString makeName(QString layer1, QString layer2, int horizontal_cuts, int vertical_cuts);

            static Instance * getInstance(QString layer1, QString layer2, int horizontal_cuts, int vertical_cuts);
            ~Cut();

            static QList<Rect*> getCutsForRects(QString layer1, QList<Rect*>, int horizontal_cuts,int vertical_cuts);
            static QList<Rect*> getCutsForRects(QString layer1, QList<Rect*>, int horizontal_cuts,int vertical_cuts,bool alignLeft);

            static QList<Rect*> getVerticalFillCutsForRects(QString layer1, QList<Rect*> rects, int horizontal_cuts);


            static QList<Cut*> getCuts();



        protected:
            static QMap<QString,Cut*> cuts_;

    };

}

#endif // CUT_H
