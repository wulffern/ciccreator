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
#include "text.h"
#include <iostream>


namespace cIcCore{

        enum SortDirection {SORT_RIGHT, SORT_LEFT, SORT_BOTTOM, SORT_TOP};
        enum Offset {LOW, HIGH,NO_OFFSET};
        enum CutProperty { NO_START_CUT, NO_END_CUT, CENTER_CUT};
        enum RouteType { LEFT, RIGHT, LEFT_DOWN_LEFT_UP, STRAIGHT,VERTICAL,U_RIGHT, U_LEFT, ROUTE_UNKNOWN,LEFT_UP_LEFT_DOWN,STRAP, U_TOP, U_BOTTOM};
        enum Trim{NO_TRIM,TRIM_START_LEFT,TRIM_START_RIGHT, TRIM_END_LEFT, TRIM_END_RIGHT};


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
//              virtual QList<Rect*> addCuts(QList<Rect*>,QList<Rect*>&);
                        virtual QList<Rect*> addCuts(QList<Rect*>,QList<Rect*>&,int cuts_, int vcuts_);
                        void route() override;

                        void routeUHorizontal();
                        void addVertical(int x);
                        void applyOffset(int width, Rect* start, Offset offset);

                        void hasMatch(QString options);
                        int getIntegerFromMatch(QString regex,QString options, int defaultValue);
                        QString getQStringFromMatch(QString regex,QString options, QString defaultValue);

                protected:
                        QString routeLayer_ = "";
                        RouteType routeType_ = ROUTE_UNKNOWN;
                        QString route_;
                        QString net_ = "";
                        SortDirection sortDirection_;
                        Offset startOffset_;
                        Offset startOffsetCut_;
                        Offset endOffsetCut_;
                        Offset stopOffset_;
                        Trim startTrim_;
                        Trim endTrim_;
                        int track_;
                        bool hasTrack_;
                        int startCuts_;
                        int startVCuts_;
                        int endCuts_;
                        int endVCuts_;



                        int cuts_ = 2;
                        int vcuts_ = 1;
                        QString routeWidthRule_;
                        bool fillvcut_;
                        bool fillhcut_;
                        bool antenna_;
                        QString options_ = "";
                        QList<Rect*> start_rects_;
                        QList<Rect*> stop_rects_;
                        QList<Rect*> routes_;
                        QList<Rect*> startcuts;
                        QList<Rect*> endcuts;
                        QList<Rect*> add_after_route;
                        bool leftAlignCut;
                        QString startLayer_;
                        QString stopLayer_;


                        void add(Rect* r) override;
                        void add(QList<Rect*> rects) override;
                        void routeStraight();
                        void routeLeft();
                        void routeRight();
                        void routeVertical();
                        void routeLeftDownLeftUp();
                        void routeLeftUpLeftDown();
                        void routeStrap();
                        void routeStrapRects(Rect * sr, QList<Rect*> rects, bool start);
                                                Rect * cellFromJson(QJsonObject co) override;

        };

}

#endif
