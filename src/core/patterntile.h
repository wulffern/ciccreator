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

  struct Enclosure{
    QString layer;
    int startx;
    QList<QString> encloseWithLayers;

  };

  struct EnclosureRectangle{
    QString layer;
    int x1;
    int y1;
    int width;
    int height;
    QList<QString> encloseWithLayers;
  };

  class PatternTile: public Cell
  {
    Q_OBJECT
    Q_PROPERTY(int minPolyLength READ minPolyLength WRITE setMinPolyLength)
    Q_PROPERTY(int widthoffset READ widthoffset WRITE setWidthoffset)
    Q_PROPERTY(int heightoffset READ heightoffset WRITE setHeightoffset)
    Q_PROPERTY(qreal yoffset READ yoffset WRITE setYoffset)
    Q_PROPERTY(qreal xoffset READ xoffset WRITE setXoffset)
    Q_PROPERTY(int mirrorPatternString READ mirrorPatternString WRITE setMirrorPatternString)

  public:

    PatternTile();
    PatternTile(const PatternTile&);
    ~PatternTile();

    Q_INVOKABLE void fillCoordinatesFromString(QJsonArray ar);
    Q_INVOKABLE void copyColumn(QJsonObject obj);
    Q_INVOKABLE void addEnclosure(QJsonArray ar);
    Q_INVOKABLE void addEnclosureByRectangle(QJsonArray ar);
    Q_INVOKABLE void addEnclosuresByRectangle(QJsonArray ar);

    virtual QMap<QString,QVariant> initFillCoordinates();
    virtual void onFillCoordinate(QChar c, QString layer, int x, int y, QMap<QString,QVariant> &data){
    }

    virtual void endFillCoordinate(QMap<QString,QVariant> &data){}
	  virtual void paintRect(Rect *r, QChar c,int x, int y){}

    void paint();

    virtual Rect calcBoundingRect();

    int minPolyLength(){return minPolyLength_;}
    int setMinPolyLength(int val){ minPolyLength_ = val; return minPolyLength_;  }

    qreal widthoffset(){return widthoffset_;}
    qreal setWidthoffset(qreal widthoffset){widthoffset_ = widthoffset; return widthoffset_;}

    qreal heightoffset(){return heightoffset_;}
    qreal setHeightoffset(qreal heightoffset){heightoffset_ = heightoffset; return heightoffset_;}

    qreal xoffset(){return xoffset_;}
    qreal setXoffset(qreal xoffset){xoffset_ = xoffset; return xoffset_;}

    qreal yoffset(){return yoffset_;}
    qreal setYoffset(qreal yoffset){yoffset_ = yoffset; return yoffset_;}

    int mirrorPatternString(){return mirrorPatternString_;}
    int setMirrorPatternString(int mirrorPatternString){mirrorPatternString_ = mirrorPatternString; return mirrorPatternString_;}




  protected:
     QMap<QString,QMap<int,QMap<int,QChar> > > rectangle_strings_;
    int mirrorPatternString_;
    int minPolyLength_;
    int currentHeight_;
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
    QMap<QString,QMap<int,QMap<int,Rect*> > > rectangles_;
    Rect * prev_rect_;

    QList<Rect*> findPatternRects(QString layer);
     QList<Rect*> findPatternStrings(QString layer);
      Rect * makeRect(QString layer,QChar c,int x, int y);
      void paintEnclosures();

    inline int translateX(int x){  return (x + xoffset_)*xspace_;}
    inline int translateY(int y){ return (y + yoffset_)*yspace_;}
  };

}

Q_DECLARE_METATYPE(cIcCore::PatternTile)

#endif // PATTERNTILE_H
