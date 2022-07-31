//====================================================================
//        Copyright (c) 2015 Carsten Wulff Software, Norway
// ===================================================================
// Created       : wulff at 2015-04-03
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

#ifndef CIC_CORE_PATTERNTILE_H
#define CIC_CORE_PATTERNTILE_H

#include <QJsonArray>
#include "cell.h"
#include "consoleoutput.h"
#include "core/port.h"

namespace cIcCore{

    struct CopyColumn{
        int count;
        int offset;
        int length;
    };

    struct CopyLayer{
        QString from;
        QString to;
    };

    struct Enclosure{
        QString layer;
        int startx;
        QList<QString> encloseWithLayers;

    };

    class PatternData
    {
        public:
            int xcount;
            int ycount;
            QChar name;
            QStringList pattern;

            QList<Rect*> getRectangles(Rect* r)
            {
                QList<Rect*> rects;
                if(!r) return rects;


                int x1 = r->x1();
                int y1 = r->y1();
                int x2 = r->x2();
                int y2 = r->y2();
                int xstep = (x2 - x1)/xcount;
                int ystep = (y2 - y1)/ycount;
                int xa = x1;
                int ya = y1;

                Rect* prevrect;

                for(int y=0;y<ycount;y++){
                    QString s = pattern[y];
                    xa = x1;
                    for(int x = 0;x<xcount;x++){
                        QChar c = s[x];
                        if(c == 'x'){
                            if(prevrect && prevrect->x2() == xa){
                                prevrect->setRight(xa+xstep);
                            }else{
                                Rect * ra = new Rect(r->layer(),xa,ya,xstep,ystep);
                                rects.append(ra);
                                prevrect = ra;

                            }
                        }
                        xa += xstep;
                    }
                    ya += ystep;
                }
                return rects;

            }


    };



    struct EnclosureRectangle{
        QString layer;
        double x1;
        double y1;
        double width;
        double height;
        QList<QString> encloseWithLayers;
    };

    class PatternTile: public Cell
    {
        Q_OBJECT
        Q_PROPERTY(qreal minPolyLength READ minPolyLength WRITE setMinPolyLength)
        Q_PROPERTY(qreal widthoffset READ widthoffset WRITE setWidthoffset)
        Q_PROPERTY(qreal heightoffset READ heightoffset WRITE setHeightoffset)
        Q_PROPERTY(qreal verticalGrid READ verticalGrid WRITE setVerticalGrid)
        Q_PROPERTY(qreal horizontalGrid READ horizontalGrid WRITE setHorizontalGrid)
        Q_PROPERTY(double verticalGridMultiplier READ verticalGridMultiplier WRITE setVerticalGridMultiplier)
        Q_PROPERTY(double horizontalGridMultiplier READ horizontalGridMultiplier WRITE setHorizontalGridMultiplier)
        Q_PROPERTY(qreal yoffset READ yoffset WRITE setYoffset)
        Q_PROPERTY(qreal xoffset READ xoffset WRITE setXoffset)
        Q_PROPERTY(int mirrorPatternString READ mirrorPatternString WRITE setMirrorPatternString)
        Q_PROPERTY(int polyWidthAdjust READ polyWidthAdjust WRITE setPolyWidthAdjust)
        Q_PROPERTY(bool metalUnderMetalRes  READ metalUnderMetalRes WRITE setMetalUnderMetalRes)



        public:

            PatternTile();
            PatternTile(const PatternTile&);
            ~PatternTile();

            Q_INVOKABLE void fillCoordinatesFromString(QJsonArray ar);
            Q_INVOKABLE void getRuleForHorizontalGrid(QJsonArray ar);
            Q_INVOKABLE void getRuleForVerticalGrid(QJsonArray ar);
            Q_INVOKABLE void copyColumn(QJsonObject obj);
            Q_INVOKABLE void copyLayer(QJsonArray ar);
            Q_INVOKABLE void addEnclosure(QJsonArray ar);
            Q_INVOKABLE void addEnclosureByRectangle(QJsonArray ar);
            Q_INVOKABLE void addEnclosuresByRectangle(QJsonArray ar);

