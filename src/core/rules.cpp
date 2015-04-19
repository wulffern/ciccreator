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



 qreal Rules::get(QString layer, QString rule){
   qreal v = 0;

    if(rules_.contains(layer)){
   QHash<QString,qreal> lay =  rules_[layer];

  if(!lay.contains(rule)){
      qDebug() << "Could not find rules for layer " << layer;
     }else{
        v = lay[rule];

     }
      }
  return v;
 }
}
