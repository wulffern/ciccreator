/*********************************************************************
 *        Copyright (c) 2015 Carsten Wulff Software, Norway
 * *******************************************************************
 * Created       : wulff at 2015-03-03
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
#ifndef CIC_CORE_PATTERNTRANSISTOR_H
#define CIC_CORE_PATTERNTRANSISTOR_H

#include <QObject>
#include "cell.h"
#include "patterntile.h"

namespace cIcCore{
class PatternTransistor : public PatternTile
{
  Q_OBJECT
  Q_PROPERTY(QString mosType READ mosType WRITE setMosType)
  //Make the object, let's see how that works in Qt

public:
    PatternTransistor();
    ~PatternTransistor();


    QString mosType(){return mosType_;}
    void setMosType(QString mosType){mosType_ = mosType;}


private:
    QString mosType_;

};
}

Q_DECLARE_METATYPE(cIcCore::PatternTransistor)

#endif // PATTERNTRANSISTOR_H