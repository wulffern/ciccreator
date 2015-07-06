/*********************************************************************
 *        Copyright (c) 2014 Carsten Wulff Software, Norway
 * *******************************************************************
 * Created       : wulff at 2014-9-21
 * Modified      : $Author$ $Date$
 * Version       : $Revision$ $HeadURL$
 * Description   :
 ********************************************************************/

#include "printer/designprinter.h"


namespace cIcPrinter{


	void DesignPrinter::startCell(Cell * o){
	}

	void DesignPrinter::printReference(Cell * o){
	}
	
    void DesignPrinter::openFile(QString filename){
        file.setFileName(filename);
        file.open(QIODevice::WriteOnly );

    }

    void DesignPrinter::closeFile(){
        file.close();
        //if(!stream.is_open()){
        //    stream.close();
        // }

    }

    void DesignPrinter::printCell(Cell * c){

        this->startCell(c);

        foreach(Rect * child, c->children()){
			this->printRect(child);
        }
        this->endCell();
    }

    void DesignPrinter::print(Design * d ){

		
        foreach(Cell * r, d->designs()  ){
            this->printCell(r);
        }


    }

    //}


};
