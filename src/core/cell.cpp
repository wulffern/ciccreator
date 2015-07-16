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

#include "core/cell.h"

//  sub children{ return (shift)->property("children",(shift))};
//  sub name{ return (shift)->property("name",(shift));}
//  sub parent{ return (shift)->property("parent",(shift));}
//  sub hasPR{ return (shift)->property("hasPR",(shift));}
//  sub mirror{ return (shift)->property("mirror",(shift));}
//  sub mirror_line{ return (shift)->property("mirror_line",(shift));}
//  sub schObject{ return (shift)->property("schObject",(shift));}
//#  sub isLibraryCell{ return (shift)->property("isLibraryCell",(shift));}
namespace cIcCore{

    QMap<QString,Cell*> Cell::_allcells;

    Cell::Cell(): Rect(){

    }

    Cell::Cell(const Cell&){


    }

    Cell::~Cell() {

    }



    void Cell::paint(){
//    qDebug() << "Painting Cell";
    }
    void Cell::route(){}
    void Cell::place(){}
    void Cell::addAllPorts(){}

    Rect* Cell::getRect(QString layer){
        foreach (Rect* child, _children){
            QString lay = child->layer();
            if (lay.compare(layer) == 0) {
                return child;
            }
        }
        return 0    ;
    }



    void Cell::add(Rect* child){

        if ( child == 0){
            qDebug() << "Tried to add null-pointer to " << this->name();
            return;
        }

        if (child && !_children.contains(child)) {
            child->parent(this);
            this->_children.append(child);
            connect(child,SIGNAL(updated()),this, SLOT(updateBoundingRect()));
        }

        this->updateBoundingRect();
    }



    void Cell::translate(int dx, int dy) {

        Rect::translate(dx,dy);

        foreach(Rect* child, _children) {
            //if( strcmp(child->metaObject()->className(),"IcEnclosingRect") == 0){
            //    continue;
            //  }
            child->translate(dx,dy);
        }
        emit updated();

        this->updateBoundingRect();
    }

    void Cell::mirrorX(int ax) {
        this->setLeft(2*ax - this->left());
        this->setRight(2*ax - this->right());

        if (this->right() < this->left()) {
            int tmp = this->left();
            this->setLeft(this->right());
            this->setRight(tmp);
        }

        foreach(Rect * child, _children) {
            //  if( strcmp(child->metaObject()->className(),"IcEnclosingRect") == 0){
            //      continue;
            //    }
            child->mirrorX(ax);
        }
        this->updateBoundingRect();
        emit updated();
    }

    void Cell::mirrorY(int ay) {

        this->setTop(2 *  ay - this->top());
        this->setBottom(2 *  ay - this->bottom());

        if (this->bottom() < this->top()) {
            int tmp = this->top();
            this->setTop(this->bottom());
            this->setTop(tmp);
        }

        foreach(Rect* child, _children) {
            //if( strcmp(child->metaObject()->className(),"IcEnclosingRect") == 0){
            //    continue;
            //  }
            child->mirrorY(ay);
        }
        this->updateBoundingRect();
        emit updated();
    }

    void Cell::moveTo(int ax, int ay) {

        int x1 = this->left();
        int y1 = this->top();

        this->setRight(this->right() + ax - this->left());
        this->setBottom(this->bottom() + ay - this->top());
        this->setLeft(ax);
        this->setTop(ay);
        foreach(Rect* child, _children) {
            //if( strcmp(child->metaObject()->className(),"IcEnclosingRect") == 0){
            //    continue;
            //  }
            child->translate( ax - x1, ay -  y1);
        }
        emit updated();

    }

    void Cell::moveCenter(int ax, int ay) {
        this->updateBoundingRect();

        int xc1 = this->centerX();
        int yc1 = this->centerY();

        int xpos = this->left() - ( xc1 - ax );
        int ypos = this->top() - ( yc1 - ay );

        this->moveTo(xpos,ypos);
    }


