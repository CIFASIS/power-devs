
#ifndef CONFIGWINDOW_H
#define CONFIGWINDOW_H

#include <QTabWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>
#include <QLabel>
#include <QtGui>
#include <QPushButton>
#include <QListWidget>
#include <QDialog>
#include <QList>
#include <QString>
#include <QListWidget>
#include <QLineEdit>

class ConfigWindow : public QDialog {

    Q_OBJECT

public:
    ConfigWindow(QWidget *parent =0,QStringList *headers=0,QStringList *libs=0, QStringList *incl=0, QStringList *cpps=0, QStringList *f=0);

    
public slots:
    void addItem();
    void removeItem();
    void exitButton();
    void includeText();

private:
    QHBoxLayout *qHBox;
    QHBoxLayout *qHBoxAddFile;
    QVBoxLayout *qVBox;
    QWidget	*qHWidget;
    QWidget	*qHAddFileWidget;
    QPushButton *bAdd;
    QPushButton *bRemove;
    QPushButton *bExit;
    QPushButton *bInclude;
    QTabWidget  *tabWidget;
    QListWidget    *lvHeaders;
    QListWidget    *lvLibs;
    QListWidget    *lvInclude;
    QListWidget    *lvCPP;
    QListWidget    *lvFlags;
    QStringList	   *headers;
    QStringList	   *flags;
    QStringList	   *libs;
    QStringList	   *include;
    QStringList	   *cpps;
    QLineEdit	   *addFile;


};

#endif
