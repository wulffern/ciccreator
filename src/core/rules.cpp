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

 int Rules::get(QString layer, QString rule){
   int val = 0;
qDebug() << "test";
   qDebug() << rules_;
   qDebug() << "test";
   QJsonObject lay =  rules_[layer].toObject();

   if(lay.empty()){
      qDebug() << "Could not find rules for layer " << layer;
     }else{
   if(lay[rule].isUndefined()){
       qDebug() << "Could not find rule" << rule << " for layer " << layer ;
     }else{
         val = lay[rule].toInt();
     }
     }
  return val;
 }
}
