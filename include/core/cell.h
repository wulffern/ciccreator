//====================================================================
//        Copyright (c) 2015 Carsten Wulff Software, Norway
// ===================================================================
// Created       : wulff at 2015-1-26
// ===================================================================
// Licensed under the Apache License, Version 2.0 (the "License")
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
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
    void translate(qreal dx, qreal dy);
    void mirrorX(qreal ax);
    void mirrorY(qreal ay);
    void moveTo(qreal ax, qreal ay);
    void moveCenter(qreal ax, qreal ay);
    QRect calcBoundingRect();
    QString toString();
    static Cell * createInstance();
    QString name(){return _name;}
    QString setName(QString val){ _name = val; return _name;}

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
