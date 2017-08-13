//====================================================================
//        Copyright (c) 2010 Carsten Wulff Software, Norway
// ===================================================================
// Created       : wulff at 2010-9-19
// Modified      : $Author$ $Date$
// Version       : $Revision$ $HeadURL$
// Description   :
//====================================================================

#include "gEdaFileWriter.h"
#include <QFile>
#include <QTextStream>

gEdaFileWriter::gEdaFileWriter(){
}

gEdaFileWriter::~gEdaFileWriter(){

}

void gEdaFileWriter::writeFile(QString & str, GObject * o){

  QFile file ( str );
  if ( file.open( QIODevice::WriteOnly | QIODevice::Truncate) )
    {
      QTextStream stream( &file );
      foreach(GObject * child, o->children()){
        stream << child->toString() << "\n";
      }//
      file.close();

    }
}
