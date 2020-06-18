#ifndef LCD_H
#define LCD_H

#include <QDialog>
#include <QTimer>

namespace Ui {
    class LCD;
}

class LCD : public QDialog {
    Q_OBJECT
public:
    LCD(QString file,QWidget *parent = 0);
    ~LCD();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::LCD *ui;
    QString _file;
    QTimer _timer;
private slots:
    void refresh();

};

#endif // LCD_H