            virtual QMap<QString,QVariant> initFillCoordinates();
            virtual void onFillCoordinate(QChar , QString , int , int , QMap<QString,QVariant> &){};
            virtual void onPaintEnclosure(Rect*);

            virtual void endFillCoordinate(QMap<QString,QVariant> &){};
            virtual void paintRect(Rect*, QChar ,int , int ){};

            void paint();

            virtual Rect calcBoundingRect();

            qreal minPolyLength(){return minPolyLength_;}
            qreal setMinPolyLength(qreal val){ minPolyLength_ = val*this->rules->gamma(); return minPolyLength_;  }

            bool metalUnderMetalRes(){return metalUnderMetalRes_;}
            bool setMetalUnderMetalRes(bool val){ metalUnderMetalRes_ = val;  }

            qreal verticalGrid(){return verticalGrid_;}
            qreal setVerticalGrid(qreal val){ verticalGrid_ = val; return verticalGrid_;  }

            double verticalGridMultiplier(){return verticalGridMultiplier_;}
            double setVerticalGridMultiplier(double val){ verticalGridMultiplier_ = val; return verticalGridMultiplier_;  }

            int horizontalGrid(){return horizontalGrid_;}
            int setHorizontalGrid(int val){ horizontalGrid_ = val; return horizontalGrid_;  }

            double horizontalGridMultiplier(){return horizontalGridMultiplier_;}
            double setHorizontalGridMultiplier(double val){ horizontalGridMultiplier_ = val; return horizontalGridMultiplier_;  }

            qreal widthoffset(){return widthoffset_;}
            qreal setWidthoffset(qreal widthoffset){widthoffset_ = widthoffset; return widthoffset_;}

            qreal heightoffset(){return heightoffset_;}
            qreal setHeightoffset(qreal heightoffset){heightoffset_ = heightoffset; return heightoffset_;}

            qreal xoffset(){return xoffset_;}
            qreal setXoffset(qreal xoffset){xoffset_ = xoffset; return xoffset_;}


            int polyWidthAdjust(){return polyWidthAdjust_;}
            int setPolyWidthAdjust(int val){polyWidthAdjust_ = val; return val;}


            qreal yoffset(){return yoffset_;}
            qreal setYoffset(qreal yoffset){yoffset_ = yoffset; return yoffset_;}

            int mirrorPatternString(){return mirrorPatternString_;}
            int setMirrorPatternString(int mirrorPatternString){mirrorPatternString_ = mirrorPatternString; return mirrorPatternString_;}


            static QMap<QString,QStringList> Patterns;
            QMap<QChar,PatternData*> Pattern;


        protected:
            QMap<QString,QMap<int,QMap<int,QChar> > > rectangle_strings_;
            int mirrorPatternString_;
            qreal minPolyLength_;
            int currentHeight_;
            qreal horizontalGrid_;
            qreal verticalGrid_;
            double horizontalGridMultiplier_;
            double verticalGridMultiplier_;
            bool metalUnderMetalRes_;
            int polyWidthAdjust_;
            int xmax_;
            int ymax_;
            qreal yoffset_;
            qreal xoffset_;
            int xspace_;
            int yspace_;
            QList<Enclosure*> enclosures_;
            QList<EnclosureRectangle*> enclosures_by_rect_;
            QMap<QString,QList<QString> > layers_;
            QList<QString> layerNames_;
            qreal widthoffset_;
            qreal heightoffset_;
            QList<CopyColumn> copyColumn_;
            QList<CopyLayer> copyLayer_;
            QMap<QString,QMap<int,QMap<int,Rect*> > > rectangles_;
            Rect * prev_rect_;

            int arraylength;



            QList<Rect*> findPatternRects(QString layer);
            QList<Rect*> findPatternStrings(QString layer);
            Rect * makeRect(QString layer,QChar c,int x, int y);
            void paintEnclosures();
            virtual void onPaintEnd();
            virtual void readPatterns();
            virtual QJsonObject toJson();
            virtual void fromJson(QJsonObject o);



            inline int translateX(int x){  return (x + xoffset_)*xspace_;}
            inline int translateY(int y){ return (y + yoffset_)*yspace_;}
    };

}

Q_DECLARE_METATYPE(cIcCore::PatternTile)

#endif // PATTERNTILE_H
