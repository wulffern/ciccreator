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

        //TODO: OD -> M1 contact is not generated correctly, should
        //fix at some point

        this->setName(this->makeName(layers.first()->name,layers.last()->name,horizontal_cuts,vertical_cuts));

        foreach(Layer * l, layers){
            if(l->material == Layer::metal || l->material == Layer::poly|| l->material == Layer::diffusion ){
                QString encRule = l->next  + "encOpposite";
                int encOpposite = rules->get(l->name,encRule);
                QString encThis = l->next + "enclosure";
                int enclosure = 0;
                if(rules->hasRule(l->name,encRule)){
                    enclosure = rules->get(l->name,encThis);
                }else{
                    enclosure = rules->get(l->name,"enclosure");
                }
                int cut_width = rules->get(l->next,"width");
                int cut_height = rules->get(l->next,"height");
                int cut_space = rules->get(l->next,"space");

                int r_width;
                int r_height;
                if(horizontal_cuts > vertical_cuts){
                    r_width = encOpposite*2 + cut_width*horizontal_cuts + cut_space*(horizontal_cuts - 1);
                    r_height = enclosure*2 + cut_height*vertical_cuts + cut_space*(vertical_cuts - 1);
                }else if(horizontal_cuts == vertical_cuts){
                    //2018-02-24: Modified to support 22nm
                    r_width = encOpposite*2 + cut_width*horizontal_cuts + cut_space*(horizontal_cuts - 1);
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
                QString encThis = l->name + "enclosure";
                int enclosure = 0;
                if(rules->hasRule(l->previous,encRule)){
                    enclosure = rules->get(l->previous,encThis);
                }else{
                    enclosure = rules->get(l->previous,"enclosure");
                }

//                int enclosure = rules->get(l->previous,"enclosure");
                int cut_width = rules->get(l->name,"width");
                int cut_height = rules->get(l->name,"height");
                int cut_space = rules->get(l->name,"space");

                int enc_x = encOpposite;
                int enc_y = enclosure;
                if(horizontal_cuts < vertical_cuts){
                    enc_x = enclosure;
                    enc_y = encOpposite;
                }else if(horizontal_cuts == vertical_cuts){
                    //2018-02-20: Modified to support 22nm
                    enc_x = encOpposite;
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

    Cut::Cut(QString layer1, QString layer2, Rect* r):Cell()
    {
        Rules * rules = Rules::getRules();
        QList<Layer*> layers = rules->getConnectStack(layer1,layer2);

        if(layers.count() == 0){
            qDebug() << "No layers to cut for " << layer1 << layer2;
            return;
        }

        this->setName("fillCut");

        foreach(Layer * l, layers){
            if(l->material == Layer::metal || l->material == Layer::poly|| l->material == Layer::diffusion ){


                this->add(new Rect(l->name,0,0,r->width(),r->height()));
            }else if(l->material == Layer::cut){
                QString encRule = l->name  + "encOpposite";
                int encOpposite = rules->get(l->previous,encRule);
                int enclosure = rules->get(l->previous,"enclosure");
                int cut_width = rules->get(l->name,"width");
                int cut_height = rules->get(l->name,"height");
                int cut_space = rules->get(l->name,"space");

                int enc_x = encOpposite;
                int enc_y = enclosure;
                if(r->isHorizontal() && r->isVertical()){
                    enc_x = encOpposite;
                    enc_y = encOpposite;
                }else if(r->isVertical()){
                    enc_x = enclosure;
                    enc_y = encOpposite;
                }else if(r->isHorizontal()){
                    enc_x = encOpposite;
                    enc_y = enclosure;
                }


                int width = r->width() - enc_x*2;
                int horizontal_cuts = width/(cut_width  + cut_space);
                int height = r->height() - enc_y*2;
                int vertical_cuts = height/(cut_height  + cut_space);

                if(vertical_cuts == 0) {
                    vertical_cuts = height/(cut_height);
                }

                if(horizontal_cuts == 0) {
                    horizontal_cuts = width/(cut_width);
                }



                this->setName(this->makeName(layers.first()->name,layers.last()->name,horizontal_cuts,vertical_cuts));


                int xa1 = enc_x;
                int ya1 = enc_y;


                if(r->isHorizontal()){
                    xa1 = int((r->width() - horizontal_cuts*(cut_width + cut_space) + cut_space)/2/10)*10;
                }

                if(r->isVertical()){
                    ya1 = int((r->height() - vertical_cuts*(cut_height + cut_space) + cut_space)/2/10)*10;
                }



                for(int x = 0;x < horizontal_cuts; x++){
                    for( int y = 0;y < vertical_cuts; y++){
                        Rect * r1 = new Rect(l->name,xa1,ya1,cut_width,cut_height);
                        this->add(r1);
                        ya1 += cut_height + cut_space;
                    }
                    ya1 = enc_y;
                    xa1 += cut_width + cut_space;
                }


            }
        }

        this->updateBoundingRect();
        this->moveTo(r->x1(),r->y1());
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

        return Cut::getCutsForRects(layer1,rects,horizontal_cuts,vertical_cuts,true);
    }



    QList<Rect*> Cut::getCutsForRects(QString layer1, QList<Rect*> rects, int horizontal_cuts,int vertical_cuts, bool alignLeft){

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

                    if(alignLeft){
                        inst->moveTo(r->x1(),r->y1());
                    }
                    else{
                        inst->moveTo(r->x2() - inst->width(),r->y1());
                    }


                    int xc = r->centerX();

                    //Resize rectangle if the center of rect is not
                    //contained in the instance
                    if(inst->x1() > xc || inst->x2() < xc){
                        r->setWidth(inst->width());
                    }


                    cuts.append(inst);
                }
            }
        }
        return cuts;
    }

    Instance * Cut::getInstance(QString layer1, QString layer2, int horizontal_cuts, int vertical_cuts){

        //-No point in making a cut without layer transition
        if(layer1 == layer2){
            return nullptr;
        }

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

        if(!instance){
            std::string errorMessage  = "Error: Could not create cut from " + layer1.toStdString() + " to " + layer2.toStdString();
            throw std::runtime_error(errorMessage);
        }

        return instance;
    }

}
