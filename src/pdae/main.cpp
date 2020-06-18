#include <QApplication>

#include "pdae.h"
int main(int argc, char *argv[]) 
{
	QApplication app(argc, argv);
	PDAE pdae(NULL, argv[1]);
	pdae.show();
	return app.exec();
}


