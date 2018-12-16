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

#include "core/layer.h"

namespace cIcCore{

	Layer::Layer(){
        name = "M1";
        number = 0;
        material = diffusion;
        previous = "CO";
        next = "VIA1";
        pin = "M1_pin";
		res = "M1_res";
        color = "";
        nofill = false;
        visible = true;
      }

	// QPixmap Layer::icon(){
    //     return QPixmap::fromImage(drawColorIconProof(this->color, icon(200)));
    //   }
	
	// QImage Layer::icon(int size) {
    //      QImage image(size, size, QImage::Format_ARGB32_Premultiplied);
    //      image.fill(Qt::transparent);
    //      QPainter p(&image);
    //      p.setRenderHint(QPainter::Antialiasing);
    //      p.setPen(Qt::NoPen);
    //      p.translate(image.rect().center());
    //      p.scale(image.width()/2.2, image.height()/2.2);
    //      p.setBrush(Qt::white);
    //      p.drawRect(0,0,image.width(),image.height());
    //      return image;
    //   }

	// void Layer::drawColorIcon(QPainter & p, QColor color, const QImage & alpha)
    //   {
    //     p.save();
    //     p.setCompositionMode(QPainter::CompositionMode_SourceOver);
    //     p.fillRect(QRect(0, 0, alpha.width(), alpha.height()), color);
    //     p.setCompositionMode(QPainter::CompositionMode_DestinationIn);
    //     p.drawImage(0, 0, alpha);
    //     p.restore();
    //   }

	// QImage Layer::drawColorIconProof(QColor color, const QImage & alpha) {
    //      QImage result(alpha.size(), alpha.format());
    //      QPainter p(&result);
    //      drawColorIcon(p, color, alpha);
    //      p.setPen(Qt::NoPen);
    //      QBrush brush;
    //      //brush.setTextureImage(checkers(10));
    //      p.setCompositionMode(QPainter::CompositionMode_DestinationAtop);
    //      p.fillRect(alpha.rect(), brush);
    //      return result;
    //   }


}

