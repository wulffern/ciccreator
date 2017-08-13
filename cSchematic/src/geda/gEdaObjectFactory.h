/*********************************************************************
 *        Copyright (c) 2010 Carsten Wulff Software, Norway
 * *******************************************************************
 * Created       : wulff at 2010-8-25
 * Modified      : $Author$ $Date$
 * Version       : $Revision$ $HeadURL$
 * Description   :
 ********************************************************************/

#ifndef GEDAOBJECTFACTORY_H
#define GEDAOBJECTFACTORY_H

#include <QStack>
#include "defines/defines.h"
#include "geda/gobject.h"
#include <QDebug>
#include <QString>
#include <QByteArray>

  class gEdaObjectFactory{

  private:
    QStack<GObject*> stack_;
    GObject * lastObject_;

  public:

    gEdaObjectFactory(){
      lastObject_ = 0;
    }


    GObject*  getObject(QString line, QTextStream & in){

      GObject * obj = 0;
      QStringList s = line.split(QRegExp("\\s+"));

      if(s[0] == OBJ_LINE ){
        obj = new GLine(s);
      }
      else if(s[0] == COMMENT){
        obj = new GObject();
        obj->orgstring = line;
        obj->string = line;
      }
      else if(s[0] == OBJ_VERSION){
        obj = new GVersion(s);
      }
      else if(s[0] == OBJ_NET ){
        obj = new GNet(s);
      }
      else if(s[0] == OBJ_BOX ){
        obj = new GBox(s);
      }
      else if(s[0] == OBJ_COMPLEX ){
        obj = new GComponent(s);
      }
      else if (s[0] == OBJ_PIN){
        obj = new GPin(s);
      }
      else if ( s[0] ==  STARTATTACH_ATTR){
        if(lastObject_ != 0){
          stack_.push(lastObject_);
        }
        return 0;
      }
      else if ( s[0] == ENDATTACH_ATTR){
        if(lastObject_ != 0){
          stack_.pop();
        }
        return 0;
      }
      else if ( s[0] == OBJ_TEXT){
        obj = new GText(s,in);
      }
      else {
	//        qDebug() << "Unknown object: ' " << line << "'";
        return 0;
      }

      lastObject_ = obj;

      if(!stack_.isEmpty()){
        stack_.top()->addChild(obj);
        return 0;
      }
      return obj;

    }


  };

#endif // GEDAOBJECTFACTORY
