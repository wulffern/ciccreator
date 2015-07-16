//====================================================================
//        Copyright (c) 2015 Carsten Wulff Software, Norway 
// ===================================================================
// Created       : wulff at 2015-4-6
// ===================================================================
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//====================================================================

#ifndef CIC_CORE_RULES_H
#define CIC_CORE_RULES_H

#include <QObject>
#include <QJsonObject>
#include <QFile>
#include <QJsonDocument>
#include <QDebug>

namespace cIcCore{

	class Rules: public QObject
	{

	    Q_OBJECT

	public:
	  Rules(){
	   // this->rules_ = 0;
//		  rules_ = 0;
	     // QHash<QString, QHash<QString,qreal> > myrules;
	    //rules_ = myrules;
//	    rules_ = new QString("test");
		  //rules_(new  ());
	  }
	  ~Rules(){}
	  Rules(const Rules&){}

	  static void loadRules(QString path);

	  static Rules* getRules(){return myRules_;}

	  qreal get(QString layer, QString rule);
	  int gamma(){return gamma_;}
	  int grid(){return grid_;}
	  void setRules(QJsonObject job);

        private:
        static Rules * myRules_;
          //QJsonObject rules_;
//          qreal rules_;
         // QString * rules_;
          QHash<QString, QHash<QString,qreal> >  rules_;
          int gamma_;
          int grid_;
          //QJsonObject layers_;
          //QJsonObject technology_;

        };



}

#endif // RULES_H

