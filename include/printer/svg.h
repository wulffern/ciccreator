/*********************************************************************
 *        Copyright (c) 2014 Carsten Wulff Software, Norway 
 * *******************************************************************
 * Created       : wulff at 2014-9-21
 * Modified      : $Author$ $Date$
 * Version       : $Revision$ $HeadURL$
 * Description   :  
 ********************************************************************/

#ifndef CIC_PRINTER_SVG
#define CIC_PRINTER_SVG

#include <QObject>
#include <QString>
#include "printer/designprinter.h"
namespace cIcPrinter{
	using namespace cIcCore;
	
    class Svg : public DesignPrinter{

    public:

        Svg(QString filename):DesignPrinter(filename){
           
        }
        ~Svg(){
            
        }
        
        virtual void startCell(Cell * cell);
		virtual void endCell();
		//      void endFile();
 //      void printInstance(Instance & inst);
 //      void printInitLib(string name) ;
 //      void printEndLib() ;
 //      void printInitCell(Cell & cell) ;
 //      void printEndCell() ;
 //      void printText(Text & text);
  //     void printPin(Pin & pin);
        virtual void printRect(Rect * rect);
//		virtual void printCell(Cell * rect);
		virtual void printReference(Cell * o);
        

      
    
  };
};

#endif
