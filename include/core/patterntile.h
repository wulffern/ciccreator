/*********************************************************************
 *        Copyright (c) 2015 Carsten Wulff Software, Norway
 * *******************************************************************
 * Created       : wulff at 2015-04-03
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
#ifndef CIC_CORE_PATTERNTILE_H
#define CIC_CORE_PATTERNTILE_H

#include <QJsonArray>
#include "cell.h"

namespace cIcCore{

  class PatternTile: public Cell
  {
    Q_OBJECT
    Q_PROPERTY(int minPolyLength READ minPolyLength WRITE setMinPolyLength)

  public:

    PatternTile();

    PatternTile(const PatternTile&);

    ~PatternTile();

    Q_INVOKABLE void fillCoordinatesFromString(QJsonArray ar);

    QHash<QString,QVariant> initFillCoordinates();
    void onFillCoordinate(QChar c, QString layer, int x, int y, QHash<QString,QVariant> data);
    void endFillCoordinate(QHash<QString,QVariant> data);
    int minPolyLength(){return minPolyLength_;}
    int setMinPolyLength(int val){
      minPolyLength_ = val;
      return minPolyLength_;
    }
    void paint() override;


  private:
    int minPolyLength_;
    int xmax_;
    int ymax_;
    qreal yoffset_;
    qreal xoffset_;
    int xspace_;
    int yspace_;
    int currentHeight_;
    QHash<QString,QList<QString> > layers_;

   // inline int xs(int x){  return (x + xoffset_)*xspace_;}
    //inline int ys(int y){ return (y + yoffset_)*yspace_;}
  };

}

Q_DECLARE_METATYPE(cIcCore::PatternTile)

#endif // PATTERNTILE_H
