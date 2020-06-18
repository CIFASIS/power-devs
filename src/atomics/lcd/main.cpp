#include <QApplication>
#include "lcd.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    if (argc<2)
      return -1;
    LCD *w = new LCD(QString(argv[1]));
    if (argc>2)
        w->setWindowTitle(QString(argv[2]));
    w->show();
    return a.exec();
}
