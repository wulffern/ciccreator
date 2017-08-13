/*********************************************************************
 *        Copyright (c) 2010 Carsten Wulff Software, Norway
 * *******************************************************************
 * Created       : wulff at 2010-8-25
 * Modified      : $Author$ $Date$
 * Version       : $Revision$ $HeadURL$
 * Description   :
 ********************************************************************/

#ifndef GFILEDATA_H
#define GFILEDATA_H


#include <QDebug>
#include <QString>
#include <QLineF>
#include <QStringList>
#include <QTextStream>
#include "defines/defines.h"

//-----------------------------------------------------------------
// GObject
//-----------------------------------------------------------------
class GObject{

private:

protected:
    QList<GObject*> m_children;


public:
    GObject(){
    }
    GObject(QStringList s){
        orgstring = s.join(" ");
        GObject();
    }
    QString type;
    QString string;
    QString orgstring;
    virtual QString & toString()
    {
        return *(new QString(type));
    };

    virtual QString & childrenToString(QString & string)
    {
        int nrtext = 0;
        QStringList strs;
        strs.append(string);
        strs.append(STARTATTACH_ATTR);
        foreach(GObject *o,children()){
            if(o->type == OBJ_TEXT){
                strs.append(o->toString());
                nrtext++;
            }
        }
        strs.append(ENDATTACH_ATTR);
        if(nrtext > 0){
            string = strs.join("\n");
        }

        return string;
    }
    

    QList<GObject*> &children(){
        return m_children;
    }

    void addChild(GObject * g){
        m_children.append(g);
    }

    void removeChild(GObject *g){
        m_children.removeOne(g);
    }


    //From G coordinates
    int fg(int i){
        return i/G_SCALE;
    }

    //To G coordinates
    int tg(int i){
        return i*G_SCALE;
    }
};



    

//-----------------------------------------------------------------
// GVersion
//-----------------------------------------------------------------
    class GVersion:public GObject{
    public:
        int version;
        int fileformat_version;

        GVersion(){
            version = 20170226;
            fileformat_version = 2;
        }

        GVersion(QStringList & s):GObject(s){
            type = OBJ_VERSION;
            version = s[1].toInt();
            fileformat_version = s[2].toInt();
        }

        QString & toString(){
            string = QString("%1 %2 %3")
                .arg(OBJ_VERSION)
                .arg(version)
                .arg(fileformat_version);
            return string;
        }
    };

//-----------------------------------------------------------------
// GLine
//-----------------------------------------------------------------
class GLine:public GObject{
    /*
      Valid in: Schematic and Symbol files
      type x1 y1 x2 y2 color width capstyle dashstyle dashlength dashspace
    */
public:
    QLineF line;
    int color;
    int width;
    int capstyle;
    int dashstyle;
    int dashlength;
    int dashspace;

    GLine():GObject(){

    }

    GLine(QStringList & s):GObject(s){
        type = OBJ_LINE;
        int x1 = fg(s[1].toInt());
        int y1 = fg(s[2].toInt());
        int x2 = fg(s[3].toInt());
        int y2 = fg(s[4].toInt());
        line = QLineF(x1,y1,x2,y2);
        color = s[5].toInt();
        width = s[6].toInt();
        capstyle = s[7].toInt();
        dashstyle = s[8].toInt();
        dashlength = s[9].toInt();
        dashspace= s[10].toInt();
    }

    QString & toString(){
        string = QString("%1 %2 %3 %4 %5 %6 %7 %8 %9 %10 %11")
            .arg(OBJ_LINE)
            .arg(tg(line.x1()))
            .arg(tg(line.y1()))
            .arg(tg(line.x2()))
            .arg(tg(line.y2()))
            .arg(color)
            .arg(width)
            .arg(capstyle)
            .arg(dashstyle)
            .arg(dashlength)
            .arg(dashspace);
        return string;
    }

    inline static GLine * commentLine(QPointF p1,QPointF p2){
        return commentLine(p1.x(),p1.y(),p2.x(),p2.y());
    }

    static GLine * commentLine(int x1, int y1, int x2, int y2){
        GLine * g = new GLine();
        g->type = OBJ_LINE;
        g->line = QLineF(x1,y1,x2,y2);
        g->color = TEXT_COLOR;
        g->width = 2;
        g->capstyle = 0;
        g->dashstyle = 0;
        g->dashlength = -1;
        g->dashspace = -1;
        return g;
    }
};

