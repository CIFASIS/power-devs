#include <QDebug>
#include <QPushButton>
#include "rtview.h"
#include "ui_rtview.h"
#include "lcd.h"
#include "knob.h"


RtView::RtView(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RtView)
{
    ui->setupUi(this);
    running = false;
}

RtView::~RtView()
{
    delete ui;
}

void RtView::newInstance(const QString &s)
{
  QStringList args = s.split(" ");
  qDebug() << args;
  if (args.size()==1) return;
  if (args.at(1)=="clean") {
    running = false;
    foreach(QWidget *w, active) {
      w->setEnabled(false);
    }
    qDebug() << "Next clean";
    return;
  }
  if (running==false) {
    foreach(QWidget *w, active) {
      ui->topLayout->removeWidget(w);
      ui->bottomLayout->removeWidget(w);
      delete w;
    } 
    running=true;
  } 
  if (args.at(1)=="knob") {
    Knob *k = new Knob();
    ui->topLayout->addWidget(k);
    active << k;
    for (int i=2;i<args.size();i++) {
      if (args.at(i)=="-min" && i+1<args.size()) 
        k->setMin(args.at(++i).toInt());
      if (args.at(i)=="-max" && i+1<args.size()) 
        k->setMax(args.at(++i).toInt());
      if (args.at(i)=="-name" && i+1<args.size()) 
        k->setName(args.at(++i));
      if (args.at(i)=="-value" && i+1<args.size()) 
        k->setValue(args.at(++i).toInt());
      if (args.at(i)=="-file" && i+1<args.size()) 
        k->setFile(args.at(++i));
    }
    ui->topLayout->addWidget(k);
  } else {  
    LCD *l = new LCD();
    active << l;
    for (int i=2;i<args.size();i++) {
      if (args.at(i)=="-name" && i+1<args.size()) 
        l->setName(args.at(++i));
      if (args.at(i)=="-value" && i+1<args.size()) 
        l->setValue(args.at(++i));
      if (args.at(i)=="-file" && i+1<args.size()) 
        l->setFile(args.at(++i));
    }
    ui->bottomLayout->addWidget(l);
  }
}
