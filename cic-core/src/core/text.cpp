//====================================================================
//        Copyright (c) 2016 Carsten Wulff Software, Norway 
// ===================================================================
// Created       : wulff at 2016-4-30
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


#include "core/text.h"

namespace cIcCore{
	
	Text::Text(){
	  name_ = "";
	  this->setLayer("TXT");
    }

	Text::~Text(){
    }

	Text::Text(QString name){
	  name_ = name;
	  	  this->setLayer("TXT");
	}

    QString Text::name(){return name_;}

	void Text::setName(QString name){name_ = name;}

	void Text::fromJson(QJsonObject o){
	      Rect::fromJson(o);
	      name_ = o["name"].toString();
	}

	QJsonObject Text::toJson(){
	  QJsonObject o = Rect::toJson();
	  o["class"] = "Text";
	  o["name"] = name_;
	  return o;
	}
}


