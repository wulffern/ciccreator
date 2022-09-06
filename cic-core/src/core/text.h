//====================================================================
//        Copyright (c) 2015 Carsten Wulff Software, Norway 
// ===================================================================
// Created       : wulff at 2015-8-20
// ===================================================================
//   This program is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation, either version 3 of the License, or
//   (at your option) any later version.
// 
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
// 
//   You should have received a copy of the GNU General Public License
//   along with this program.  If not, see <http://www.gnu.org/licenses/>.
//====================================================================

#ifndef CIC_CORE_TEXT
#define CIC_CORE_TEXT

#include <QObject>
#include <QString>
#include <QtCore>
#include "rect.h"

namespace cIcCore{

	/*!
	  Text is just a text
	 */
  class Text: public Rect
  {
    Q_OBJECT

  public:

	  Text();
	  ~Text();
	  explicit Text(QString name);

	  //! Net name
	  QString name();
	  void setName(QString name);
	  
	  QJsonObject toJson() override;
	  void fromJson(QJsonObject o) override;

  protected:
	  QString name_;	  
  };



}

Q_DECLARE_METATYPE(cIcCore::Text)

#endif // Text

