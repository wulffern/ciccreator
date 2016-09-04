#include "cut.h"

namespace cIcCore {

    QMap<QString,Cut*> Cut::cuts_;


    QString Cut::makeName(QString layer1, QString layer2, int horizontal_cuts, int vertical_cuts) {
        return QString("cut_%1%2_%3x%4").arg(layer1,layer2,QString::number(horizontal_cuts),QString::number(vertical_cuts));

    }


    Cut::Cut(QString layer1, QString layer2, int horizontal_cuts, int vertical_cuts):Cell()
    {
        Rules * rules = Rules::getRules();
        QList<Layer*> layers = rules->getConnectStack(layer1,layer2);

        if(layers.count() == 0){
            qDebug() << "No layers to cut for " << layer1 << layer2;
            return;
        }

        this->setName(this->makeName(layers.first()->name,layers.last()->name,horizontal_cuts,vertical_cuts));

        foreach(Layer * l, layers){
            if(l->material == Layer::metal || l->material == Layer::poly){
                int width = rules->get(l->name,"width");
                QString encRule = l->next  + "encOpposite";
                int encOpposite = rules->get(l->name,encRule);
                int enclosure = rules->get(l->name,"enclosure");
                int cut_width = rules->get(l->next,"width");
                int cut_height = rules->get(l->next,"height");
                int cut_space = rules->get(l->next,"space");

                int r_width;
                int r_height;
                if(horizontal_cuts > vertical_cuts){
                    r_width = encOpposite*2 + cut_width*horizontal_cuts + cut_space*(horizontal_cuts - 1);
                    r_height = enclosure*2 + cut_height*vertical_cuts + cut_space*(vertical_cuts - 1);
                }else if(horizontal_cuts == vertical_cuts){
                    r_width = enclosure*2 + cut_width*horizontal_cuts + cut_space*(horizontal_cuts - 1);
                    r_height = encOpposite*2 + cut_height*vertical_cuts + cut_space*(vertical_cuts - 1);
                }
                else{
                    r_width = enclosure*2 + cut_width*horizontal_cuts + cut_space*(horizontal_cuts - 1);
                    r_height = encOpposite*2 + cut_height*vertical_cuts + cut_space*(vertical_cuts - 1);
                }

                Rect * r = new Rect(l->name,0,0,r_width,r_height);
                this->add(r);

            }else if(l->material == Layer::cut){
                QString encRule = l->name  + "encOpposite";
                int encOpposite = rules->get(l->previous,encRule);
                int enclosure = rules->get(l->previous,"enclosure");
                int cut_width = rules->get(l->name,"width");
                int cut_height = rules->get(l->name,"height");
                int cut_space = rules->get(l->name,"space");

                int enc_x = encOpposite;
                int enc_y = enclosure;
                if(horizontal_cuts < vertical_cuts){
                    enc_x = enclosure;
                    enc_y = encOpposite;
                }else if(horizontal_cuts == vertical_cuts){
                    enc_x = enclosure;
                    enc_y = encOpposite;

                }
                

                int xa1 = enc_x;
                int ya1 = enc_y;
                for(int x = 0;x < horizontal_cuts; x++){
                    for( int y = 0;y < vertical_cuts; y++){
                        Rect * r = new Rect(l->name,xa1,ya1,cut_width,cut_height);
                        this->add(r);
                        ya1 += cut_height + cut_space;
                    }
                    ya1 = enc_y;
                    xa1 += cut_width + cut_space;
                }
            }
        }

        this->updateBoundingRect();
        
    }

    Cut::~Cut()
    {

    }


    QList<Cut*> Cut::getCuts(){
        return cuts_.values();

    }

    QList<Rect*> Cut::getVerticalFillCutsForRects(QString layer1, QList<Rect*> rects, int horizontal_cuts)
    {

        //TODO: Make fill rectangle


        QList<Rect*> cuts;
        foreach(Rect *r, rects){
            if(r == NULL) continue;
            if(layer1 == r->layer()) continue;



        }

    }


    QList<Rect*> Cut::getCutsForRects(QString layer1, QList<Rect*> rects, int horizontal_cuts,int vertical_cuts){

        QList<Rect*> cuts;
        foreach(Rect *r, rects){
            if(r == NULL){continue;}
            if(layer1 != r->layer()){

                
                Instance * inst= Cut::getInstance(layer1,r->layer(),horizontal_cuts,vertical_cuts);
                if(inst){

                    if((r->isVertical() && inst->isHorizontal()) || (r->isHorizontal() && inst->isVertical())){
                        //In this case we've got the wrong cut, and it
                        //should be rotated
                        inst= Cut::getInstance(layer1,r->layer(),vertical_cuts,horizontal_cuts);
                        
                    }
                    
                    inst->moveTo(r->x1(),r->y1());

//Don't think this is a good idea
//r->setWidth(inst->width());

                    cuts.append(inst);
                }
            }
        }
        return cuts;
    }

    Instance * Cut::getInstance(QString layer1, QString layer2, int horizontal_cuts, int vertical_cuts){

        InstanceCut *instance = 0;
        QString tag1 =   Cut::makeName(layer1,layer2,horizontal_cuts,vertical_cuts);
        QString tag2 = Cut::makeName(layer2,layer1,horizontal_cuts,vertical_cuts);

        Cut * c;
        if(cuts_.contains(tag1)){
            c = cuts_[tag1];
        }else if(cuts_.contains(tag2)){
            c = cuts_[tag2];
        }else{
            c = new Cut(layer1,layer2,horizontal_cuts,vertical_cuts);

            cuts_[c->name()] = c;
        }

        if(c->name() != ""){


            instance= new InstanceCut();
            instance->setCell(c);
            instance->setName(c->name());
            instance->updateBoundingRect();

        }

        return instance;
    }

}
