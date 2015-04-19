#include <QCoreApplication>
#include "core/design.h"
#include "core/rules.h"
#include <iostream>
#include <QDebug>
#include <QString>

int main(int argc, char *argv[])
{
	
	if(argc ==  3){

		QString file = argv[1];
		QString rules = argv[2];

		//Load rules
		cIcCore::Rules::loadRules(rules);

		//Load design
		cIcCore::Design * c = new cIcCore::Design();
		c->read(file);

	}else{
	   qWarning() << "Wrong number of arguments " << argc;
	  }

		

  
}
