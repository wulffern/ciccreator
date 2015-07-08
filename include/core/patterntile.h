/*********************************************************************
 *        Copyright (c) 2015 Carsten Wulff Software, Norway
 * *******************************************************************
 * Created       : wulff at 2015-04-03
 * *******************************************************************
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
    void paint() ;


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
