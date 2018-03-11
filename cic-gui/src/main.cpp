#include "window.h"
#include <QApplication>

int main(int argc, char *argv[])
{

	if(argc >=  2){
		
        QString file = argv[1];
        QString rules = argv[2];

        //Load rules
        cIcCore::Rules::loadRules(rules);

		
	QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/ALGIC003_STLIB.icns"));
	cIcGui::Window window;
	window.loadFile(file);
	window.show();

	return app.exec();
	
	}else{
		qDebug() << " Usage: cic-gui <rule file> <json file> \n ";
		
	}
	
	return 0;
	

}
