//====================================================================
//        Copyright (c) 2015 Carsten Wulff Software, Norway
// ===================================================================
// Created       : wulff at 2015-4-6
// ===================================================================
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//====================================================================

#ifndef CIC_CORE_RULES_H
#define CIC_CORE_RULES_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QJsonDocument>
#include <QDebug>
#include "layer.h"

namespace cIcCore{

    class Device{
    public:
        QString name;
        QStringList ports;

    };

    class Rules: public QObject
    {

        Q_OBJECT

    public:
        Rules();

        ~Rules(){}
        Rules(const Rules&){}

        static void loadRules(QString path);

        static Rules* getRules(){return myRules_;}

        Device * getDevice(QString dev);
        qreal get(QString layer, QString rule);
        bool hasRule(QString layer, QString rule);
		double  spiceUnit();
        QString layerToColor(QString    name);
        int layerToNumber(QString name);
        int layerToDataType(QString name);
        int gamma(){return gamma_;}
        int grid(){return grid_;}
        void setRules(QJsonObject job);
        Layer * getLayer(QString string);
        QMap<QString,Layer *> layers(){ return layers_;}
        QString getNextLayer(QString lay);
        double toMicron(int val);

    private:
        static Rules * myRules_;
        QMap<QString,Layer *> layers_;
        QHash<QString, QHash<QString,qreal> >  rules_;
        QMap<QString,Device* > devices_;
        int gamma_;
        int grid_;
		double spiceunit_;

    };



}

#endif // RULES_H
