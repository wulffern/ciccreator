#include <QCoreApplication>
#include "core/design.h"
#include <iostream>
#include <QDebug>
#include <QString>

int main(int argc, char *argv[])
{

	qWarning() << "argc : " << argc;
	qWarning() << "argv[0] : " << argv[0];
	qWarning() << "argv[1] : " << argv[1];
	
	if(argc ==  2){
		cIcCore::Design * c = new cIcCore::Design();
		QString file = argv[1];
		qWarning() << "TEST " << file;
		
		c->read(file);
	}

		

  
}
