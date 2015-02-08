/*********************************************************************
 *        Copyright (c) 2015 Carsten Wulff Software, Norway
 * *******************************************************************
 * Created       : wulff at 2015-1-26
 * *******************************************************************
 * Licensed under the Apache License, Version 2.0 (the "License")
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ********************************************************************/



#ifndef CIC_CORE_RULES_H
#define CIC_CORE_RULES_H

#include <QtCore>
#include <QObject>
#include <QString>
#include <QJsonObject>

namespace cIcCore{

    class Rules: public QObject
    {
        Q_OBJECT;

    public:
        Rules();
        void load(QString filename);

    private:

    signals:

    };
}
#endif
