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
#include "port.h"
#include "text.h"
#include "instanceport.h"
#include <QPainterPath>
#include "spice/subckt.h"

namespace cIcCore{

    /*!
      Base class for all cells, usually inherited to provide specialization.
      The methods of this class is called in the following order when a cell is created:
      place();
      route();
      paint();

    */
    class Cell: public Rect
    {
        Q_OBJECT
        //Q_PROPERTY(QString name READ name WRITE setName)
        //
         /**
         * @brief Mark a cell as physical only
         * @accessors isPhysicalOnly(), setPhysicalOnly()
         **/
        Q_PROPERTY(bool physicalOnly READ isPhysicalOnly WRITE setPhysicalOnly)
        Q_INVOKABLE
        void moveTo(QJsonArray obj);
        Q_INVOKABLE
        void abstract(QJsonValue obj);



        public:
            Cell();
            Cell(const Cell&);
            ~Cell();

            //! Find the first rectangle in this cell that uses layer
            Rect * getRect(QString layer);

            //! Add a rectangle to the cell, hooks updated() of the child to updateBoundingRect
            virtual void add(Rect* rect);
            virtual void add(QList<Rect*> rects);

            //! Move this cell, and all children by dx and dy
            void translate(int dx, int dy) override;

            //! Mirror this cell, and all children around ax
            void mirrorX(int ax) override;

            //! Mirror this cell, and all children around ay
            void mirrorY(int ay) override;

            //! Move this cell, and all children to ax and ay
            void moveTo(int ax, int ay) override;

            //! Center this cell, and all children on ax and ay
            Q_INVOKABLE
            void moveCenter(int ax, int ay) override;

            Q_INVOKABLE
            void meta(QJsonObject obj);

            //! Center this cell, and all children on ax and ay
            Q_INVOKABLE
            void boundaryIgnoreRouting(QJsonValue obj);
            void setBoundaryIgnoreRouting(bool bir);
            bool boundaryIgnoreRouting();

            //! Shortcut for adding ports
            void addPort(QString name, Rect* r);


            //! Mirror this cell, and all children around horizontal center point (basically flip horizontal)
            Q_INVOKABLE
            void mirrorCenterX();
            Q_INVOKABLE
            void mirrorCenterY();

            //! Calculate the extent of this cell. Should be overriden by children
            virtual Rect calcBoundingRect();
            static Rect calcBoundingRect(QList<Rect*> children);
            static Rect calcBoundingRect(QList<Rect*> children,bool ignoreBoundaryRouting);

            static bool isEmpty(Cell *c);


            //! Convert cell to a human readable format, useful for debug
            QString toString() override;

            //! Mark as a physcial only cell
            bool isPhysicalOnly(){return _physicalOnly;}
            bool setPhysicalOnly(bool val){ _physicalOnly = val;
                return _physicalOnly;
            }


            //! Name of this cell
            QString name(){return _name;}
            QString setName(QString val){ _name = val;
                if(_subckt){
                    _subckt->setName(val);
                }
                return _name;}

            void setLibCell(bool isLibCell){lib_cell_ = isLibCell; }
            void setLibPath(QString path){lib_path_ = path; }
            QString libPath(){return lib_path_;}
            void setUsed(bool isUsed){cell_used_ = isUsed; }
            bool isUsed(){return cell_used_;}

            //! Get the port linked to net name (name)
            Port * getPort(QString name);
            Port * getCellPort(QString name);

            //! Get all ports on this cell
            QList<Port *>  ports();
            QMap<QString,QList<Port*>> allports();
            QList<QString> allPortNames();

            //! Update rectangle of port, if port does not exist a new one
            //! is created
            Port * updatePort(QString name,Rect* r);

            //! Spice subcircuit object
            cIcSpice::Subckt * subckt(){return _subckt;}
            cIcSpice::Subckt * setSubckt(cIcSpice::Subckt * val){ _subckt = val; return _subckt;}


            //! Get list of all children
            QList<Rect*> children(){return _children;}

            bool isASpicePort(QString name);


            //! Place children
            virtual void place();

            //! Route children
            virtual void route();

            //! Paint children, useful with a method after route
            virtual void paint();

            //! Automatically add remaing ports
            virtual void addAllPorts();

            //! Check if this cell contains a cell with name cell
            static bool hasCell(QString cell){
                return Cell::_allcells.contains(cell);
            }

            //! Get a named cell, returns empty cell if it does not exist, so you should check
            //! that the cell exists in this cell first
            static Cell* getCell(QString cell){
                if(Cell::_allcells.contains(cell)){
                    auto c = Cell::_allcells[cell];
                    return c;
                }else{
                    Cell * c = new Cell();
                    return c;
                }
            }

            //! Get a list of all cells in this design
            static QList<Cell*> getAllCells(){
                QList<Cell*> cells;
                foreach(Cell * cell,_allcells){
                    cells.append(cell);
                }
                return cells;
            }

            //! Add a cell to the list of all cells
            static Cell* addCell(QString cell,Cell * c){
                Cell::_allcells[cell] = c;
                return c;
            }

            //! Add a cell, and use the cell->name() as key
            static Cell* addCell(Cell *c){
                Cell::_allcells[c->name()] = c;
                return c;
            }



            //! Find all rectangles by regular expression
            virtual QList<Rect *> findRectanglesByRegex(QString regex,QString layer);
            virtual void findRectangles(QList<Rect*> &rects,QString name,QString layer);
            virtual QList<Rect *> findAllRectangles(QString regex, QString layer);

            QJsonObject toJson() override;
            void fromJson(QJsonObject o) override;
            virtual Rect * cellFromJson(QJsonObject co);
            QList<Rect*> getChildren(QString type);

            void addEnclosingLayers(QList<QString> layers);

            virtual void updateUsedChildren();



        protected:
            QList<Rect*> routes_;
            //! List of all cells
            static QMap<QString,Cell*> _allcells;

            //! Ports in this cell
            QMap<QString,Port*> ports_;

            QList<QString> allPortNames_;

            QMap<QString,QList<Port*>> allports_;

            //! Named Rects in this cell
            QMap<QString,Rect*> named_rects_;

            //! SPICE subcircuit related to this cell
            cIcSpice::Subckt * _subckt;

            //! Find bottom left rectangle in the cell
            Rect* getBottomLeftRect();
            //! Find top left rectangle in the cell
            Rect* getTopLeftRect();

            //! Children of this cell
            QList<Rect*> _children;
            QMap<QString,QList<Rect*>> children_by_type;

            QJsonObject meta_;




        protected:
            QString instanceName_;
            bool boundaryIgnoreRouting_;
            bool _has_pr;
            bool abstract_;
            bool lib_cell_;
            bool cell_used_;
            QString lib_path_;


        private:
            //! Cell name
            QString _name;
            bool _physicalOnly;




        signals:

        public slots:
            void updateBoundingRect();

    };

}

Q_DECLARE_METATYPE(cIcCore::Cell)

#endif
