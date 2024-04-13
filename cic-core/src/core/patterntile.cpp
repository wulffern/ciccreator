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
        arraylength = 0;

        verticalGrid_ = 0;
        horizontalGrid_ = 0;
        verticalGridMultiplier_ = 1;
        horizontalGridMultiplier_ = 1;
        metalUnderMetalRes_ = true;

        prev_rect_  = 0;
        _has_pr = true;

    }

    Rect PatternTile::calcBoundingRect(){

        int x1  = 0;
        int y1  = 0;
        int x2  = ((float)xmax_ + (float)widthoffset_)*(float)xspace_;

        //Find the sum of all heights
        int y2 = (double)heightoffset_*(double)yspace_;

        y2 += verticalMultiplyVectorSum(ymax_)*(double)yspace_;

        //There is an error when ymax_ == 0
        //TODO: Look at the boundary, I think there is a think error on ymax
        //The below is a bandaid
        if(ymax_ == 0){
            y2 = (double)yspace_;
        }

        //qDebug() << "Bounding pattern" << x1 << y1 << x2 << y2 << ymax_ << verticalMultiplyVector_.count() << widthoffset_ << heightoffset_;

        Rect r;
        r.setPoint1(x1,y1);
        r.setPoint2(x2,y2);
        return r;
    }


    PatternTile::~PatternTile()
    {
    }

    void PatternTile::verticalMultiplyVector(QJsonArray ar){
        foreach(QJsonValue v, ar){
            verticalMultiplyVector_.append(v.toDouble());
        }
    }

    double PatternTile::verticalMultiplyVectorSum(int y){
        double sum = 0;
        if(y >= 0 && verticalMultiplyVector_.count() >= y){
            for(int i=0;i<y;i++){
                sum += verticalMultiplyVector_[i];
            }
        }else{
            sum = y;
        }
        return sum;
    }

    int PatternTile::translateY(int y){
        double yt = yoffset_*yspace_;
        yt += verticalMultiplyVectorSum(y)*(double)yspace_;
        return (int)yt;
    }

    void PatternTile::copyColumn(QJsonObject obj){
        CopyColumn c;
        c.count =obj["count"].toInt();
        c.length = obj["length"].toInt();
        c.offset= obj["offset"].toInt();
        if(obj.contains("position")){
            c.position= obj["position"].toInt();
        }else{
            c.position= obj["offset"].toInt();
        }
        copyColumn_.append(c);
    }

     void PatternTile::copyRow(QJsonObject obj){
        CopyRow c;
        c.count =obj["count"].toInt();
        c.length = obj["length"].toInt();
        c.offset= obj["offset"].toInt();
        if(obj.contains("position")){
            c.position= obj["position"].toInt();
        }else{
            c.position= obj["offset"].toInt();
        }
        copyRow_.append(c);
    }

    void PatternTile::copyLayer(QJsonArray ar){
        CopyLayer l;
        l.from = ar[0].toString();
        l.to= ar[1].toString();
        copyLayer_.append(l);
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


        QString layer = ar[0].toString();
        ar.pop_front();

        if(arraylength == 0) arraylength = ar.count();

       if(arraylength != ar.count()){
           qDebug() << " Error: " << " layer " << layer << " does not have " << arraylength << " lines \n";

       }

       //Check if the current layer is supposed to be copied. If so, then run fillCoordinates
       foreach(CopyLayer cl, copyLayer_){
            if(layer == cl.from){
                QJsonArray ar2 = ar;
                ar2.push_front(cl.to);
                fillCoordinatesFromString(ar2);
            }

       }

       //- Make a string list with the pattern
       QList<QString> rstrs;
       for(int i=0;i<ar.count();i++){
            rstrs.append(ar[i].toString());
       }

       //Copy Rows
       if(copyRow_.length() > 0){

           for(int z=0;z<copyRow_.length();z++){
               CopyRow c = copyRow_[z];


               if(rstrs.count() < c.offset){
                   qDebug() << " Warning: copyRow offset to large for " << rstrs.count();
                   continue;
               }
               if(rstrs.count() < c.offset + c.length){
                   qDebug() << " Warning: copyRow offset+length to large for " << rstrs.count();
                   continue;
               }


               QList<QString> s;
               for(int k=c.offset;k < c.offset + c.length;k++){
                       s.append(rstrs[k]);

                   }

               for(int k=0;k < c.count;k++){
                       for(int l=0;l < s.count();l++){
                           rstrs.insert(c.position,s[l]);
                       }
                   }

           }
       }
        
        QList<QString> strs;
        for(int i=0;i<rstrs.count();i++){
            QString str = rstrs[i];

            //Copy columns
            if(copyColumn_.length() > 0){
                for(int z=0;z<copyColumn_.length();z++){
                    CopyColumn c = copyColumn_[z];
                    if(str.length() < c.offset){
                        qDebug() << " Warning: copyColumn offset to large for " << rstrs.count();
                        continue;
                    }


                    QString sorg = str.mid(c.offset,c.length);
                    for(int x=0;x<c.count;x++){
                        str.insert(c.position,sorg);
                    }
                }
            }

            if(this->mirrorPatternString()){
                QString tmp;
                for(int k=str.length()-1;k >= 0; k--){
                    tmp.append(str[k]);
                }
                str = tmp;
            }

            for(int x=0;x < str.length();x++){
                QChar c = str[x];
                int y = rstrs.count() - i -1 ;

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

        foreach(QString layer, layerNames_){

            QList<QString> strs = layers_[layer];

            //Set the start Y coordinate
            for(int y=0;y <= ymax_;y++){

                //Get the vertical Multiplier, default no multiplier
                double vmv = 1;
                if(verticalMultiplyVector_.count() > y){
                    vmv = verticalMultiplyVector_[y];
                }else{
                    if(verticalMultiplyVector_.count() > 0){
                        qDebug() << "Error: verticalMultiplyVector does not contain index " << y;
                    }
                }

                currentHeight_ = yspace_*vmv;
                currentHeightDelta_ = currentHeight_ - yspace_;
                int ys = translateY(y);

                for(int x=0;x <= xmax_;x++){


                    QString s = strs[strs.length() - y -1];

                    if((s.length()-1) < x){
                        qDebug() << "Error: To few columns in "  << s;
                    }

                    QChar c = s[x];


                    if(c == '-'){continue;}

                    Rect * rect = new Rect();
                    rect->setLayer(layer);

                    Port * p = 0;

                    //Get the coordinate for the current x
                    int xs = translateX(x);

                    //- Set height, including the vertical Multiplier Vector
                    if(layer == "PO"){
                        currentHeight_ = this->rules->get(layer,"width") + currentHeightDelta_;
                        if(currentHeight_ < this->minPolyLength_){
                            currentHeight_ = this->minPolyLength_+ currentHeightDelta_;
                        }
                    }else if( c== 'x'){
                        currentHeight_ = yspace_*vmv;
                    }

                    if(c == 'X' || polyWidthAdjust_ == 0){
                        currentHeight_ = yspace_*vmv;
                    }

                    int ch = 0;

                    // Local yspace including multiplier
                    int lyspace_ = yspace_*vmv;

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
                        rect->moveCenter(xs + xspace_/2.0, ys + lyspace_/2.0);
                        break;
                    case '3':
                        ch = this->rules->get(layer,"height");
                        rect->setRect(xs,ys,xspace_*3,ch);
                        rect->moveCenter(xs + xspace_/2.0, ys + lyspace_/2.0);
                        break;
                    case 'V':
                        rect->setRect(xs,ys - lyspace_/2.0,xspace_,lyspace_*2.0);
                        break;
                    case 'm':
                        rect->setRect(xs,ys,xspace_,this->minPolyLength()+ currentHeightDelta_);
                        rect->moveCenter(xs + xspace_/2.0, ys + lyspace_/2.0);
                        break;
                    case 'w':
                        int minw = rules->get(layer,"width")+ currentHeightDelta_;
                        rect->setRect(xs,ys,xspace_,minw);
                        rect->moveCenter(xs + xspace_/2.0, ys + lyspace_/2.0);
                        currentHeight_ = minw;
                        break;
                    }


                    
                    
                    if(rect && Pattern.contains(c)){

                        PatternData *pl = Pattern[c];
                        if(pl){
                            rect->setRect(xs,ys,xspace_,lyspace_);                            
                            QList<Rect*> rects = pl->getRectangles(rect);
                            foreach(Rect* r,rects){
                                this->add(rects);
                            }
                            rect->setRect(xs,ys,0,0);
                        }
                        
                    }


                    //Adjustment to get poly transistors correct
                    if(c =='G' && layer == "PO"){
                        rect->setRect(xs,ys,xspace_,this->minPolyLength()+ currentHeightDelta_);
                        rect->moveCenter(xs + xspace_/2.0, ys + lyspace_/2.0);
                    }





                    //Don't combine rectangles if it's a metal resistor
                    if(c != 'r' &&prev_rect_ && prev_rect_->abutsLeft(rect)){
                        prev_rect_->setRight(rect->x2());
                        delete(rect);
                        rect = prev_rect_;
                    }else if(!rect->empty() && !this->_children.contains(rect)){

                        if(c == 'r'){
                            //Some technologies don't want metal under metal resistor
                            if(this->metalUnderMetalRes()){
                                this->add(rect);
                                rectangles_[layer][y][x] = rect;
                                prev_rect_ = rect;
                            }else{
                            }
                        }else{
                            this->add(rect);
                            rectangles_[layer][y][x] = rect;
                            prev_rect_ = rect;
                        }

                    }

                    //Set port rectangle if we're on a port
                    if(p){
                        p->set(rect);
                    }

                    int cxoffset = 0;

                  
                    //- Handle contacts with care
                    int cw = 0;
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

                        cr->moveCenter(xs -cxoffset + xspace_/2.0, ys + lyspace_/2.0);

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

                        cr->moveCenter(xs + cxoffset, ys + lyspace_/2.0);
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
            e->height = verticalMultiplyVectorSum(ymax_+1);

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
                        }else if(mirrorPatternString_) {

                            //cout << "\n" << e->x1 << "," << e->y1 << "," << e->width << "," <<e->height << "\n";
                            int xmax = this->xmax_ + 1 - (c.length)*c.count;
                            int x2mir = xmax - e->x1;
                            int x1mir = xmax - e->x1 - e->width;

                            //cout << "x1mir=" << x1mir << " x2mir=" << x2mir << "\n";
                            //cout << c.offset<< "\n";

                            if(x1mir < c.offset && x2mir > c.offset){
                                e->width += (c.length)*c.count;
                            }

                        }
                    }

                }
            }
            
            QJsonValue h = rect[3];
            if(h.isString() && h.toString() == "height"){
                e->height = verticalMultiplyVectorSum(ymax_+1);
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

    QJsonObject PatternTile::toJson(){
        QJsonObject o = Cell::toJson();


        o["xmax"]  = xmax_;
        o["ymax"] = ymax_;
        o["yoffset"] = yoffset_;
        o["xspace"] = xspace_;
        o["yspace"] = yspace_;
        //o["minPolyLength"] = minPolyLength_;
        o["widthoffset"] = widthoffset_;
        o["heightoffset"] = heightoffset_;

        QJsonArray ar;
        foreach(double d, verticalMultiplyVector_){
            ar.append(d);
        }

        o["verticalMultiplyVector"] = ar;
        //o["mirrorPatternString"] = mirrorPatternString_;
        //o["polyWidthAdjust"] = polyWidthAdjust_;
        //o["verticalGrid"] = verticalGrid_;
        //o["verticalGridMultiplier"] =

        return o;
    }

    void PatternTile::fromJson(QJsonObject o){
        Cell::fromJson(o);

        xmax_ = o["xmax"].toInt();
        ymax_ = o["ymax"].toInt();
        yoffset_ = o["yoffset"].toDouble();
        xoffset_ = o["xoffset"].toDouble();
        widthoffset_ = o["widthoffset"].toDouble();
        heightoffset_ = o["heightoffset"].toDouble();
        xspace_ = o["xspace"].toInt();
        yspace_ = o["yspace"].toInt();

        if(o.contains("verticalMultiplyVector")){
            QJsonArray car = o["verticalMultiplyVector"].toArray();
            foreach(QJsonValue c, car){
                verticalMultiplyVector_.append(c.toDouble());
            }
        }

    }




}
