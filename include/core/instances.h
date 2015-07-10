#ifndef CIC_CORE_INSTANCE_H
#define CIC_CORE_INSTANCE_H


namespace cIcCore{

  class Instance: public Rect{

    Q_OBJECT

  public:
    Instance();
    ~Instance();
    Instance(const Instance&);


  };

}

#endif // INSTANCES_H

