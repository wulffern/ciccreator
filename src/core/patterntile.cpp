//====================================================================
//        Copyright (c) 2015 Carsten Wulff Software, Norway
// ===================================================================
// Created       : wulff at 2015-04-03
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
// See the License for the specific language g  overning permissions and
// limitations under the License.
//====================================================================
#include "core/patterntile.h"

namespace cIcCore {

    PatternTile::PatternTile()
    {
      _ymax = 0;
      _xmax = 0;
      _yoffset = 0;

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

      for(int i=1;i<ar.count();i++){
          QString str = ar[i].toString();

          for(int x=0;x < str.length();x++){
              QChar c = str[x];
              int y = ar.count() - i - 2;

              if(y > _ymax){ _ymax = y;}
              if(x > _xmax){ _xmax = x;}

              if(c.isDigit()){
                 continue;
                }

              if(c != '-'){
                  this->onFillCoordinate(c,layer,x,y,data);
               }

            }
        }

      qWarning() << " ymax = " << _ymax << ", xmax = "<< _xmax;
      this->endFillCoordinate(data);

    }

}
