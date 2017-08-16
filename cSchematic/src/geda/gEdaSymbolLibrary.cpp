/*********************************************************************
 *        Copyright (c) 2010 Carsten Wulff Software, Norway
 * *******************************************************************
 * Created       : wulff at 2010-9-3
 * Modified      : $Author$ $Date$
 * Version       : $Revision$ $HeadURL$
 * Description   :
 ********************************************************************/
#include "gEdaSymbolLibrary.h"

gEdaSymbolLibrary * gEdaSymbolLibrary::myLib = 0;

gEdaSymbolLibrary::gEdaSymbolLibrary(){

  QStringList dirs;
  dirs.append("/Users/wulff/pro/ciccreator/lay");

  foreach(QString d, dirs){
    findFiles(d);
  }

}

void gEdaSymbolLibrary::findFiles(QString & path){
  QStringList filter ;
  filter << "*.sym";

  QDir d(path);
  d.setNameFilters(filter);
  QFileInfoList files =  d.entryInfoList(QDir::Files|QDir::Readable,QDir::DirsFirst);
  foreach(QFileInfo f, files){
    library_.insert(f.fileName(),f.filePath());
  }

  QFileInfoList dirs =  d.entryInfoList(QDir::AllDirs|QDir::NoDotAndDotDot);
  foreach(QFileInfo f, dirs){
    QString s = f.filePath();
    findFiles(s);

  }

}

QMap<QString,QString> & gEdaSymbolLibrary::library(){
  return library_;
}

QMap<QString,GObject*> & gEdaSymbolLibrary::objectlibrary(){
  return objectlibrary_;
}

void gEdaSymbolLibrary::addObject(QString basename,GObject *o){
  objectlibrary_.insert(basename,o);
}

gEdaSymbolLibrary * gEdaSymbolLibrary::instance(){


  if(myLib == 0){
    myLib = new gEdaSymbolLibrary();
  }

  return myLib;
}
