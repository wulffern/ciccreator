#include "window.h"
#include "version.h"
#include <QApplication>

int main(int argc, char *argv[])
{

    int error =0;
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
        qWarning() << "Usage:   cic-gui <JSON file> <Technology file>";
        qWarning() << "Example: cic-gui SAR_ESSCIRC16_28N.cic examples/tech.json ";
        qWarning() << "About: \n\tcIcCreator GUI reads .cic file and shows the cells in a GUI" <<
            "\n\tVersion" << CICVERSION <<
            "\n\tHash" << CICHASH << "\n";
		error = 1;
	}
	
	return error;
	

}