//-----------------------------------------------------------------
// GPicture
//-----------------------------------------------------------------
class GPicture:public GObject{
    /*
      type x1 y1 width height angle ratio mirrored embedded
      filename
      [encoded picture data
      encoded picture end]
    */
public:
    int x;
    int y;
    int width;
    int height;
    int angle;
    char mirrored;
    char embedded;
    QString filename;
    QString base64_picture_data;
};

//-----------------------------------------------------------------
// GBox
//-----------------------------------------------------------------
class GBox:public GObject{
    /*
      type = B
      Valid in: Schematic and Symbol files
      type x y width height color width capstyle dashtype dashlength dashspace filltype fillwidth angle1 pitch1 angle2 pitch2
    */
public:
    int x;
    int y;
    int width;
    int height;
    int color;
    int linewidth;
    int capstyle;
    int dashstyle;
    int dashlength;
    int dashspace;
    int filltype;
    int fillwidth;
    int angle1;
    int pitch1;
    int angle2;
    int pitch2;

    GBox():GObject(){

    }

    GBox(QStringList & s):GObject(s){
        type = OBJ_BOX;
        x = fg(s[1].toInt());
        y = fg(s[2].toInt());
        width = fg(s[3].toInt());
        height = fg(s[4].toInt());
        color = s[5].toInt();
        linewidth = s[6].toInt();
        capstyle = s[7].toInt();
        dashstyle = s[8].toInt();
        dashlength = s[9].toInt();
        dashspace = s[10].toInt();
        filltype = s[11].toInt();
        fillwidth = s[12].toInt();
        angle1 = s[13].toInt();
        pitch1 = s[14].toInt();
        angle2 = s[15].toInt();
        pitch2 = s[16].toInt();
    }

    QString & toString(){
        string = QString("%1 %2 %3 %4 %5 %6 %7 %8 %9 %10 %11 %12 %13 %14 %15 %16 %17")
            .arg(OBJ_BOX)
            .arg(tg(x))
            .arg(tg(y))
            .arg(tg(width))
            .arg(tg(height))
            .arg(color)
            .arg(linewidth)
            .arg(capstyle)
            .arg(dashstyle)
            .arg(dashlength)
            .arg(dashspace)
            .arg(filltype)
            .arg(fillwidth)
            .arg(angle1)
            .arg(pitch1)
            .arg(angle2)
            .arg(pitch2);

        return string;
    }


    static GBox * commentBox(int x, int y){
        GBox * g = new GBox();
        g->type = OBJ_BOX;
        g->x = x;
        g->y = y;
        g->width = 1;
        g->height = 1;
        g->color = TEXT_COLOR;
        g->linewidth = 1;
        g->width = 1;
        g->capstyle = 0;
        g->dashstyle = 0;
        g->dashlength = -1;
        g->dashspace = -1;
        g->filltype = 0;
        g->fillwidth = 0;
        g->angle1 = 0;
        g->pitch1 = 0;
        g->angle2 = 0;
        g->pitch2 = 0;

        return g;
    }
};

//-----------------------------------------------------------------
// GCircle
//-----------------------------------------------------------------
class GCircle:public GObject{
    /*
      Valid in: Schematic and Symbol files
      type x y radius color width capstyle dashtype dashlength dashspace filltype fillwidth angle1 pitch1 angle2 pitch2
    */
public:
    int x;
    int y;
    int radius;
    int color;
    int width;
    int capstyle;
    int dashstyle;
    int dashlength;
    int dashspace;
    int filltype;
    int fillwidth;
    int angle1;
    int pitch1;
    int angle2;
    int pitch2;
};

//-----------------------------------------------------------------
// GArc
//-----------------------------------------------------------------
class GArc: public GObject{
    /*
      Valid in: Schematic and Symbol files
      type x y radius startangle sweepangle color width capstyle dashtype dashlength dashspace
    */
public:
    int x;
    int y;
    int raidus;
    int startangle;
    int sweepangle;
    int color;
    int width;
    int capstyle;
    int dashstyle;
    int dashlength;
    int dashspace;
};


//-----------------------------------------------------------------
// GText
//-----------------------------------------------------------------
class GText:public GObject{
    /*
      Valid in: Schematic and Symbol files
      type x y color size visibility show_name_value angle alignment num_lines
      string line 1
      string line 2
      string line 3
      …
      string line N
    */
public:
    int x;
    int y;
    int color;
    int size;
    int visibility;
    int show_name_value;
    int angle;
    int alignment;
    int num_lines;
    QStringList strings;
    QStringList filteredStrings;

    GText():GObject(){
    }

