//====================================================================
//        Copyright (c) 2015 Carsten Wulff Software, Norway
// ===================================================================
// Created       : wulff at 2015-04-03
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
#include "core/patterntile.h"

namespace cIcCore {

    PatternTile::PatternTile()
    {
      ymax_ = 0;
      xmax_ = 0;
      yoffset_ = 0;

    }

    PatternTile::~PatternTile()
    {
	}

	PatternTile::PatternTile(const PatternTile&)
    {

    }

    QHash<QString,QVariant> PatternTile::initFillCoordinates(){
      QHash<QString,QVariant> qh;
return qh;
   }

    void PatternTile::onFillCoordinate(QChar c, QString layer, int x, int y, QHash<QString,QVariant> data){

    }

    void PatternTile::endFillCoordinate(QHash<QString,QVariant> data){
    }



    void PatternTile::fillCoordinatesFromString(QJsonArray ar){

      QHash<QString,QVariant> data = this->initFillCoordinates();

      //TODO: implement copyRows function

     // my $ref = $self->copyRows;

//       for (my $i=0;$i<scalar(@_);$i+=1) {
//         my $str = $_[$i];
//         push(@strs,$str);
//         if ($ref) {
//           next if $i < $ref->{offset};
//           push(@copyRows,$str);
//           if ($i== ($ref->{offset} + $ref->{length})) {
//             for (my $z = 0;$z < $ref->{count};$z++) {
//               foreach my $s (@copyRows) {
//                 push(@strs,$s);
//               }
//             }
//           }
//           next if $i >= ($ref->{offset} + $ref->{length});
//         }
//       }

      //TODO: implement copy columns
      //TODO: implement reverse string

      QString layer = ar[0].toString();
      QList<QString> strs;
      for(int i=1;i<ar.count();i++){
          QString str = ar[i].toString();

          for(int x=0;x < str.length();x++){
              QChar c = str[x];
              int y = ar.count() - i - 2;

              if(y > ymax_){ ymax_ = y;}
              if(x > xmax_){ xmax_ = x;}

              if(c.isDigit()){
                 continue;
                }

              if(c != '-'){
                  this->onFillCoordinate(c,layer,x,y,data);
               }


            }
        strs.append(str);
        }

      layers_[layer] = strs;

      //qWarning() << " ymax = " << _ymax << ", xmax = "<< _xmax;
      this->endFillCoordinate(data);

    }

    void PatternTile::paint(){

        int xspace = this->xspace_;
        int yspace = this->yspace_;

        foreach(QString layer, layers_.keys()){
            QList<QString> strs = layers_[layer];

            for(int x=0;x < xmax_;x++){
                for(int y=0;y < ymax_;y++){
                    QString s = strs[strs.length() - y - 1];
                    qDebug() << layer << s;

                QChar c = s[x];
                qDebug() << layer << c;
                Rect* rect = new Rect();
                rect->setLayer(layer);
                int xs = (x + xoffset_)*xspace;
                int ys = (y + yoffset_)*yspace;
                switch(c.unicode()){
                  case 'X':
                  case 'x':
                     rect->setRect(xs,ys,xspace,yspace);
                  case 'V':
                     rect->setRect(xs,ys - yspace/2,xspace,yspace*2);
                  case 'm':
                      rect->setRect(xs,ys,xspace,this->minPolyLength());
                      rect->moveCenter(xs + xspace/2, ys + yspace/2);
                  case 'w':
                    int minw = rules->get(layer,"width");
                    rect->setRect(xs,ys,xspace,minw);
                    rect->moveCenter(xs + xspace/2, ys + yspace/2);
                    currentHeight_ = minw;
                  }
                this->add(rect);



               }
            }

          }

    }

}
