#include "knob.h"

Knob::Knob() { 
  setupUi(this); 
}

void Knob::on_dial_valueChanged(int v) {
  QFile f(_file);
  if (f.open(QIODevice::WriteOnly | QIODevice::Text)) {
    f.write(qPrintable(QString("%1").arg(v)));
    f.close();
  }
  txtVal->setText(QString("%1").arg(v));
}

void Knob::on_txtVal_editingFinished() {
  int val = txtVal->text().toInt();
  if (val< dial->minimum() || val>dial->maximum()) {
    val = dial->minimum();
  }
  dial->setValue(val);
}
