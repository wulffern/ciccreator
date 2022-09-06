#include "instanceport.h"

namespace cIcCore{


    void InstancePort::mirrorX(int ay)
    {
        Rect::mirrorX(ay);
    }

    void InstancePort::mirrorY(int ax)
    {
        Rect::mirrorY(ax);
    }

    InstancePort::InstancePort(QString name, Port * p, Rect * parent):Port(name)
    {
        childport_ = p;
        parent_ = parent;
        Rect *r = p->get();
        if(r){
            Layer * l = rules->getLayer(r->layer());
            routeLayer_ = l;
            this->setRect(r->layer(), r->x1(), r->y1(),r->width(),r->height());

        }
    }




    InstancePort::~InstancePort()
    {

    }


    QString InstancePort::childName(){
        if(childport_){
            return childport_->name();
        }else{
            return QString("");
        }

    }



}
