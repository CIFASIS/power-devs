#include <QDebug>
#include <QFile>
#include "slider.h"
#include "ui_slider.h"

Slider::Slider(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Slider)
{
    ui->setupUi(this);
    QStringList args=QCoreApplication::arguments ();
    if (args.size()==1) {
      printf("Usage:\n\tslider -text Voltage -min -33 -max 90 -value 5 -file out.dat\n");
      exit(0);
    }
    for( int i=0;i<args.size();i++) {
      if (args.at(i)=="-text" && i+1<args.size()) 
        text=args.at(++i);
      if (args.at(i)=="-min" && i+1<args.size()) 
        min=args.at(++i);
      if (args.at(i)=="-max" && i+1<args.size()) 
        max=args.at(++i);
      if (args.at(i)=="-value" && i+1<args.size()) 
        value=args.at(++i);
      if (args.at(i)=="-file" && i+1<args.size()) 
        file=args.at(++i);
    }
    ui->description->setText(QString("%1: %2").arg(text).arg(value));
    ui->dial->setMinimum(min.toInt());
    ui->dial->setMaximum(max.toInt());
    ui->dial->setValue(value.toInt());
    QFile f(file);
    f.open(QIODevice::WriteOnly);
    f.write(qPrintable(QString("%1").arg(value)));
    f.close();
}

Slider::~Slider()
{
    delete ui;
}

void Slider::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void Slider::on_dial_valueChanged(int value) {
  QFile f(file);
  f.open(QIODevice::WriteOnly);
  f.write(qPrintable(QString("%1").arg(value)));
  f.close();
  ui->description->setText(QString("%1: %2").arg(text).arg(value));
}
