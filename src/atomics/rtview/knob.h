#include "ui_knob.h"
#include <QWidget>
#include <QFile>

class Knob: public QWidget, public Ui::knob
{
  Q_OBJECT
public:
  Knob();
  void setName(QString n) { name->setText(n); }
  void setValue(int value) { dial->setValue(value); txtVal->setText(QString("%1").arg(value));}
  void setMin(int min) { dial->setMinimum(min); }
  void setMax(int max) { dial->setMaximum(max); }
  void setFile(QString file) { _file = file; }
private slots:
  void on_dial_valueChanged(int);
  void on_txtVal_editingFinished();
private:
  QString _file;
};