    GText(QStringList & s,QTextStream & in):GObject(s){
        type = OBJ_TEXT;
        x = fg(s[1].toInt());
        y = fg(s[2].toInt());
        color = s[3].toInt();
        size = s[4].toInt();
        visibility = s[5].toInt();
        show_name_value = s[6].toInt();
        angle = s[7].toInt();
        alignment = s[8].toInt();
        num_lines = s[9].toInt();
        QStringList lines;
        lines.append(s.join(" "));
        QString line1;
        for(int i=0;i< num_lines;i++){
            line1 = in.readLine();
            strings.append(line1);
            lines.append(line1);
        }
        orgstring = lines.join("\n");
        filterStrings();
    }

    QString & toString(){
        num_lines = strings.count();
        string = QString("%1 %2 %3 %4 %5 %6 %7 %8 %9 %10")
            .arg(OBJ_TEXT)
            .arg(tg(x))
            .arg(tg(y))
            .arg(color)
            .arg(size)
            .arg(visibility)
            .arg(show_name_value)
            .arg(angle)
            .arg(alignment)
            .arg(num_lines);
        QStringList str;
        str << string << strings;
        string  = str.join("\n");
        return string;
    }

    static GText * commentText(int x, int y,QString & str){
        GText * t = new GText();
        t->type = OBJ_TEXT;
        t->x = x;
        t->y = y;
        t->color = TEXT_COLOR;
        t->size = 6;
        t->visibility = 1;
        t->show_name_value = 0;
        t->angle = 0;
        t->alignment = 0;
        t->num_lines = 0;
        t->strings.append(str);
        t->filterStrings();
        return t;
    }

    static GText * pinText(int x, int y,QString key, QString value){
        GText * t = new GText();
        t->type = OBJ_TEXT;
        t->x = x;
        t->y = y;
        t->color = TEXT_COLOR;
        t->size = 6;
        t->visibility = 1;
        t->show_name_value = 1;
        t->angle = 0;
        t->alignment = 0;
        t->num_lines = 0;
        t->strings.append(key + "=" +  value);
        t->filterStrings();
        return t;
    }



    void filterStrings(){
        /*SHOW NAME VALUE = 0 (show both name and value of an attribute)
          SHOW VALUE = 1 (show only the value of an attribute)
          SHOW NAME = 2 (show only the name of an attribute)
        */
        if(visibility == 1){
            foreach(QString s, strings){
                QStringList keyval = s.split("=");
                if(keyval.count() < 2){
                    filteredStrings.append(s);
                    continue;
                }
                switch(show_name_value){
                case 0:
                    filteredStrings.append(s);
                    break;
                case 1:
                    filteredStrings.append(keyval[1]);
                    break;
                case 2:
                    filteredStrings.append(keyval[0]);
                    break;
                }
            }
        }
    }
};

//-----------------------------------------------------------------
// GNet
//-----------------------------------------------------------------
class GNet:public GObject{
    /*
      Valid in: Schematic files ONLY
      type x1 y1 x2 y2 color
    */
public:

    QLineF line;
    int color;

    GNet():GObject(){
    }

    GNet(QStringList & s):GObject(s){
        type = OBJ_NET;
        int x1 = fg(s[1].toInt());
        int y1 = fg(s[2].toInt());
        int x2 = fg(s[3].toInt());
        int y2 = fg(s[4].toInt());
        line = QLineF(x1,y1,x2,y2);
        color = s[5].toInt();
        orgstring = s.join(" ");
    }

    QString & toString(){
        string = QString("%1 %2 %3 %4 %5 %6")
            .arg(OBJ_NET)
            .arg(tg(line.x1()))
            .arg(tg(line.y1()))
            .arg(tg(line.x2()))
            .arg(tg(line.y2()))
            .arg(color);

        QStringList strs;
        int nrtext = 0;
        strs.append(string);
        strs.append(STARTATTACH_ATTR);
        foreach(GObject *o,children()){
            if(o->type == OBJ_TEXT){
                strs.append(o->toString());
                nrtext++;
            }
        }
        strs.append(ENDATTACH_ATTR);
        if(nrtext > 0){
            string = strs.join("\n");
        }
        return string;
    }

    inline static GNet * newNet(QPointF p1,QPointF p2){
        return newNet(p1.x(),p1.y(),p2.x(),p2.y());
    }

    static GNet * newNet(int x1, int y1, int x2, int y2){
        GNet * g = new GNet();
        g->type = OBJ_NET;
        g->line = QLineF(x1,y1,x2,y2);
        g->color = NET_COLOR;
        return g;
    }



};

//-----------------------------------------------------------------
// GBus
//-----------------------------------------------------------------
class GBus:public GObject{
    /*
      Valid in: Schematic files ONLY
      type x1 y1 x2 y2 color ripperdir
    */
public:
    int x1;
    int y1;
    int x2;
    int y2;
    int color;
    int ripperdir;
};

