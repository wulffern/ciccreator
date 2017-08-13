/*********************************************************************
 *        Copyright (c) 2010 Carsten Wulff Software, Norway
 * *******************************************************************
 * Created       : wulff at 2010-9-3
 * Modified      : $Author$ $Date$
 * Version       : $Revision$ $HeadURL$
 * Description   :
 ********************************************************************/

#ifndef GEDASYMBOLLIBRARY_H
#define GEDASYMBOLLIBRARY_H


#include <QString>
#include <QStringList>
#include <QDir>
#include <QFileInfo>
#include <QFileInfoList>
#include <QDebug>
#include "geda/gobject.h"

  class gEdaSymbolLibrary{


  private:

    QMap<QString,QString> library_;
    QMap<QString,GObject*> objectlibrary_;

    static gEdaSymbolLibrary * myLib;
    gEdaSymbolLibrary();

    void findFiles(QString & path);

  public:
    QMap<QString,QString> & library();
    QMap<QString,GObject*> & objectlibrary();

    void addObject(QString basename,GObject *o);

    static gEdaSymbolLibrary * instance();
  };


#endif // GEDASYMBOLLIBRARY
