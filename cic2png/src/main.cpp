//====================================================================
//        Copyright (c) 2016 Carsten Wulff Software, Norway
// ===================================================================
// Created       : wulff at 2016-12-5
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


#include "cic-core.h"
#include <iostream>
#include <QDebug>
#include <QString>



using namespace cIcCore;
using namespace std;

#define UNIT 4000.0

double toUnit(int angstrom){
    return angstrom/UNIT;
    ;}


int main(int argc, char *argv[])
{

    try
    {

        if(argc >=  3){

            QString file = argv[1];
            QString rules = argv[2];
            QString cell = argv[3];

            if(cell == ""){
                QRegularExpression re("/?([^\\/]+)\\.json");
                QRegularExpressionMatch m = re.match(file);
                cell = m.captured(1);
            }

            //Load rules
            cIcCore::Rules::loadRules(rules);

            //Load design, this is where the magic happens
            cIcCore::Design * d= new cIcCore::Design();
            d->read(file);

            Cell * c = Cell::getCell(cell);

            Qt::BrushStyle bstyle = Qt::SolidPattern;

            if(c->name() == ""){
                qDebug() << "Could not find cell " << cell << "\n";
                return -1;}

            c->setBoundaryIgnoreRouting(false);
            c->updateBoundingRect();

            double xmargin = c->width()*0.01;
            double ymargin = c->height()*0.01;
            double margin = ymargin < xmargin ? ymargin: xmargin;
            double width = toUnit(c->width() + margin*2);
            double height = toUnit(c->height() + margin*2);
            QImage * image = new QImage(width,height,QImage::Format_ARGB32_Premultiplied );
            QImage &im = *image;

            QPainter painter;
            painter.begin(image);

            //Draw background
            painter.setBrush(QBrush("white",bstyle));
            painter.drawRect(0,0,width,height);

            //Add margin
            painter.translate(toUnit(margin),toUnit(margin));



            cIcPainter::CellPainter *cp = new cIcPainter::CellPainter();

            painter.save();
            //Scale and paint cell
            QTransform trans;
//            trans.scale(0.8,0.8);
            trans.translate(width*0.1*5,height*0.1*5);
            trans.shear(0,-0.4);
//            painter.setTransform(trans);
            painter.scale(1/UNIT,1/UNIT);
            cp->paint(painter,c,-c->x1(),-c->y1(),c->width(),c->height());
            painter.restore();



            QString prev_group = "";

            if(c->isLayoutCell()){
                LayoutCell * cc = (LayoutCell*) c;

                foreach(cIcSpice::SubcktInstance * ckt_inst,c->subckt()->instances()){
                    QString group = ckt_inst->groupName();

                    if(prev_group.compare(group) != 0){


                        Instance* i = cc->getInstanceFromInstanceName(ckt_inst->name());
                        if(i){

                            //TODO: Need to figure out how to use paintReference
                            painter.save();
                            //Scale and paint cell
                            QTransform trans;
                            trans.scale(0.8,0.8);
                            trans.translate(width*0.1*5,height*0.1*5);
                            trans.shear(0,-0.4);
                            painter.setTransform(trans);
                            painter.scale(1/UNIT,1/UNIT);
                            Point* p = i->getCellPoint();
//                            painter.translate(-p->x,p->y);
                            
//                            cp->paint(painter,i->cell(),-p->x-c->x1(),-p->y-c->y1(),c->width(),c->height());
                            painter.restore();

                        }



                    }

                    prev_group = group;

                }

            }


            painter.end();


            QImageWriter writer(cell + ".png");
            writer.setFormat("png");
            writer.write(im);



        }else{
            qWarning() << "Wrong number of arguments " << argc;
        }

    }catch(...){
        return -1;
    }





}
