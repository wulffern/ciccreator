//====================================================================
//        Copyright (c) 2016 Carsten Wulff Software, Norway
// ===================================================================
// Created       : wulff at 2016-12-5
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



#ifndef CIC_CORE_CELLPAINTER_H
#define CIC_CORE_CELLPAINTER_H

#include "core/layoutcell.h"
#include <QtCore>
#include <QPainter>
#include <QColor>
#include <QObject>
#include <QString>
#include <cmath>
#include <QImageWriter>
#include <QImage>

namespace cIcPainter{
    using namespace cIcCore;
    
    
    class CellPainter : public QObject{
    public:
        CellPainter(){
        }
        ~CellPainter(){
        }

        virtual void startCell(QPainter &painter,Cell*);
        virtual void endCell(QPainter &painter);
        virtual void paintChildren(QPainter &painter, QList<Rect*> children,QString hierarchy);
        
        virtual void paintPort(QPainter &painter,Port *);
        virtual void paintText(QPainter &painter,Text *);

                    //virtual void paint(QPainter &painter, Cell *,int x, int y, int width, int height,QString instanceName);
                    //virtual void paint(QPainter &painter, Cell *,int x, int y, int width, int height);
        virtual void paintCell(QPainter &painter, Cell *,QString hierarchy);
        virtual void paintRect(QPainter &painter, Rect * rect);
        virtual void paintReference(QPainter &painter,Instance *,QString hiearchy);
        void paintMyText(QPainter &painter,int x, int y, QString txt, QString colorName,int size);
        double toUnit(int angstrom);
        int unit = 1;


    private:
        QString _instanceName = "";
        bool _paint = true;
        QString _hasPainted = "";



    };
};

#endif
