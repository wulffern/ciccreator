/*********************************************************************
 *        Copyright (c) 2014 Carsten Wulff Software, Norway
 * *******************************************************************
 * Created       : wulff at 2014-9-21
 * Modified      : $Author$ $Date$
 * Version       : $Revision$ $HeadURL$
 * Description   :
 ********************************************************************/


#include "printer/svg.h"

namespace cIcPrinter{



	void Svg::printReference(Cell * o){

		QString str;
		QTextStream(&str)  << "<image x=\"" << o->x1() << "\" y=\"" << o->y1() << "\" width=\""<< o->width() << "\" height=\"" << o->height() << "\" xlink:href=\"" << o->name() << "\" />";

	}
	
    void Svg::printRect(Rect * o){
       
        this->add("<rect  x=\"");
        this->add(o->x1());
        this->add("\" y=\"");
        this->add(o->y1());
        this->add("\" width=\"");
        this->add(o->width());
        this->add("\" height=\"");
        this->add(o->height());

		if(o->layer() == "PO"){
			this->add("\" style=\"fill:red;stroke:red;opacity:0.5\"/>");
		}else if(o->layer() == "M1"){
			this->add("\" style=\"fill:blue;stroke:blue;opacity:0.5\"/>");
		}else if(o->layer() == "M2"){
			this->add("\" style=\"fill:goldenrod;stroke:goldenrod;opacity:0.5\"/>");
		}else if(o->layer() == "M3"){
			this->add("\" style=\"fill:aqua;stroke:aqua;opacity:0.5\"/>");
		}else if(o->layer() == "M4"){
			this->add("\" style=\"fill:darkgreen;stroke:darkgreen;opacity:0.5\"/>");
		}else if(o->layer() == "M5"){
			this->add("\" style=\"fill:brown;stroke:brown;opacity:0.5\"/>");
		}else if(o->layer() == "M6"){
			this->add("\" style=\"fill:darkviolet;stroke:darkviolet;opacity:0.5\"/>");
		}else if(o->layer() == "OD"){
			this->add("\" style=\"fill:green;stroke:green;opacity:0.5\"/>");
		}else if(o->layer() == "CO"){
			this->add("\" style=\"fill:yellow;stroke:yellow;opacity:0.9\"/>");
		}else{
			this->add("\" style=\"fill:grey;stroke:grey;opacity:0.1\"/>");
		}
		
		
    };


    void Svg::startCell(Cell *cell){
        DesignPrinter::startCell(cell);
        QString file = cell->name();
        file.append(".svg");
        this->openFile(file);
		this->add("<?xml version=\"1.0\" standalone=\"no\"?>\n"
					 "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \n"
					 "\"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n"
					 "<svg  width=\"1000\" height=\"1000\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" version=\"1.1\">\n");
		this->add("<g id=\"");
		this->add(cell->name());
		this->add("\">\n");
		
    }
    
    
    
    
	void Svg::endCell(){
        this->add("</g>\n");
        this->add("</svg>\n");
        this->closeFile();
        
	}


};
