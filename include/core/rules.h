//====================================================================
//        Copyright (c) 2015 Carsten Wulff Software, Norway 
// ===================================================================
// Created       : wulff at 2015-4-6
// ===================================================================
// Licensed under the Apache License, Version 2.0 (the "License")
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
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

	  void setRules(QJsonObject job){
	    QJsonObject r = job["rules"].toObject();

	    foreach(QString layer, r.keys()){
		QJsonObject vral = r[layer].toObject();
		foreach(QString name,vral.keys()){
		  qreal v = vral[name].toDouble();
		  this->rules_[name][layer] = v;
		//  if(this->rules_.contains(layer)){
		//	QHash<QString,qreal> rl = this->rules_[name];
		//	rl[name] = v;
		 // }

		  //rules_ = v;
		  }
	    }
	     // rules_ = job["rules"].toObject();
	     // layers_ = job["layers"].toObject();
	     // technology_ = job["technology"].toObject();
	  }

        private:
        static Rules * myRules_;
          //QJsonObject rules_;
//          qreal rules_;
         // QString * rules_;
          QHash<QString, QHash<QString,qreal> >  rules_;
          //QJsonObject layers_;
          //QJsonObject technology_;

        };



}

#endif // RULES_H

