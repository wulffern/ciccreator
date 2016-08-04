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

#ifndef CIC_CORE_LAYOUTCELL_H
#define CIC_CORE_LAYOUTCELL_H

#include <QObject>
#include "cell.h"
#include "instance.h"
#include "route.h"
#include "routering.h"
#include "graph.h"
#include "datatypes.h"

namespace cIcCore{

    class LayoutCell : public Cell
    {
        Q_OBJECT

    public:
        LayoutCell();
        LayoutCell(const LayoutCell&);
        ~LayoutCell() ;

        Q_INVOKABLE void setYoffsetHalf(QJsonValue obj);
        Q_INVOKABLE void noPowerRoute(QJsonValue obj);
		Q_INVOKABLE void addDirectedRoute(QJsonArray obj);
		Q_INVOKABLE void addConnectivityRoute(QJsonArray obj);
        Q_INVOKABLE void addPortOnRect(QJsonArray obj);
		Q_INVOKABLE void addVia(QJsonArray obj);
		Q_INVOKABLE void addConnectivityVia(QJsonArray obj);
		Q_INVOKABLE void addPortVia(QJsonArray obj);
		Q_INVOKABLE void addVerticalRect(QJsonArray obj);
		Q_INVOKABLE void addRouteRing(QJsonArray obj);
		Q_INVOKABLE void addPowerRing(QJsonArray obj);
		Q_INVOKABLE void addRouteConnection(QJsonArray obj);
		Q_INVOKABLE void addPowerConnection(QJsonArray obj);
        Q_INVOKABLE void trimRouteRing(QJsonArray obj);
        Q_INVOKABLE void addRectangle(QJsonArray obj);
        
        QList<Graph*> getNodeGraphs(QString regex);
        void noPowerRoute();
        Instance* addInstance(cIcSpice::SubcktInstance* ckt,int x, int y);

        void addRectangle(QString layer, int x1, int y1, int width, int height, QString angle);
        void addConnectivityRoute(QString layer,QString regex, QString routeType,QString options,QString cuts,QString excludeInstances, QString includeInstances);
        
        void trimRouteRing(QString path, QString location,QString whichEndToTrim);

		void addRouteRing(QString layer, QString name, QString location, int widthmult, int spacemult);
		void addPowerRing(QString layer, QString name, QString location, int widthmult);
		void addPowerConnection(QString name, QString includeInstances,  QString location);
		void addRouteConnection(QString path, QString includeInstances, QString layer, QString location, QString options);

		
        virtual void place();
        virtual void route();
        virtual  void addAllPorts();
        virtual void routePower();
        virtual void paint();

		QStringList expandBus(QString name);

        virtual QList<Rect*> findRectanglesByNode(QString node, QString filterChild,QString filterInstance);


		virtual void fromJson(QJsonObject obj);

        void addPowerRoute(QString net,QString excludeInstances);

	private:
        bool useHalfHeight;
		QHash<QString,Graph*> nodeGraph_;

        bool noPowerRoute_;

		void addToNodeGraph(Instance * inst);

    };

}

Q_DECLARE_METATYPE(cIcCore::LayoutCell)

#endif // LAYOUTCELL_H
