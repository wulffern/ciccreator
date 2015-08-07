#ifndef CIC_SPICE_MOSFET
#define CIC_SPICE_MOSFET

#include <QString>
#include "core/rules.h"
#include "spice/spiceobject.h"

namespace cIcSpice{

  class Mosfet : public SpiceObject
  {



    Q_OBJECT

  public:
    Mosfet(){
      numberOfFingers = 1;
      drainWidth = 0;
      sourceWidth = 0;
      width = 0;
      length = 0;
      multiplier = 1;
      this->deviceName_ = "nch";
      this->spiceType_ = "M";
      QStringList n;
      n << "D" << "G" << "S" << "B";
      this->setNodes(n);
    }

    Mosfet(const Mosfet& mos){

    }

    ~Mosfet(){

    }


    virtual QString toSpice( QString instance, QStringList nodes){
      QString s;
      QTextStream ts(&s);

      cIcCore::Rules * rules = cIcCore::Rules::getRules();
      cIcCore::Device * mtype = rules->getDevice(this->deviceName());

      if(mtype){
            ts << "M" << instance << " " << nodes.join(' ') <<  " " << mtype->name << " w=" << width << " l=" << length << " nf=" << numberOfFingers << " M=" << multiplier;
        }else{
                ts << "M" << instance << " " << nodes.join(' ') <<  " " << this->deviceName() << " w=" << width << " l=" << length << " nf=" << numberOfFingers << " M=" << multiplier;
        }

    return s;
    }


    int numberOfFingers;
    double drainWidth;
    double sourceWidth;
    double width;
    double length;
    int multiplier;


  };

}

Q_DECLARE_METATYPE(cIcSpice::Mosfet)

#endif // MOSFET

