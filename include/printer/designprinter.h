/*********************************************************************
 *        Copyright (c) 2014 Carsten Wulff Software, Norway
 * *******************************************************************
 * Created       : wulff at 2014-9-20
 * Modified      : $Author$ $Date$
 * Version       : $Revision$ $HeadURL$
 * Description   :
 ********************************************************************/

#ifndef CIC_PRINTER_DESIGNPRINTER
#define CIC_PRINTER_DESIGNPRINTER

#include <QObject>
#include <QFile>
#include <QTextStream>
#include "core/design.h"

namespace cIcPrinter{
	using namespace cIcCore;
    class DesignPrinter : public QObject{
    private:


    protected:
        Cell * current_cell;
        QString filename;
        QFile  file;
	  
//        ofstream  stream;

    public:

         DesignPrinter(QString filename){
          this->filename = filename;

         }

     
        virtual ~DesignPrinter(){
            //file.close();
           //stream.flush();
//           if(stream.cl)
//            if(stream.is_open()){
//                stream.close();
//            }
        }

		
		virtual void add(QString str){
	   QTextStream s(&file);
            s << str;
        }

		
        virtual void add(QString & str){
           QTextStream s(&file);
            s << str;
        }
        
        
        virtual void add(int i){
           QTextStream s(&file);
            s << i;
//            if(this->stream.is_open()){
//                this->stream << i;
//            }
        }
        

        void print(Design * o);

        virtual void openFile(QString file);
        virtual void closeFile();
        


        virtual void startCell(Cell * cell);
        virtual void endCell() = 0;
//      virtual void endFile()  = 0;
        //     virtual void printInstance(Instance & inst)  = 0;
        //     virtual void printInitLib(string name) = 0;
        //    virtual void printEndLib() = 0;
        //   virtual void printInitCell(Cell & cell) = 0;
        //   virtual void printEndCell() = 0;
        //   virtual void printText(Text & text) = 0;
        //  virtual void printPin(Pin & pin) = 0;
       virtual void printRect(Rect * rect) = 0;
       virtual void printCell(Cell * c);
		virtual void printReference(Cell *c);



    };
};

#endif
