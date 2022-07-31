//====================================================================
//        Copyright (c) 2015 Carsten Wulff Software, Norway
// ===================================================================
// Created       : wulff at 2015-7-6
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


#ifndef CIC_SPICE_SPICEOBJECT_H
#define CIC_SPICE_SPICEOBJECT_H

#include <QObject>
#include <QTextStream>
#include <QRegularExpression>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

namespace cIcSpice{

        class SpiceObject: public QObject
        {

                Q_OBJECT
                Q_PROPERTY(QString name READ name WRITE setName)
                Q_PROPERTY(QStringList spiceStr READ spiceStr WRITE setSpiceStr)
                Q_PROPERTY(QStringList nodes READ nodes WRITE setNodes)
                Q_PROPERTY(int lineNumber READ lineNumber WRITE setLineNumber)

                public:
                        SpiceObject();
                        SpiceObject(const SpiceObject&);
                        ~SpiceObject();

                        QString name();

                        virtual QJsonObject toJson();
                        void fromJson(QJsonObject o);

                        virtual QString setName(QString val);

                        int lineNumber();
                        int setLineNumber(int val);

                        QList<QString> spiceStr();      QList<QString> setSpiceStr(QList<QString>  val);

                        QStringList nodes();        QStringList setNodes(QStringList val);

                        QVariantMap properties();

                        QString spiceType();
                        void setSpiceType(QString type);


                        QString deviceName();       void setDeviceName(QString name);

                        virtual QString toSpice( QString instance, QStringList nodes);


                        virtual        void setProperty(QString key, int val);
                        virtual void setProperty(QString key, QString val);
                        virtual void setProperty(QString key, double val);
                        virtual bool hasProperty(QString key);
                        //QVariant getProperty(QString key);
                        virtual QString getPropertyString(QString key);
                        void setPrefix(QString prefix);


                protected:
                        QString spiceType_;
                        QString deviceName_;
                        QList<QString> _spice_str;
                        QVariantMap _properties;
                        QStringList _nodes;
                        QString prefix_;

                private:

                        QString _name;

                        QList<SpiceObject*> _children;
                        int _line_number;




        };


}

Q_DECLARE_METATYPE(cIcSpice::SpiceObject)


#endif
