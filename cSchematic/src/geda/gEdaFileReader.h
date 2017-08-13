/*********************************************************************
 *        Copyright (c) 2010 Carsten Wulff Software, Norway
 * *******************************************************************
 * Created       : wulff at 2010-8-25
 * Modified      : $Author$ $Date$
 * Version       : $Revision$ $HeadURL$
 * Description   :
 ********************************************************************/

#ifndef GEDAFILEREADER_H
#define GEDAFILEREADER_H

#include <QObject>
#include <QList>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include "geda/gobject.h"
#include "gEdaObjectFactory.h"
#include "gEdaSymbolLibrary.h"

  class gEdaFileReader
  {

  private:

  public:
    //--------------------------------------------------------
    // ctor and dtor
    //--------------------------------------------------------
    gEdaFileReader(){
    }

    ~gEdaFileReader(){

    }


    GObject * readFile(QString & file_){

      GObject * parent = new GObject();
      readFile(file_,parent);

      return parent;

    }

    void readFile(QString & file_, GObject * parent){
      QFile myFile(file_);

      if ( ! myFile.exists() ){
        qDebug() << "File does not exists '" << file_<< "'\n";
      }

      if (!myFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

      QTextStream in(&myFile);
      QString line = in.readLine();
      gEdaObjectFactory gof;
      while (!line.isNull()) {
        //Remove whitespace from start and end
        line = line.trimmed();
        GObject * g = gof.getObject(line,in);

        if(g != 0)
          {
	    parent->addChild(g);
	    if(g->type == OBJ_COMPLEX)
	      {
		GComponent * c = dynamic_cast<GComponent*>(g);
		readSymbolFile(c);
	      }
          }
        line = in.readLine();
      }
      myFile.close();
      return ;
    }

    void readSymbolFile(GComponent *c){
      gEdaSymbolLibrary *sym = gEdaSymbolLibrary::instance();
      if(sym->library().contains(c->basename)){

        //Make symbol cache
        if(!sym->objectlibrary().contains(c->basename)){
          GObject * o = new GObject();
          QString basename = sym->library().value(c->basename);
          readFile(basename,o);
          sym->addObject(c->basename,o);
        }
	
        GObject *o = sym->objectlibrary().value(c->basename);
	c->symbol = o;
      }else{
	c->symbol = 0;
      }
    }




  };



#endif // GEDAFILEREADER
