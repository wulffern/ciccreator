/*********************************************************************
 *        Copyright (c) 2010 Carsten Wulff Software, Norway 
 * *******************************************************************
 * Created       : wulff at 2010-9-19
 * Modified      : $Author$ $Date$
 * Version       : $Revision$ $HeadURL$
 * Description   :  
 ********************************************************************/

#ifndef GEDAFILEWRITER_H
#define GEDAFILEWRITER_H

#include "geda/gobject.h"
#include <QString>

class gEdaFileWriter
{
	
private:

public:
    //--------------------------------------------------------
    // ctor and dtor
    //--------------------------------------------------------
    gEdaFileWriter();
    ~gEdaFileWriter();

       void writeFile(QString & str,GObject * o);

public slots:
    
protected:



};

#endif // GEDAFILEWRITER
