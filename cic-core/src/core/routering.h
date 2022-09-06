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
#ifndef CIC_CORE_ROUTE_RING_H
#define CIC_CORE_ROUTE_RING_H

#include <QObject>
#include "cell.h"
#include "instance.h"
#include "cut.h"
#include "text.h"
#include <iostream>


namespace cIcCore{

	class RouteRing : public Cell
	{


		Q_OBJECT

	public:

		RouteRing();
		RouteRing(QString layer,QString name, Rect* size,QString location,int xgrid, int ygrid, int width );
		~RouteRing();
		void addRoute(Rect* r, QString layer, QString options, QString location);
        void trimRouteRing(QString location,QString whichEndToTrim);
        void translate(int dx, int dy) override;
        void moveTo(int ax,int ay) override;
        Rect* get(QString location);
        Rect* getDefault();
        Rect* getPointer(QString location);


	protected:
		Rect* bottom = NULL;
		Rect* left = NULL;
		Rect* right = NULL;
		Rect* top = NULL;
        Rect* default_rectangle = NULL;

	};

};

#endif
