#ifndef SLIDER_H
#define SLIDER_H

#include <QMainWindow>

namespace Ui {
    class Slider;
}

class Slider : public QMainWindow {
    Q_OBJECT
public:
    Slider(QWidget *parent = 0);
    ~Slider();

protected:
    void changeEvent(QEvent *e);
private slots:
    void on_dial_valueChanged(int);

private:
    Ui::Slider *ui;
    QString min,max,text,file,value;
};

#endif // SLIDER_H
