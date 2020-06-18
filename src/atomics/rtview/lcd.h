#include "ui_lcd.h"
#include <QWidget>
#include <QFile>
#include <QTimer>

class LCD: public QWidget, public Ui::lcd
{
  Q_OBJECT
public:
  LCD();
  void setName(QString n) { name->setText(n); };
  void setValue(QString v) { value->display(v); };
  void setFile(QString f);
private slots:
  void timeout();
private:
  QString _file;
  QTimer timer;
};
