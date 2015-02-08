//====================================================================
//        Copyright (c) 2015 Carsten Wulff Software, Norway 
// ===================================================================
// Created       : wulff at 2015-1-26
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


namespace cIcCore{


	Rules::Rules(){

		
		
	}

	void load(QString filename){
		QString val;
        QFile file;
        file.setFileName(filename);
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        val = file.readAll();
        file.close();
        QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());
        QJsonObject rules = d.object();
        QJsonValue cells = obj.take("cells");
        if(cells.isArray()){
            QJsonArray cellArray  = cells.toArray();
            foreach (const QJsonValue & value, cellArray) {
                QJsonObject c = value.toObject();

                qWarning() << c["class"];
                qWarning() << c["name"];

            }
        }else{
            qWarning() << "Could not find 'cells' array in json file\n";
        }

	}



}
