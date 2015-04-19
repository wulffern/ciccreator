//====================================================================
//        Copyright (c) 2015 Carsten Wulff Software, Norway
// ===================================================================
// Created       : wulff at 2015-1-26
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

#ifndef CIC_CORE_CELL_H
#define CIC_CORE_CELL_H

#include <QObject>

#include "rect.h"

namespace cIcCore{

  class Cell: public Rect
  {
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName)

  public:
    Cell();
    Cell(const Cell&);
    ~Cell();
    Rect * getRect(QString layer);
    void add(Rect* rect);
    void translate(int dx, int dy);
    void mirrorX(int ax);
    void mirrorY(int ay);
    void moveTo(int ax, int ay);
    void moveCenter(int ax, int ay);
    QRect calcBoundingRect();
    QString toString();
    static Cell * createInstance();
    QString name(){return _name;}
    QString setName(QString val){ _name = val; return _name;}

    virtual void place();
    virtual void route();
    virtual void paint();
    virtual void addAllPorts();

  private:
    QList<Rect*> _children;
    QString _name;
    bool _hasPR;
    Rect* parent;

  signals:

  public slots:
    void updateBoundingRect();

  };

}

Q_DECLARE_METATYPE(cIcCore::Cell)

#endif
