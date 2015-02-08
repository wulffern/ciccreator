//====================================================================
//        Copyright (c) 2015 Carsten Wulff Software, Norway
// ===================================================================
// Created       : wulff at 2015-1-26
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
// See the License for the specific language governing permissions and
// limitations under the License.
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

  Cell::Cell(): Rect(){

  }

  Rect* Cell::getRect(QString layer){
    foreach (Rect* child, _children){
        QString lay = child->layer();
        if (lay.compare(layer) == 0) {
            return child;
          }
      }
    return 0;
  }

  Cell * Cell::createInstance()
  {
    Cell * c = new Cell();
    return c;

  }

  void Cell::add(Rect* child){


    if (child && !_children.contains(child)) {
        child->parent(this);
        this->_children.append(child);
        connect(child,SIGNAL(updated()),this, SLOT(updateBoundingRect()));
      }

    this->updateBoundingRect();
  }

  void Cell::translate(qreal dx, qreal dy) {

    Rect::translate(dx,dy);

    foreach(Rect* child, _children) {
        if( strcmp(child->metaObject()->className(),"IcEnclosingRect") == 0){
            continue;
          }
        child->translate(dx,dy);
      }
    emit updated();

    this->updateBoundingRect();
  }

  void Cell::mirrorX(qreal ax) {
    this->setLeft(2*ax - this->left());
    this->setRight(2*ax - this->right());

    if (this->right() < this->left()) {
        qreal tmp = this->left();
        this->setLeft(this->right());
        this->setRight(tmp);
      }

    foreach(Rect * child, _children) {
        if( strcmp(child->metaObject()->className(),"IcEnclosingRect") == 0){
            continue;
          }
        child->mirrorX(ax);
      }
    this->updateBoundingRect();
    emit updated();
  }

  void Cell::mirrorY(qreal ay) {

    this->setTop(2 *  ay - this->top());
    this->setBottom(2 *  ay - this->bottom());

    if (this->bottom() < this->top()) {
        qreal tmp = this->top();
        this->setTop(this->bottom());
        this->setTop(tmp);
      }

    foreach(Rect* child, _children) {
        if( strcmp(child->metaObject()->className(),"IcEnclosingRect") == 0){
            continue;
          }
        child->mirrorY(ay);
      }
    this->updateBoundingRect();
    emit updated();
  }

  void Cell::moveTo(qreal ax, qreal ay) {

    qreal x1 = this->left();
    qreal y1 = this->top();

    this->setRight(this->right() + ax - this->left());
    this->setBottom(this->bottom() + ay - this->top());
    this->setLeft(ax);
    this->setTop(ay);
    foreach(Rect* child, _children) {
        if( strcmp(child->metaObject()->className(),"IcEnclosingRect") == 0){
            continue;
          }
        child->translate( ax - x1, ay -  y1);
      }
    emit updated();

  }

  void Cell::moveCenter(qreal ax, qreal ay) {
    this->updateBoundingRect();

    qreal xc1 = this->centerX();
    qreal yc1 = this->centerY();

    qreal xpos = this->left() - ( xc1 - ax );
    qreal ypos = this->top() - ( yc1 - ay );

    this->moveTo(xpos,ypos);
  }


  void Cell::updateBoundingRect(){
    QRectF r = this->calcBoundingRect();
    qDebug() << r;
    this->setRect(r);
  }

  QRectF Cell::calcBoundingRect(){
    qreal x1  = std::numeric_limits<qreal>::max();
    qreal y1  = std::numeric_limits<qreal>::max();
    qreal x2  = -std::numeric_limits<qreal>::max();
    qreal y2  =  -std::numeric_limits<qreal>::max();

    qDebug() << x1 << " " << y1 << " " << x2 << " " << y2;
    foreach(Rect* cr, _children) {

        if (cr->left() < x1) {
            x1 = cr->left();
          }

        if (cr->top() < y1) {
            y1 = cr->top();
          }
        if (cr->right() > x2) {
            x2 = cr->right();
          }
        if (cr->bottom() > y2) {
            y2 = cr->bottom();

          }

      }
    QRectF r;
    r.setLeft(x1);
    r.setTop(y1);
    r.setRight(x2);
    r.setBottom(y2);
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
