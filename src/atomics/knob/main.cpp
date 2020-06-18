#include <QApplication>
#include "slider.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Slider w;
    w.show();
    return a.exec();
}
