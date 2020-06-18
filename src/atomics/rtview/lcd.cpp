#include "lcd.h"
#include <QLCDNumber>
LCD::LCD() { 
  setupUi(this); 
  QPalette palette = value->palette();
  palette.setColor(QPalette::WindowText, Qt::red);
  palette.setColor(QPalette::Text, Qt::red);
  QPalette *p = new QPalette(value->palette());
  p->setColor(QPalette::WindowText, Qt::red);
  value->setPalette(*p);
  value->setSegmentStyle(QLCDNumber::Filled);
}
void LCD::setFile(QString f) { 
    _file=f; 
    timer.start(200);
    connect(&timer,SIGNAL(timeout()),this,SLOT(timeout()));
}

void LCD::timeout() {
  QFile file(_file);
  if (file.open(QIODevice::ReadOnly)) {
    QString val(file.readAll()); 
    value->display(val);
    }
}
