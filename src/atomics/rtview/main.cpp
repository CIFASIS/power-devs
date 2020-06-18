#include <QDebug>
#include <QApplication>
#include <qtsingleapplication.h>
#include "rtview.h"

int main(int argc, char *argv[])
{
    QString args;
    for (int i=0;i<argc;i++)
      args += (i > 0 ? " " : "") + QString(argv[i]);
    QtSingleApplication instance(argc, argv);
    if (instance.sendMessage(args))
	    return 0;
    RtView w;
    w.show();

    QObject::connect(&instance, SIGNAL(messageReceived(const QString&)),
		     &w, SLOT(newInstance(const QString&)));
    w.newInstance(args);
    return instance.exec();
}
