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

#include "core/rules.h"

namespace cIcCore{

   Rules * Rules::myRules_;

  void Rules::loadRules(QString filename){
  QString val;
  QFile file;
  file.setFileName(filename);
  file.open(QIODevice::ReadOnly | QIODevice::Text);
  val = file.readAll();
  file.close();
  QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());
  QJsonObject obj = d.object();
  Rules::myRules_ = new Rules();
  Rules::myRules_->setRules(obj);

}
  void Rules::setRules(QJsonObject job){

    QJsonObject tech = job["technology"].toObject();
    gamma_ = tech["gamma"].toInt();
    grid_ = tech["grid"].toInt();

    QJsonObject r = job["rules"].toObject();
    foreach(QString layer, r.keys()){
        QJsonObject vral = r[layer].toObject();
        foreach(QString name,vral.keys()){
          qreal v = vral[name].toDouble();
          this->rules_[layer][name] = v;
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


 qreal Rules::get(QString layer, QString rule){
   qreal v = 0;

   if(rules_.contains(layer)){
	   QHash<QString,qreal> lay =  rules_[layer];
	   
	   if(!lay.contains(rule)){
		   qDebug() << "Could not find rule "<< rule ;
	   }else{
//		   qDebug() << "rules" << rule;
		   v = lay[rule]*gamma_;
		   
	   }
   }else{
	   qDebug() << "Could not find rule " << rule << " for layer " << layer << ", no such layer";
   }
	 
   return v;
 }
}
