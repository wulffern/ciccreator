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
        xoffset_ = 0;
        xspace_ = 0;
        yspace_ = 0;
        minPolyLength_ = 0;
        widthoffset_ = 0;
        heightoffset_ = 0;

    }

    Rect PatternTile::calcBoundingRect(){


        int x1  = 0;
        int y1  = 0;
        int x2  = (xmax_ + widthoffset_)*xspace_;
        int y2  = (ymax_ + heightoffset_)*yspace_;

        Rect r;
        r.setPoint1(x1,y1);
        r.setPoint2(x2,y2);
//        qWarning() << "Testing" << r.toString();
        return r;
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
        for(int i=0;i<ar.count();i++){
            QString str = ar[i].toString();

            for(int x=0;x < str.length();x++){
                QChar c = str[x];
                int y = ar.count() - i -1 ;

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

//		ymax_ += 1;
		xmax_ += 1;
		
        layers_[layer] = strs;

        //qWarning() << " ymax = " << _ymax << ", xmax = "<< _xmax;
        this->endFillCoordinate(data);

    }

    void PatternTile::paint(){

        //Load rules
        this->xspace_ = this->rules->get("ROUTE","horizontalgrid");
        this->yspace_ = this->rules->get("ROUTE","verticalgrid");

        if( this->minPolyLength_ == 0 ){
            this->minPolyLength_ = this->rules->get("PO","mingatelength");
        }


        int minpoly = this->minPolyLength();
        int currentHeight_ = yspace_;
        foreach(QString layer, layers_.keys()){
            QList<QString> strs = layers_[layer];

			for(int y=0;y < ymax_;y++){
	    currentHeight_ = yspace_;
				for(int x=0;x < xmax_;x++){
                    QString s = strs[strs.length() - y - 1];

                    QChar c = s[x];

                    Rect* rect = new Rect();
                    rect->setLayer(layer);
                    int xs = (x + xoffset_)*xspace_;
                    int ys = (y + yoffset_)*yspace_;


                    switch(c.unicode()){
                    case 'X':
                        currentHeight_ = yspace_;
                    case 'x':
                        currentHeight_ = yspace_;
                    case 'B':
                    case 'A':
                    case 'D':
                    case 'S':
                    case 'G':
                    case 'r':
                    case 'K':
                    case 'C':
                    case 'c':
                        rect->setRect(xs,ys,xspace_,currentHeight_);
                        rect->moveCenter(xs + xspace_/2, ys + yspace_/2);
                        break;
                    case 'V':
                        rect->setRect(xs,ys - yspace_/2,xspace_,yspace_*2);
                        break;
                    case 'm':
                        rect->setRect(xs,ys,xspace_,this->minPolyLength());
                        rect->moveCenter(xs + xspace_/2, ys + yspace_/2);
                        break;
                    case 'w':
                        int minw = rules->get(layer,"width");
                        rect->setRect(xs,ys,xspace_,minw);
                        rect->moveCenter(xs + xspace_/2, ys + yspace_/2);
                        currentHeight_ = minw;
                        break;

                    }

                    if(!rect->empty()){
                        this->add(rect);
                    }

                    int cxoffset = 0;

                    int cw = 0;
                    int ch = 0;
                    int cs = 0;
                    QString lay;
                    Rect *cr;
                    Rect *cr1;
                    switch(c.unicode()){
                    case 'C':
                        cxoffset = xspace_/2;
                    case 'c':
                        //TODO: Get next layer based on current
                        //QString lay = this->rules->get
                        lay = "CO";
                        cr = new Rect();
                        cr->setLayer(lay);
                        cw = this->rules->get(lay,"width");
                        ch = this->rules->get(lay,"height");
                        cs = this->rules->get(lay,"space");
                        cr->setRect(xs,ys,cw,ch);
                        cr->moveCenter(xs -cxoffset + xspace_/2, ys + yspace_/2);
                        this->add(cr);

                        break;
                    case 'K':
                        cxoffset = xspace_/2;
                    case 'k':
                        //TODO: Get next layer based on current
                        //QString lay = this->rules->get
                        lay = "CO";
                        cr = new Rect();
                        cr->setLayer(lay);
                        cw = this->rules->get(lay,"width");
                        ch = this->rules->get(lay,"height");
                        cs = this->rules->get(lay,"space");
                        cr->setRect(xs,ys,cw,ch);
                        cr1 = cr->getCopy();
                        cr->moveCenter(xs -cxoffset + xspace_/2, ys + yspace_/2);
                        cr1->moveCenter(cr->centerX() - cs - cw/2,cr->centerY());
                        this->add(cr);
                        this->add(cr1);
                        break;
                    }





                }
            }


        }
        this->updateBoundingRect();

    }

}
