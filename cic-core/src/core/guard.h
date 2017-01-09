//====================================================================
//        Copyright (c) 2016 Carsten Wulff Software, Norway 
// ===================================================================
// Created       : wulff at 2016-6-13
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
#ifndef CIC_CORE_GUARD_H
#define CIC_CORE_GUARD_H

#include <QObject>
#include "cell.h"
#include "instance.h"
#include "cut.h"
#include "text.h"
#include <iostream>


namespace cIcCore{

	class Guard : public Cell
	{
		Q_OBJECT

	public:
		Guard(Rect* r,QList<QString> layers);
		~Guard();

	protected:

	};

};

#endif
