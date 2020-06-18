#ifndef RTVIEW_H
#define RTVIEW_H

#include <QMainWindow>

namespace Ui {
    class RtView;
}

class RtView : public QMainWindow {
    Q_OBJECT
public:
    RtView(QWidget *parent = 0);
    ~RtView();

public slots:
  void newInstance(const QString &args);

private:
    bool running;
    Ui::RtView *ui;
    QList<QWidget*> active;
};

#endif // RTVIEW_H
