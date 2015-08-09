#ifndef CIC_PRINTER_SPICE_H
#define CIC_PRINTER_SPICE_H

#include "designprinter.h"
#include "core/cell.h"

namespace cIcPrinter{

class Spice : public DesignPrinter
{
public:
  Spice(QString name);
  ~Spice();

  virtual void startCell(Cell * cell);
  virtual void endCell();
  virtual void printRect(Rect *rect);
  virtual void printReference(Cell *c);
  virtual void printPort(Port *);


protected:
  bool subcktInPrint;

};

}

#endif // SPICE_H
