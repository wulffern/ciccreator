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

//#include <QImage>
//#include <QPainter>
#include <QString>
#include <QMap>


namespace cIcCore{

    /*!
	 Defines a layer, data for this object is usually loaded from the "layers" section in the technology file
     */
    class Layer
    {
    public:

        //!Name of layer, for example "M1"
        QString name;

        //!GDS layer number
        int number;

        //!GDS layer datatype
        int datatype;

        QMap<QString,int> datatypes;

        enum MATERIAL_TYPE {diffusion, poly, metal, cut, metalres,other,marker,implant};

        //!Type of material
        MATERIAL_TYPE material;

        //!Previous layer in routing stack, i.e CO for M1
        QString previous;

        //!Next layer in routing stack, i.e VIA1 for M1
        QString next;

        //!Name of pin layer for this layer, i.e. M1_pin
        QString pin;
		
		//!Name of resistor layer for this layer, i.e. M1_res
        QString res;

        //!Color of this layer to use in GUI, QColor names can be used
        QString color;

        //!Fill rectangle of this layer in GUI
        bool nofill;

        //!Whether this layer is visible in GUI
        bool visible;

        Layer();

        //!Makes a small icon of this layer that can be used in GUI
//        QPixmap icon();

    private:
//        QImage icon(int size);
//        void drawColorIcon(QPainter & p, QColor color, const QImage & alpha);
//        QImage drawColorIconProof(QColor color, const QImage & alpha) ;
    };

}
#endif // LAYER_H
