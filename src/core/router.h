//====================================================================
//        Copyright (c) 2015 Carsten Wulff Software, Norway 
// ===================================================================
// Created       : wulff at 2015-10-25
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

#ifndef CIC_CORE_ROUTER_H
#define CIC_CORE_ROUTER_H

#include <QObject>
#include "cell.h"
#include "instance.h"

namspace cIcCore{

	enum SortDirection {TOP_RIGHT, TOP_LEFT, BOTTOM_RIGHT, BOTTOM_LEFT};
	enum Offset {END_LOW, END_HIGH, START_LOW, START_HIGH};
	enum Cut { NO_START_CUT, NO_END_CUT, CENTER_CUT}
	enum RouteType { LEFT, RIGHT, LEFT_DOWN_LEFT_UP, STRAIGHT
	
	class Router : public Cell
	{
		Q_OBJECT

	public:
		Router();
		Router(const Router&);
		~Router();

		virtual void addStartCuts();
		virtual void addEndCuts();
		virtual void addCuts();
		virtual void route();
		

	protected:
		QString _layer;
		QString _net;

		SortDirections _sortDirections;
		Offset _offset;
		int _track;
		int _cuts;
		int _vcuts;
		bool _antenna;
		QString _options;
		QList<Rect*> _start_rects;
		QList<Rect*> _end_rects; 


	}

}

#endif
