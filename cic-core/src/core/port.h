//====================================================================
//        Copyright (c) 2015 Carsten Wulff Software, Norway 
// ===================================================================
// Created       : wulff at 2015-8-20
// ===================================================================
//   This program is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation, either version 3 of the License, or
//   (at your option) any later version.
// 
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
// 
//   You should have received a copy of the GNU General Public License
//   along with this program.  If not, see <http://www.gnu.org/licenses/>.
//====================================================================

#ifndef CIC_CORE_PORT
#define CIC_CORE_PORT

#include <QObject>
#include <QList>
#include <QString>
#include <QtCore>
#include "rect.h"


namespace cIcCore{

	/*!
	  Port is a net name attached to a rectangle
	 */
  class Port: public Rect
  {
    Q_OBJECT

  public:

	  Port();
	  ~Port();
	  Port(QString name);

	  //! Net name
	  QString name();
	  void setName(QString name);
	  QString pinLayer();

	  virtual void set(Rect * r );
	  virtual Rect* get();
	  virtual Rect* get(QString layer);
      virtual QList<Rect*> getAll(QString layer);
      virtual void mirrorX(int ay);
      virtual void mirrorY(int ax);
      
	  void add(Rect* r);

	  bool isInstancePort(){
	    if(strcmp(this->metaObject()->className(),"cIcCore::InstancePort") == 0){
		return true;
	      }else if(this->inherits("cIcCore::InstancePort")){
		return true;
	      }
	      return false;
	  }

	  virtual QJsonObject toJson();
	   virtual void fromJson(QJsonObject o);


  protected:
	  QString name_;

	  Layer * routeLayer_;
	  Rect* rect_;

	  QList<Rect*> alternates_rectangles_;

  public slots:
        void updateRect();
	  
  };



}

Q_DECLARE_METATYPE(cIcCore::Port)

#endif // PORT

