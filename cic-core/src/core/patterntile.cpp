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

    QMap<QString,QStringList> PatternTile::Patterns;


    PatternTile::PatternTile():Cell()
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
        mirrorPatternString_ = 0;
        polyWidthAdjust_ = 1;

        verticalGrid_ = 0;
        horizontalGrid_ = 0;
        verticalGridMultiplier_ = 1;
        horizontalGridMultiplier_ = 1;


        prev_rect_  = 0;
        _has_pr = true;

    }

    Rect PatternTile::calcBoundingRect(){


        int x1  = 0;
        int y1  = 0;
        int x2  = ((float)xmax_ + (float)widthoffset_)*(float)xspace_;
        int y2  = ((float)ymax_ + (float)heightoffset_)*(float)yspace_;

        Rect r;
        r.setPoint1(x1,y1);
        r.setPoint2(x2,y2);
        return r;
    }


    PatternTile::~PatternTile()
    {
    }

    void PatternTile::copyColumn(QJsonObject obj){
        CopyColumn c;
        c.count =obj["count"].toInt();
        c.length = obj["length"].toInt();
        c.offset= obj["offset"].toInt();
        copyColumn_.append(c);
    }

    PatternTile::PatternTile(const PatternTile&)
    {

    }

    QMap<QString,QVariant> PatternTile::initFillCoordinates(){
        QMap<QString,QVariant> qh;
        return qh;
    }


    void PatternTile::getRuleForHorizontalGrid(QJsonArray ar)
    {
        if(ar.size() < 2){
            qDebug() << "Error: getRuleForHorizontalGrid must contain at least to elements\n";
            return;
        }

        QString layer = ar[0].toString();
        QString rule = ar[1].toString();
        horizontalGrid_ = this->getRules()->get(layer,rule);

    }

    void PatternTile::getRuleForVerticalGrid(QJsonArray ar)
    {
        if(ar.size() < 2){
            qDebug() << "Error: getRuleForVerticalGrid must contain at least to elements\n";
            return;
        }

        QString layer = ar[0].toString();
        QString rule = ar[1].toString();
        verticalGrid_ = this->getRules()->get(layer,rule);

    }


    //TODO: How dow I figure out how the patterns should be translated
    //into rectangles? I guess it's a quantization of the rectangle,
    //so some sort of loop that adds sub-rectangles?
    void PatternTile::readPatterns()
    {
        foreach(const QString & key, Patterns.keys()){
            QStringList sl = Patterns[key];
            int ycount = sl.count();
            int xcount = 0;


            foreach(const QString & s, sl){
                int xc = s.length();
                if(xc > xcount){
                    xcount = xc;
                }
            }
            PatternData* d = new PatternData();

            d->xcount = xcount;
            d->ycount = ycount;
            d->name = key[0];
            d->pattern = sl;

            Pattern[key[0]] = d;
        }


    }

    int arraylength = 0;


    void PatternTile::fillCoordinatesFromString(QJsonArray ar){


        //Load rules
        this->xspace_ = this->rules->get("ROUTE","horizontalgrid")*horizontalGridMultiplier_;
        this->yspace_ = this->rules->get("ROUTE","verticalgrid")*verticalGridMultiplier_;


        if(horizontalGrid_ != 0){
            this->xspace_ = horizontalGrid_;
        }

        if(verticalGrid_ != 0){
            this->yspace_ = verticalGrid_;
        }
        if( this->minPolyLength_ == 0 ){
            this->minPolyLength_ = this->rules->get("PO","mingatelength");
        }


        QMap<QString,QVariant> data = this->initFillCoordinates();

        //TODO: implement copyRows function
        QString layer = ar[0].toString();
        ar.pop_front();

        if(arraylength == 0) arraylength = ar.count();

       if(arraylength != ar.count()){
           qDebug() << " Error: " << " layer " << layer << " does not have " << arraylength << " lines \n";
           
           
       }
        
        
        QList<QString> strs;
        for(int i=0;i<ar.count();i++){
            QString str = ar[i].toString();


            //Copy columns
            if(copyColumn_.length() > 0){
                for(int z=0;z<copyColumn_.length();z++){
                    CopyColumn c = copyColumn_[z];
                    if(str.length() < c.offset);
                    QString sorg = str.mid(c.offset,c.length);
                    for(int x=0;x<c.count;x++){
                        str.insert(c.offset,sorg);
                    }
                }
            }

            if(this->mirrorPatternString()){
                QString tmp;
                for(int i=str.length()-1;i >= 0; i--){
                    tmp.append(str[i]);
                }
                str = tmp;
            }

            for(int x=0;x < str.length();x++){
                QChar c = str[x];
                int y = ar.count() - i -1 ;

                if(y > ymax_){ ymax_ = y;}
                if(x > xmax_){ xmax_ = x;}

                if(c.isDigit()){
                    continue;
                }
                if(c != '-'){
                    rectangle_strings_[layer][x][y] = c;
                    this->onFillCoordinate(c,layer,x,y,data);
                }
            }
            strs.append(str);
        }

        layers_[layer] = strs;
        layerNames_.append(layer);

        this->endFillCoordinate(data);

    }

    void PatternTile::paint(){


        if(horizontalGrid_ != 0){
            this->xspace_ = horizontalGrid_;
        }

        if(verticalGrid_ != 0){
            this->yspace_ = verticalGrid_;
        }

        if( this->minPolyLength_ == 0 ){
            this->minPolyLength_ = this->rules->get("PO","mingatelength");
        }

        //- Load all user-defined patterns
        readPatterns();


        currentHeight_ = yspace_;
        foreach(QString layer, layerNames_){

            QList<QString> strs = layers_[layer];
            for(int y=0;y <= ymax_;y++){
                currentHeight_ = yspace_;
                for(int x=0;x <= xmax_;x++){
                    QString s = strs[strs.length() - y -1];

                    QChar c = s[x];

                    if(c == '-'){continue;}

                    Rect * rect = new Rect();
                    rect->setLayer(layer);

                    Port * p = 0;

                    int xs = translateX(x);
                    int ys = translateY(y);

                    //- Set height
                    if(layer == "PO"){

                        currentHeight_ = this->rules->get(layer,"width");
                        if(currentHeight_ < this->minPolyLength_){
                            currentHeight_ = this->minPolyLength_;
                        }
                    }else if( c== 'x'){
                        currentHeight_ = yspace_;
                    }


                    if(c == 'X' || polyWidthAdjust_ == 0){
                        currentHeight_ = yspace_;
                    }

                    //Make rectangle
                    switch(c.unicode()){
                        //Create ports
                    case 'B':
                    case 'A':
                    case 'P':
                    case 'N':
                    case 'D':
                    case 'S':
                    case 'G':
                        p = this->getPort(QString(c));
                        if(!p){
                            p = new Port(c);
                            this->add(p);
                        }
                    case 'x':
                    case 'X':
                    case 'K':
                    case 'k':
                    case 'C':
                    case 'Q':
                    case 'r':
                    case 'c':
                        rect->setRect(xs,ys,xspace_,currentHeight_);
                        rect->moveCenter(xs + xspace_/2.0, ys + yspace_/2.0);
                        break;
                    case 'V':
                        rect->setRect(xs,ys - yspace_/2.0,xspace_,yspace_*2.0);
                        break;
                    case 'm':
                        rect->setRect(xs,ys,xspace_,this->minPolyLength());
                        rect->moveCenter(xs + xspace_/2.0, ys + yspace_/2.0);
                        break;
                    case 'w':
                        int minw = rules->get(layer,"width");
                        rect->setRect(xs,ys,xspace_,minw);
                        rect->moveCenter(xs + xspace_/2.0, ys + yspace_/2.0);
                        currentHeight_ = minw;
                        break;
                    }


                    
                    
                    if(Pattern.contains(c)){
                        

                        PatternData *p = Pattern[c];
                        if(p){
                            rect->setRect(xs,ys,xspace_,yspace_);                            
                            QList<Rect*> rects = p->getRectangles(rect);
                            foreach(Rect* r,rects){
                                this->add(rects);
                            }
                            rect->setRect(xs,ys,0,0);
                        }
                        
                    }


                    //Adjustment to get poly transistors correct
                    if(c =='G' && layer == "PO"){
                        rect->setRect(xs,ys,xspace_,this->minPolyLength());
                        rect->moveCenter(xs + xspace_/2.0, ys + yspace_/2.0);
                    }

                    if(prev_rect_ && prev_rect_->abutsLeft(rect)){
                        prev_rect_->setRight(rect->x2());
                        delete(rect);
                        rect = prev_rect_;
                    }else if(!rect->empty() && !this->_children.contains(rect)){
                            this->add(rect);
                            rectangles_[layer][y][x] = rect;
                    }
                    
                    prev_rect_ = rect;

                    if(p){
                        p->set(rect);
                    }



                    int cxoffset = 0;

                    //TODO: Implement Q
                    int cw = 0;
                    int ch = 0;
                    int cs = 0;
                    QString lay;
                    Rect *cr;
                    Rect *cr1;
                    //Make cuts
                    switch(c.unicode()){
                    case 'C':
                        if(this->mirrorPatternString()){
                            cxoffset = -xspace_/2.0;
                        }else{
                            cxoffset = xspace_/2.0;
                        }
                    case 'c':
                        lay = this->rules->getNextLayer(layer);
                        cr = new Rect();
                        cr->setLayer(lay);
                        cw = this->rules->get(lay,"width");
                        ch = this->rules->get(lay,"height");
                        cs = this->rules->get(lay,"space");
                        cr->setRect(xs,ys,cw,ch);

                        cr->moveCenter(xs -cxoffset + xspace_/2.0, ys + yspace_/2.0);

                        this->add(cr);

                        break;
                    case 'K':
                    case 'Q':
                    case 'k':
                        lay = this->rules->getNextLayer(layer);
                        cr = new Rect();

                        cr->setLayer(lay);
                        cw = this->rules->get(lay,"width");
                        ch = this->rules->get(lay,"height");
                        cs = this->rules->get(lay,"space");
                        cr->setRect(xs,ys,cw,ch);
                        cxoffset = xspace_/2.0;

                        if(c == 'K'){
                            if(this->mirrorPatternString()){
                                cxoffset = xspace_ ;
                            }else{
                                cxoffset = 0;
                            }
                        }

                        if(c == 'Q'){
                            if (this->mirrorPatternString()) {
                                cxoffset = -xspace_/2.0;
                            } else {
                                cxoffset = xspace_/2 +cs/2 + cr->width()/2;

                            }
                        }


                        cr1 = cr->getCopy();
//                        if(this->mirrorPatternString()){cxoffset -= cs/2 - cw/2;}

                        cr->moveCenter(xs + cxoffset, ys + yspace_/2.0);
                        if(this->mirrorPatternString()){
                            cr1->moveCenter(cr->centerX() + cs + cw,cr->centerY());
                        }else{
                            cr1->moveCenter(cr->centerX() - cs - cw,cr->centerY());
                        }
                        this->add(cr);
                        this->add(cr1);
                        break;
                    }

                    this->paintRect(rect,c,x,y);

                }
            }
        }

        this->updateBoundingRect();
        this->paintEnclosures();
        this->onPaintEnd();

    }

    QList<Rect*> PatternTile::findPatternRects(QString layer){

        ConsoleOutput console;

        if(!rectangles_.contains(layer)){
            console.comment("Error: could not find layer '" + layer + "'");
            return QList<Rect*>();
        }

        QList<Rect*> columnrects;


        QMap<int,QMap<int,Rect*> >  rects = rectangles_[layer];
        for(int y=0;y<=ymax_;y++){

            if(!rects.contains(y)){ continue;}
            QMap<int,Rect *> row = rects[y];
            QList<Rect*> rowrects;

            for(int x=0;x<=xmax_;x++){
                if(row.contains(x)){
                    Rect* r = row[x];
                    bool foundRect = false;
                    foreach(Rect* rx,rowrects){
                        if(rx->x2() == r->x1() && rx->y1() == r->y1() && rx->y2() == r->y2()){
                            rx->setRight(r->x2());
                            foundRect = true;

                        }
                    }
                    if(!foundRect){
                        Rect *  rx = new Rect(r);
                        rowrects.append(rx);
                    }

                }
            }



            foreach(Rect * r, rowrects){
                bool foundRect = false;
                foreach(Rect* ry,columnrects){
                    if(ry->y2() == r->y1() && ry->x1() == r->x1()){
                        ry->setTop(r->y2());
                        foundRect = true;

                    }
                }
                if(!foundRect){
                    Rect *  ry = new Rect(r);
                    columnrects.append(ry);

                }

            }


        }

        return columnrects;
    }



    void PatternTile::addEnclosure(QJsonArray ar){

        Enclosure * e = new Enclosure();
        e->layer = ar[0].toString();
        e->startx  = ar[1].toInt();
        QJsonArray encl = ar[2].toArray();

        foreach(QJsonValue enc, encl){
            e->encloseWithLayers.append(enc.toString()) ;
        }

        enclosures_.append(e);

    }

    void PatternTile::addEnclosuresByRectangle(QJsonArray ar){
        foreach(QJsonValue v, ar){
            this->addEnclosureByRectangle(v.toArray());
        }
    }

    void PatternTile::addEnclosureByRectangle(QJsonArray ar){
        EnclosureRectangle * e= new EnclosureRectangle();
        e->layer = ar[0].toString();
        QJsonArray rect = ar[1].toArray();

        QJsonValue x1 = rect[0];
        if(x1.isString() && x1.toString() == "self"){
            e->x1 = 0;
            e->y1 = 0;
            e->width = this->xmax_ + 1;
            e->height = this->ymax_ + 1;

        }else{
            e->x1 = rect[0].toDouble();
            e->y1 = rect[1].toDouble();

            QJsonValue w = rect[2];
            if(w.isString() && w.toString() == "width"){
                e->width = this->xmax_ + 1;
            }else{
                e->width = w.toDouble();
                if(this->copyColumn_.count() > 0){
                    foreach(CopyColumn c,copyColumn_){
                        if(e->x1 < c.offset && (e->x1 + e->width) > c.offset){
                            e->width += (c.length)*c.count;
                        }
                    }

                }


            }

            QJsonValue h = rect[3];
            if(h.isString() && h.toString() == "height"){
                e->height = this->ymax_ + 1;
            }else{


                e->height = h.toDouble();
            }
        }






        QJsonArray encl = ar[2].toArray();

        foreach(QJsonValue enc, encl){
            e->encloseWithLayers.append(enc.toString()) ;
        }

        enclosures_by_rect_.append(e);

    }

    void PatternTile::paintEnclosures(){


        //- Paint enclosures
        for(int i=0;i<enclosures_.count();i++){
            Enclosure *e = enclosures_[i];
            QList<Rect*> rects = this->findPatternRects(e->layer);
            foreach(QString lay, e->encloseWithLayers){
                if(rects.count() > e->startx){
                    Rect* r = new Rect(rects[e->startx]);

                    int enc = 0;
                    if(this->rules->hasRule(lay,e->layer + "enclosure")){
                        enc = this->rules->get(lay,e->layer + "enclosure");
                    }else{

                        enc = this->rules->get(lay,"enclosure");
                    }

                    r->adjust(enc);
                    Rect * r_enc = new Rect(r);
                    r_enc->setLayer(lay);
                    this->add(r_enc);
                    this->onPaintEnclosure(r_enc);
                }


            }
        }

        //- Paint enclosures by rectangle
        for(int i=0;i<enclosures_by_rect_.count();i++){
            EnclosureRectangle *e = enclosures_by_rect_[i];
            foreach(QString lay, e->encloseWithLayers){


                Rect* r = new Rect(lay,translateX(e->x1),translateY(e->y1),e->width*xspace_,e->height*yspace_);
                this->add(r);

                int enc = 0;
                if(this->rules->hasRule(lay,e->layer + "enclosure")){
                    enc = this->rules->get(lay,e->layer + "enclosure");
                }else if(this->rules->hasRule(lay,"enclosure")){
                    enc = this->rules->get(lay,"enclosure");
                }

                int opposide_enc = enc;
                if(this->rules->hasRule(lay,e->layer + "encOpposite")){
                    opposide_enc = this->rules->get(lay,e->layer + "encOpposite");
                }

                r->adjust(-opposide_enc,-enc, opposide_enc, enc);
                this->onPaintEnclosure(r);
            }
        }


    }

    void PatternTile::onPaintEnclosure(Rect* r)
    {
    }

    void PatternTile::onPaintEnd()
    {

    }




}
