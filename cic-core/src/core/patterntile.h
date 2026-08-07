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
#include "spice/subckt.h"

namespace cIcCore{
    using namespace cIcSpice;
    struct CopyColumn{
        int count;
        int offset;
        int length;
        int position;
    };

    struct CopyRow{
        int count;
        int offset;
        int length;
        int position;
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

            //- A sub pattern divides one pattern column into xcount parts,
            //- and the column pitch is not generally divisible by that.
            //- A truncated step then puts every boundary off the
            //- manufacturing grid and accumulates the remainder at the
            //- right edge: 3200 over six parts stepped 533, so a four
            //- part contact came out 2132 wide starting at a coordinate
            //- ending in 33. Computing each boundary from the exact
            //- fraction and snapping it to the grid keeps the sub pattern
            //- filling its column, keeps the parts within half a grid of
            //- equal, and puts every edge where the technology can make
            //- it. Called with grid 0 it behaves exactly as before.
            static int snap(long long v, int grid)
            {
                if(grid <= 0) return (int)v;
                long long q = (v >= 0) ? (v + grid/2) / grid : (v - grid/2) / grid;
                return (int)(q*grid);
            }

            QList<Rect*> getRectangles(Rect* r, int grid = 0)
            {
                QList<Rect*> rects;
                if(!r) return rects;


                int x1 = r->x1();
                int y1 = r->y1();
                int x2 = r->x2();
                int y2 = r->y2();

                QVector<int> xat(xcount+1), yat(ycount+1);
                for(int i=0;i<=xcount;i++)
                    xat[i] = snap(x1 + (long long)(x2-x1)*i/xcount, grid);
                for(int i=0;i<=ycount;i++)
                    yat[i] = snap(y1 + (long long)(y2-y1)*i/ycount, grid);

                Rect* prevrect = NULL;

                for(int y=0;y<ycount;y++){
                    QString s = pattern[y];
                    for(int x = 0;x<xcount;x++){
                        QChar c = s[x];
                        if(c == 'x'){
                            if(prevrect && prevrect->x2() == xat[x]){
                                prevrect->setRight(xat[x+1]);
                            }else{
                                Rect * ra = new Rect(r->layer(),xat[x],yat[y],
                                                     xat[x+1]-xat[x],yat[y+1]-yat[y]);
                                rects.append(ra);
                                prevrect = ra;

                            }
                        }
                    }
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
        /**
         * @brief minimum poly length
         * @accessors minPolyLength(), setMinPolyLength()
         **/
        Q_PROPERTY(qreal minPolyLength READ minPolyLength WRITE setMinPolyLength)

        /**
         * @brief offset to add to the width
         * @accessors widthoffseth(), setWidthOffset()
         **/
        Q_PROPERTY(qreal widthoffset READ widthoffset WRITE setWidthoffset)


        /**
         * @brief offset to add to the width
         * @accessors height(), setHeightOffset()
         **/
        Q_PROPERTY(qreal heightoffset READ heightoffset WRITE setHeightoffset)

        /**
         * @brief Override vertical grid
         * @accessors verticalGrid(), setVerticalGrid()
         **/
        Q_PROPERTY(qreal verticalGrid READ verticalGrid WRITE setVerticalGrid)

        /**
         * @brief Override horizontal grid
         * @accessors horizontalGrid(), setHorizontalGrid()
         **/
        Q_PROPERTY(qreal horizontalGrid READ horizontalGrid WRITE setHorizontalGrid)

        /**
         * @brief multiply vertical grid by an number
         * @accessors verticalGridMultiplier(), setVerticalGridMultiplier()
         **/
        Q_PROPERTY(double verticalGridMultiplier READ verticalGridMultiplier WRITE setVerticalGridMultiplier)

        /**
         * @brief multiply horizontal grid by an number
         * @accessors horizontalGridMultiplier(), setHorizontalGridMultiplier()
         **/
        Q_PROPERTY(double horizontalGridMultiplier READ horizontalGridMultiplier WRITE setHorizontalGridMultiplier)

        /**
         * @brief add offset to Y origin coordinate
         * @accessors yoffset(), setYoffset()
         **/
        Q_PROPERTY(qreal yoffset READ yoffset WRITE setYoffset)

        /**
         * @brief add offset to X origin coordinate
         * @accessors xoffset(), setXoffset()
         **/
        Q_PROPERTY(qreal xoffset READ xoffset WRITE setXoffset)

        /**
         * @brief Mirror the pattern string after creation
         * @accessors mirrorPatternString(), setMirrorPatternString()
         **/
        Q_PROPERTY(int mirrorPatternString READ mirrorPatternString WRITE setMirrorPatternString)

         /**
         * @brief Adjust the poly width
         * @accessors polyWidthAdjust(), setPolyWidthAdjust()
         **/
        Q_PROPERTY(int polyWidthAdjust READ polyWidthAdjust WRITE setPolyWidthAdjust)

         /**
         * @brief Add metal under metal-resistor layer, depends on technology
         * @accessors metalUnderMetalRes(), setMetalUnderMetalRes()
         **/
        Q_PROPERTY(bool metalUnderMetalRes  READ metalUnderMetalRes WRITE setMetalUnderMetalRes)



        public:

            PatternTile();
            PatternTile(const PatternTile&);
            ~PatternTile();

            Q_INVOKABLE
            /**
             * @brief fillCoordinatesFromString
             */
            void fillCoordinatesFromString(QJsonArray ar);

            Q_INVOKABLE
            /**
             * @brief verticalMultiplyVector
             *
             * EXPERIMENTAL!
             * Vector length must be the same length as the number of rows in fillCoordinatesFromString.
             * Multiplies the height of a cell with the number in the vector multiplier
             *
             */
            void verticalMultiplyVector(QJsonArray ar);


            Q_INVOKABLE
            void getRuleForHorizontalGrid(QJsonArray ar);

            Q_INVOKABLE
            void getRuleForVerticalGrid(QJsonArray ar);


            Q_INVOKABLE
            /**
             *
             * Copy a column set of a fillCoordinateFromStrings
             *
             * @param QJsonObject Array of CopyColumn objects
             *
             * A CopyColumn object consists of the following
             * {
             *  "count" : int,
             *  "offset" : int,
             *  "length" : int,
             *  ["position" : int]
             * }
             *
             * - count is the number of times to copy the column set
             * - offset is the column set index from left edge of string
             * - length is the length of the column set
             * - position is optional, and if not set will be equal to offset.
             * If position is given, then the copies of a column is inserted at that point
             *
             *
             **/
            void copyColumn(QJsonObject obj);

            Q_INVOKABLE
            /**
             *
             * Copy a row set of a fillCoordinateFromStrings
             *
             * @param QJsonObject Array of CopyRows objects
             *
             * A CopyRows object consists of the following
             * {
             *  "count" : int,
             *  "offset" : int,
             *  "length" : int,
             *  ["position" : int]
             * }
             *
             * - count is the number of times to copy the row set
             * - offset is the row set index from top edge of string
             * - length is the length of the row set
             * - position is optional, and if not set will be equal to offset.
             * If position is given, then the copies of a row set is inserted at that point
             *
             *
             **/
            void copyRow(QJsonObject obj);


            Q_INVOKABLE
            void copyLayer(QJsonArray ar);

            Q_INVOKABLE
            void addEnclosure(QJsonArray ar);

            Q_INVOKABLE
            void addEnclosureByRectangle(QJsonArray ar);

            Q_INVOKABLE
            void addEnclosuresByRectangle(QJsonArray ar);

            virtual QMap<QString,QVariant> initFillCoordinates();
            virtual void onFillCoordinate(QChar , QString , int , int , QMap<QString,QVariant> &){};
            virtual void onPaintEnclosure(Rect*);

            virtual void endFillCoordinate(QMap<QString,QVariant> &){};
            virtual void paintRect(Rect*, QChar ,int , int ){};

            void paint() override;

            virtual SimpleRect calcBoundingRect() override;

            qreal minPolyLength(){return minPolyLength_;}
            qreal setMinPolyLength(qreal val){ minPolyLength_ = val*this->rules->gamma(); return minPolyLength_;  }

            bool metalUnderMetalRes(){return metalUnderMetalRes_;}
            void setMetalUnderMetalRes(bool val){ metalUnderMetalRes_ = val;  }

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
            int currentHeightDelta_;
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
            QList<CopyRow> copyRow_;
            QList<CopyLayer> copyLayer_;
            QMap<QString,QMap<int,QMap<int,Rect*> > > rectangles_;
            Rect * prev_rect_;

            int arraylength;

            QList<double> verticalMultiplyVector_;

            double verticalMultiplyVectorSum(int y);

            QList<Rect*> findPatternRects(QString layer);
            QList<Rect*> findPatternStrings(QString layer);
            Rect * makeRect(QString layer,QChar c,int x, int y);
            void paintEnclosures();
            virtual void onPaintEnd();
            virtual void readPatterns();
            virtual QJsonObject toJson() override;
            virtual void fromJson(QJsonObject o) override;



            inline int translateX(int x){  return (x + xoffset_)*xspace_;}
            int translateY(int y);
    };

}

Q_DECLARE_METATYPE(cIcCore::PatternTile)

#endif // PATTERNTILE_H
