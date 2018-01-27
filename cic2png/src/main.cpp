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

struct CellHier
{
    double x;
    double y;
    double yshear;
    double xshear;
    double width;
    double height;
    
    Cell * cell;
    QString name;

    QStringList subcells;
};

double unit;


double toUnit(int angstrom){
    return ((double)angstrom)/unit;
    
    ;}


int main(int argc, char *argv[])
{
    unit  = 2000.0;
    
    ConsoleOutput * console = new ConsoleOutput();

    try
    {

        if(argc >=  3){
            QString file = argv[1];
	    QString rules = argv[2];
	    QString filename = argv[3];
            QString outfilename = "outfile";
            
            if(filename == ""){
                QRegularExpression re("^(.*)\\.cicl");
                QRegularExpressionMatch m = re.match(file);
                filename = m.captured(1) + ".hier";
            }

            QRegularExpression re1("/?([^\\/]+)\\.cicl");
            QRegularExpressionMatch m1 = re1.match(file);
            outfilename = m1.captured(1);

            //Load rules
	    cIcCore::Rules::loadRules(rules);

            //Load design, this is where the magic happens
            cIcCore::Design * d= new cIcCore::Design();
            console->comment("Reading JSON: " + file,ConsoleOutput::green);

	    d->readJsonFile(file);

            //Load hiearchy file
            QJsonObject obj = d->readJson(filename);
            console->comment("Reading hierarchy file:" + filename,ConsoleOutput::green);
            Cell * boundary = new Cell();
            QList<CellHier> cells;
            if(obj.contains("unit")){
                QJsonValue unitValue = obj["unit"];
                unit = unitValue.toDouble();
                console->comment("Found unit = " + QString("%1").arg(unit),ConsoleOutput::green);
            }else{
                console->comment("Unit not found using default, unit= " + QString("%1").arg(unit),ConsoleOutput::green);
            }
            
            
            
            QJsonValue hierarchy = obj["hierarchy"];
            QJsonArray cellArray  = hierarchy.toArray();
            foreach (const QJsonValue & value, cellArray) {
                QJsonObject c = value.toObject();
                CellHier ch;
                ch.yshear = c["yshear"].toDouble();
                ch.xshear = c["xshear"].toDouble();
                ch.name = c["name"].toString();
                
                QJsonArray subcells = c["subcells"].toArray();
                foreach (const QJsonValue & subcell, subcells){
                    ch.subcells.append("|" + subcell.toString() + "|");
                }

                
                Cell * cell = Cell::getCell(ch.name);
                if(cell->name() == ""){
                    console->comment("Error: Could not find cell " + ch.name+ "\n",ConsoleOutput::red);
                }else{
                    cell->setBoundaryIgnoreRouting(false);
                    cell->updateBoundingRect();
                    ch.cell = cell;
                    ch.width = toUnit(cell->width());
                    ch.height = toUnit(cell->height());
                    ch.x = c["x"].toDouble()*ch.width;
                    ch.y = c["y"].toDouble()*ch.height;
                    double lx2 = ch.x + ch.width  + abs(ch.height*ch.xshear);
                    double ly2 = ch.y + ch.height + abs(ch.width*ch.yshear);
                    Rect *r = new Rect();
                    r->setPoint1(ch.x,ch.y);
                    r->setPoint2(lx2,ly2);
                    boundary->add(r);                    
                    cells.append(ch);

                }
            }


            Rect r = boundary->calcBoundingRect();
            
            
            QImage * image = new QImage(r.width(),r.height(),QImage::Format_ARGB32_Premultiplied );
            
            QImage &im = *image;

            QPainter painter;
            painter.begin(image);
            
            //Draw background
            painter.setBrush(QBrush("white",Qt::SolidPattern));
            painter.drawRect(0,0,r.width(),r.height());


            cIcPainter::CellPainter *cp = new cIcPainter::CellPainter();

            foreach(const CellHier &  ch,cells){

                painter.save();

                //Scale and paint cell
                QTransform trans;
                trans.translate(ch.x  + abs(ch.height*ch.xshear),ch.y + abs(ch.width*ch.yshear));
                trans.shear(ch.xshear,ch.yshear);
                painter.setTransform(trans);
                painter.scale(1.0/unit,1.0/unit);
                cp->paint(painter,ch.cell,-ch.cell->x1(),-ch.cell->y1(),ch.cell->width(),ch.cell->height(),ch.subcells.join(","));
                painter.restore();
            }


            console->comment("Writing " + outfilename + ".png",ConsoleOutput::green);
            QImageWriter writer(outfilename + ".png");
            writer.setFormat("png");
            writer.write(im);



        }else{
           qWarning() << "Usage: cic2png <CIC file> <Technology file> [<Hierarchy description>]";
      
        }

    }catch(...){
        return -1;
    }





}
