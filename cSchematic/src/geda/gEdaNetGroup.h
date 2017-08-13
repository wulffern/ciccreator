/*********************************************************************
 *        Copyright (c) 2010 Carsten Wulff Software, Norway
 * *******************************************************************
 * Created       : wulff at 2010-9-13
 * Modified      : $Author$ $Date$
 * Version       : $Revision$ $HeadURL$
 * Description   :
 ********************************************************************/

#ifndef GEDANETGROUP_H
#define GEDANETGROUP_H

namespace cBase {

 class gEdaNetGroup: public GObject
  {


  private:

    QList<gEdaNet*> nets_;


  public:
    //--------------------------------------------------------
    // ctor and dtor
    //--------------------------------------------------------
    gEdaNetGroup(){
      type = OBJ_NETGROUP;
    }

    ~gEdaNetGroup(){

    }

    QList<gEdaNet*> & nets(){
      return nets_;
    }

    bool isMember(gEdaNet *a){

      foreach(gEdaNet * b,nets_){
        if(cross(a,b)){
          return true;
        }
      }
      return false;
    }

    bool addIfMember(gEdaNet *net){
      if(isMember(net)){
        addNet(net);
        return true;
      }
      return false;
    }

    void addNet(gEdaNet *net){
      if(net != 0){
        nets_.append(net);
        children.append(net);
      }
    }

    void clear(){
      nets_.clear();
    }


    void stealIfMember(gEdaNetGroup * ngroup){

      bool netIsMember = false;
      foreach(gEdaNet * net,ngroup->nets()){
        if(isMember(net)){
          netIsMember = true;
        }
      }

      if(netIsMember){
        foreach(gEdaNet * net,ngroup->nets()){
          addNet(net);
        }
        ngroup->clear();
      }
    }


    bool cross(gEdaNet *a, gEdaNet *b){

      bool doCross = false;

      //Same endpoint
      if(a->x1 == b->x1 && a->y1 == b->y1)
        doCross = true;
      if(a->x2 == b->x1 && a->y2 == b->y1)
        doCross = true;
      if(a->x1 == b->x2 && a->y1 == b->y2)
        doCross = true;
      if(a->x2 == b->x2 && a->y2 == b->y2)
        doCross = true;

      if(doCross){return doCross;}

      //Intersection
      if(a->x1 == a->x2
         && (b->x1 == a->x1 || b->x2 == a->x1)
         && (b->y1 == b->y2)
         && (b->y1 < max(a->y1,a->y2) && b->y1 > min(a->y1,a->y2))
         ){
        doCross = true;
      }

      if(a->y1 == a->y2
         && (b->y1 == a->y1 || b->y2 == a->y1)
         && (b->x1 == b->x2)
         && (b->x1 < max(a->x1,a->x2) && b->x1 > min(a->x1,a->x2))
         )
        doCross = true;

      return doCross;
    }

    QString & toString(){
      QStringList strs;
      foreach(gEdaNet * net,nets_){
        strs << net->toString();
      }
      string = strs.join("\n");
      return string;
    }

  };
}



#endif // GEDANETGROUP
