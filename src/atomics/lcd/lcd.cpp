#include "lcd.h"
#include "ui_lcd.h"
#include <QFile>
#include <QDebug>

LCD::LCD(QString file,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LCD),
    _file(file)
{
    ui->setupUi(this);
    connect(&_timer,SIGNAL(timeout ()),this,SLOT(refresh()));
    _timer.start(500);
}

LCD::~LCD()
{
    delete ui;
}

void LCD::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void LCD::refresh()
{
    QFile qf(_file);
    if (qf.open(QIODevice::ReadOnly)) {
        QString line(qf.readLine());
        line = line.trimmed();
        //qDebug() << line << " " << line.toInt();
        ui->lcdNumber->display(line.toInt());
    }
}