//-----------------------------------------------------------------
// GPin
//-----------------------------------------------------------------
class GPin:public GObject{
    /*
      Valid in: Symbol files ONLY
      type x1 y1 x2 y2 color pintype whichend
    */
public:
    int x1;
    int y1;
    int x2;
    int y2;
    int color;
    int pintype;
    int whichend;

    GPin():GObject(){}


    GPin(QStringList & s):GObject(s){
        type = OBJ_PIN;
        x1 = fg(s[1].toInt());
        y1 = fg(s[2].toInt());
        x2 = fg(s[3].toInt());
        y2 = fg(s[4].toInt());
        color = s[5].toInt();
        pintype = s[6].toInt();
        whichend = s[7].toInt();
        orgstring = s.join(" ");
    }

    QString & toString(){
        string = QString("%1 %2 %3 %4 %5 %6 %7 %8")
            .arg(OBJ_PIN)
            .arg(tg(x1))
            .arg(tg(y1))
            .arg(tg(x2))
            .arg(tg(y2))
            .arg(color)
            .arg(pintype)
            .arg(whichend);

        string = childrenToString(string);
        
        
        return string;
    }

    static GPin * getPin(QString & aPinlabel, int aPinnumber,int ax, int ay)
    {
        GPin * p = new GPin();
        p->x1 = ax;
        p->y1 = ay;
        p->x2 = ax + 20;
        p->y2 = ay;
        p->color = 0;
        p->pintype = PIN_TYPE_NET;
        p->whichend = 0; //use x1 as pin

        
        GText * pinlabel = GText::pinText(ax + 22, ay-4, "pinnumber",aPinlabel);
        GText * pinnumber = GText::pinText(ax + 22, ay-4, "pinlabel", aPinlabel);
        pinnumber->visibility = 0;
        GText * pinseq = GText::pinText(ax + 5,ay +1, "pinseq",QString("%1").arg(aPinnumber));
        p->addChild(pinlabel);
        p->addChild(pinnumber);
        p->addChild(pinseq);
        return p;
    }
    

};

//-----------------------------------------------------------------
// GComponent
//-----------------------------------------------------------------
class GComponent:public GObject{
    /*
      Valid in: Schematic files ONLY
      type x y selectable angle mirror basename
    */
public:
    int x;
    int y;
    int selectable;
    int angle;
    int mirror;
    QString basename;
    GObject * symbol;

    GComponent():GObject(){}

    GComponent(QStringList & s):GObject(s){
        type = OBJ_COMPLEX;
        x = fg(s[1].toInt());
        y = fg(s[2].toInt());
        selectable = s[3].toInt();
        angle = s[4].toInt();
        mirror = s[5].toInt();
        basename = s[6];

    }

    QString & toString(){
        string = QString("%1 %2 %3 %4 %5 %6 %7")
            .arg(OBJ_COMPLEX)
            .arg(tg(x))
            .arg(tg(y))
            .arg(selectable)
            .arg(angle)
            .arg(mirror)
            .arg(basename);

        string = childrenToString(string);
        
        return string;
    }

    static GComponent * newInstance(QString &basename, int x, int y){
        GComponent *c = new GComponent();
        c->type = OBJ_COMPLEX;
        c->x = x;
        c->y = y;
        c->selectable = 1;
        c->angle = 0;
        c->mirror = 0;
        c->basename = basename;
        return c;
    }

};

//-----------------------------------------------------------------
// GPath
//-----------------------------------------------------------------
class GPath:public GObject{
    /*
      Valid in: Schematic and Symbol files
      Valid since: Fileformat version 2 (release 1.5.1)
      type color width capstyle dashstyle dashlength dashspace filltype fillwidth angle1 pitch1 angle2 pitch2 numlines
      path data line 1
      path data line 2
      path data line 3
      …
      path data line N
    */
public:
    int color;
    int width;
    int capstyle;
    int dashstyle;
    int dashlength;
    int filltype;
    int fillwidth;
    int angle1;
    int pitch1;
    int angle2;
    int pitch2;
    int num_lines;
    QStringList strings;
};

//-----------------------------------------------------------------
// GSchematic
//-----------------------------------------------------------------
class GSchematic:public GObject
{
public:
    QString name;
    GSchematic():GObject()
    {
        this->addChild(new GVersion());
    }

};


//-----------------------------------------------------------------
// GSymbol
//-----------------------------------------------------------------
class GSymbol:public GObject
{
public:
    QString name;
    GSymbol():GObject()
    {
        this->addChild(new GVersion());
    }
    
};


#endif //GFILEDATA
