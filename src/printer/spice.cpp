#include "spice.h"

namespace cIcPrinter{

  Spice::Spice(QString name):DesignPrinter(name + ".spi")
  {
    subcktInPrint = false;

  }

  Spice::~Spice()
  {
  } 

  void Spice::startCell(Cell * cell){

    if(!cell){return;}
    cIcSpice::Subckt * ckt = cell->subckt();

    if(!ckt){return;}

    //I think these are devices
    //    if(ckt->name() == ""){return;}
    QTextStream ts(&file);
    ts << "\n";
    ts << "*-------------------------------------------------------------\n";
    ts << "* " << cell->name() << " (" << cell->metaObject()->className() <<")\n";
    ts << "*-------------------------------------------------------------\n";
    if(ckt->name() != ""){
        ts << ".SUBCKT " << ckt->name() << " " << ckt->nodes().join(' ')  << "\n";
      }else{
        SpiceObject * so = cell->spiceObject();
        if(so){

            ts << ".SUBCKT " << cell->name() << " " << so->nodes().join(' ')  << "\n";
            ts << so->toSpice("1", so->nodes()) << "\n";
          }else{
            ts << ".SUBCKT " << cell->name() << "\n";

          }
      }
    this->subcktInPrint = true;

  }

  void Spice::endCell(){
    if(subcktInPrint){
        QTextStream ts(&file);
        ts << ".ENDS \n";
      }
    subcktInPrint = false;

  }

  void Spice::printRect(Rect *rect){

  }

  void Spice::printPort(Port *){

  }

  void Spice::printReference(Cell *c){

    Instance * i = (Instance *) c;
    SubcktInstance *si = i->subcktInstance();
    Cell * cell =   i->cell();

    if(!cell){return;}

//    SpiceObject * so = cell->spiceObject();

    QTextStream ts(&file);

    if(si){
//        ts << so->toSpice(si->name(), si->nodes()) << "\n";
//      }else{
        ts << si->name() << " " << si->nodes().join(' ') << " " << si->subcktName() << "\n";

      }
  }



}

