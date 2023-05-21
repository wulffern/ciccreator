#include "layoutrotatecell.h"

namespace cIcCore{
    LayoutRotateCell::LayoutRotateCell()
    {

    }

    LayoutRotateCell::~LayoutRotateCell()
    {

    }

    void LayoutRotateCell::rotateAngle(QString s){
        rotateAngle_ = s;
        this->noPowerRoute(0);

    }

    void LayoutRotateCell::rotateAngle(QJsonValue s){
        this->rotateAngle(s.toString());
    }

    void LayoutRotateCell::place(){

        foreach(cIcSpice::SubcktInstance * ckt_inst,_subckt->instances()){
            //The chain of events is important here, ports get defined in the setSubckInstance
            Instance * inst = Instance::getInstance(ckt_inst->subcktName());
            inst->setSubcktInstance(ckt_inst);
            inst->moveTo(0,0);
            inst->setAngle(rotateAngle_);
            //   inst->updateBoundingRect();

            this->add(inst);

        }
    }


    void LayoutRotateCell::paint(){
        LayoutCell::paint();
    }




}