    void Cell::updateBoundingRect(){
        //Rect r;
        Rect r = this->calcBoundingRect();
        this->setRect(r);
        // qDebug() << r;

    }

    Rect Cell::calcBoundingRect(){


        int x1  = std::numeric_limits<int>::max();
        int y1  = std::numeric_limits<int>::max();
        int x2  = -std::numeric_limits<int>::max();
        int y2  =  -std::numeric_limits<int>::max();

        foreach(Rect* cr, this->_children) {

            int cx1 = cr->x1();
            int cx2 = cr->x2();
            int cy1 = cr->y1();
            int cy2 = cr->y2();

            if (cx1 < x1) {
                x1 = cx1;
            }

            if (cy1 < y1) {
                y1 = cy1;
            }
            if (cx2 > x2) {
                x2 = cx2;
            }
            if (cy2 > y2) {
                y2 = cy2;

            }
        }
        Rect r;

        r.setPoint1(x1,y1);
        r.setPoint2(x2,y2);
//        qWarning() << 		Rect r = c->_cell->calcBoundingRect();r.toString();
        return r;
    }

    QString Cell::toString(){
        QString str;
        str.append(Rect::toString());
        str.append("\n {\n");
        QString strpar = this->metaObject()->className();
        foreach(Rect* child, _children){
            str.append("  ");
            str.append(child->toString());
            str.append("\n");
        }
        str.append(" }");
        return str;

    }

    /*
      void Cell::toPrinter{
      my $self = shift;
      my $printer = shift;
      if (this->schObject) {
      this->schObject->toPrinter($printer);
      }
      }

      void Cell::childrenToGds{
      my $self = shift;
      my $gdsref = shift;
      my $xorg = shift;
      my $yorg = shift;
      my $mirror = shift;
      my $mirror_line = shift;

      my @children = @{this->children};
      foreach my $child (@children) {
      $child->toGds($gdsref,$xorg,$yorg,$mirror,$mirror_line);
      }
      }

      void Cell::toGds{
      my $self = shift;
      my $gdsref = shift;
      my $xorg = shift;
      my $yorg = shift;
      my $mirror = shift;
      my $mirror_line = shift;

      if (this->mirror) {
      $mirror = this->mirror;
      $mirror_line = this->mirror_line;
      }

      if (ref(this->parent) eq "Gds::GdsLib") {
      $gdsref->printBgnstr(-name=>this->name);
      this->childrenToGds($gdsref,$xorg,$yorg,$mirror,$mirror_line);
      if (this->hasPR) {
      this->SUPER::toGds($gdsref,$xorg,$yorg,$mirror,$mirror_line);
      }

      if (this->schObject && this->schObject->can("ports")) {
      my @ports = @{this->schObject->ports};
      if ($gdsref->can("printPin")) {
      print this->schObject."\n";
      foreach my $p (@ports) {
      if ($p->rect) {
      $gdsref->printPin(-obj => $p);
      }
      }
      } elsif ($gdsref->can("printText")) {
      foreach my $p (@ports) {
      if ($p->rect) {
      $p->{xorg} = $xorg;
      $p->{yorg} = $yorg;
      $p->{mirror} = $mirror;
      $p->{mirror_line} = $mirror_line;
      $gdsref->printText(
      -layer => this->rules->layerToNumber($p->layer."_pin"),
      -string => $p->text,
      -dataType => 0,
      -x => ($p->xc + this->snap($p->portSize/2.0) ),
      -y => ($p->yc +this->snap($p->portSize/2.0) ),
      );
      my $port = new Gds::GdsRect($p->layer,$p->xc,$p->yc,$p->portSize,$p->portSize);
      $port->toGds($gdsref,$xorg,$yorg,$mirror,$mirror_line);
      }
      }
      }
      }
      $gdsref->printEndstr();
      } else {
      this->childrenToGds($gdsref,$xorg,$yorg,$mirror,$mirror_line);
      if (this->hasPR) {
      this->SUPER::toGds($gdsref,$xorg,$yorg,$mirror,$mirror_line);
      }

      }
      }
      }

      1;
    */
}
