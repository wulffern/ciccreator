//====================================================================
//        Copyright (c) 2015 Carsten Wulff Software, Norway
// ===================================================================
// Created       : wulff at 2015-1-26
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

#ifndef CIC_CORE_CELL_H
#define CIC_CORE_CELL_H

#include <QObject>
#include "rect.h"
#include <QPainterPath>
#include "spice/subckt.h"

namespace cIcCore{

    class Cell: public Rect
    {
        Q_OBJECT
        Q_PROPERTY(QString name READ name WRITE setName)

    public:
        Cell();
        Cell(const Cell&);
        ~Cell();
        Rect * getRect(QString layer);
        void add(Rect* rect);
        void translate(int dx, int dy);
        void mirrorX(int ax);
        void mirrorY(int ay);
        void moveTo(int ax, int ay);
        void moveCenter(int ax, int ay);


        virtual Rect  calcBoundingRect();
        QString toString();
        //static Cell * createInstance();
        QString name(){return _name;}
        QString setName(QString val){ _name = val; return _name;}
        bool isEmpty(){return _is_empty;}


        cIcSpice::Subckt * subckt(){return _subckt;}
        cIcSpice::Subckt * setSubckt(cIcSpice::Subckt * val){ _subckt = val; return _subckt;}

        QList<Rect*> children(){return _children;}

        virtual void place();
        virtual void route();
        virtual void paint();
        virtual void addAllPorts();

        static bool hasCell(QString cell){
            return Cell::_allcells.contains(cell);
        }

        static Cell* getCell(QString cell){
            if(Cell::_allcells.contains(cell)){
                return Cell::_allcells[cell];
            }else{
                Cell * c = new Cell();
                return c;
            }
        }

        static QList<Cell*> getAllCells(){
			QList<Cell*> cells;
			foreach(Cell * cell,_allcells){
				cells.append(cell);
			}
            return cells;
        }

        static Cell* addCell(QString cell,Cell * c){
            Cell::_allcells[cell] = c;
            return c;
        }

        static Cell* addCell(Cell *c){
            Cell::_allcells[c->name()] = c;
            return c;
        }

    protected:
        static QMap<QString,Cell*> _allcells;
        cIcSpice::Subckt * _subckt;

    private:
        QList<Rect*> _children;
        QPainterPath path;
        QString _name;
        bool _has_pr;
        bool _is_empty;

    signals:

    public slots:
        void updateBoundingRect();

    };

}

Q_DECLARE_METATYPE(cIcCore::Cell)

#endif
