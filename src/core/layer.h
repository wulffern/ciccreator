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

#ifndef CIC_CORE_LAYER_H
#define CIC_CORE_LAYER_H

#include <QImage>
#include <QPainter>


namespace cIcCore{

  class Layer
  {
    public:
      QString name;
      int number;
      int datatype;
      enum MATERIAL_TYPE {diffusion, poly, metal, cut, metalres,other,marker,implant};
      MATERIAL_TYPE material;
      QString previous;
      QString next;
      QString pin;
      QString color;
      bool nofill;
      bool visible;

      Layer();
      QPixmap icon();
  private:

      QImage icon(int size);
      void drawColorIcon(QPainter & p, QColor color, const QImage & alpha);
      QImage drawColorIconProof(QColor color, const QImage & alpha) ;
  };

}
#endif // LAYER_H

