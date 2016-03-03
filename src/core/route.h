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

#ifndef CIC_CORE_ROUTE_H
#define CIC_CORE_ROUTE_H

#include <QObject>
#include "cell.h"
#include "instance.h"
#include "cut.h"
#include <iostream>


namespace cIcCore{

	enum SortDirection {SORT_RIGHT, SORT_LEFT, SORT_BOTTOM, SORT_TOP};
	enum Offset {LOW, HIGH,NO_OFFSET};
	enum CutProperty { NO_START_CUT, NO_END_CUT, CENTER_CUT};
	enum RouteType { LEFT, RIGHT, LEFT_DOWN_LEFT_UP, STRAIGHT,VERTICAL,U_RIGHT, U_LEFT, ROUTE_UNKNOWN};
	
	class Route : public Cell
	{
		Q_OBJECT

	  void routeU();
	  void addHorizontalTo(int x, QList<Rect *> rects, Offset offset);
	  void routeOne();
	public:
		Route(QString net, QString layer, QList<Rect*> start, QList<Rect*> stop, QString options, QString routeType);
		Route(const Route&);
		~Route();

		virtual void addStartCuts();
		virtual void addEndCuts(); 
		virtual void addCuts(QList<Rect*>);
		virtual void route();

		void addVertical(int x);
		void applyOffset(int width, Rect* start, Offset offset);

		void hasMatch(QString options);
		int getIntegerFromMatch(QString regex,QString options, int defaultValue);
	protected:
		QString routeLayer_;
		RouteType routeType_;
		QString route_;
		QString net_;
		SortDirection sortDirection_;
		Offset startOffset_;
		Offset stopOffset_;
		int track_;
		int cuts_;
		int vcuts_;
		bool antenna_;
		QString options_;
		QList<Rect*> start_rects_;
		QList<Rect*> stop_rects_;

		void routeStraight();
		void routeLeft();
		void routeRight();
		void routeVertical();
	};

}

#endif
