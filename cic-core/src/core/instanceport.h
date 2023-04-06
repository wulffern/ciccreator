#ifndef CIC_CORE_INSTANCEPORT_H
#define CIC_CORE_INSTANCEPORT_H

#include <QObject>
#include <QList>
#include <QString>
#include <QtCore>
#include "rect.h"
#include "port.h"

namespace cIcCore{
    class InstancePort: public Port{


            Q_OBJECT

        public:
            InstancePort(QString name, Port * p, Rect * parent);
            ~InstancePort();
            void mirrorX(int ay) override;
            void mirrorY(int ax) override;
            Rect * parent(){return parent_;}


            QString childName();


        protected:
            //! Parent, does this rectangle belong to any particular instance
            Rect * parent_;

            //! Link to a child port, provides connectivity through a hierarchy
            Port * childport_;


    };

}

#endif // INSTANCEPORT_H
