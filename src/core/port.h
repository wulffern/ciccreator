
#ifndef CIC_CORE_PORT
#define CIC_CORE_PORT

#include <QObject>
#include <QList>
#include <QString>
#include "core/rect.h"


namespace cIcCore{
  class Port: public Rect
  {
    Q_OBJECT

  public:

    Port(){

    }

    ~Port(){

    }

    Port(QString name){
      name_ = name;
    }

    QString name(){return name_;}
    void setName(QString name){name_ = name;}
    void add(Rect * r ){

      if(rectangles_.count() == 0){
          Layer * l = rules->getLayer(r->layer());
          this->setRect(l->pin,r->x1(),r->y1(),r->width(),r->height());
        }

      rectangles_.append(r);
    }

    Rect * get(){
      if(rectangles_.count()> 0){
          return rectangles_[0];
        }
      return 0;
    }

  protected:
    QString name_;
    QList<Rect *> rectangles_;



  };

}

Q_DECLARE_METATYPE(cIcCore::Port)

#endif // PORT

