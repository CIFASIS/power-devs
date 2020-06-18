#ifndef PDAE_H
#define PDAE_H

#include <QTabWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>
#include <QLabel>
#include <QtGui>
#include <QDebug>
#include <QTextCursor>
#include <QList>
#include <QMainWindow>
#include <QCoreApplication>
#include <QProcess>
#include <QTextEdit>

#include "highlighter.h"
#include "configure_window.h"

class QTextEdit;

class PDAE : public QMainWindow
{
    Q_OBJECT

public:
    PDAE(QWidget *parent = 0,QString file="");
protected:
     void closeEvent(QCloseEvent *event);

public slots:
    void newFile();
    void openFile(const QString &path = QString());
    void saveFile();
    void saveAsFile();
    void checkFile();
    void configureFile();
    void textChange();
		void closeComp(bool);

	void keyReleaseEvent ( QKeyEvent * event );

private slots:
  void onStateCursorChanged();
  void onInitCursorChanged();
  void onDintCursorChanged();
  void onDextCursorChanged();
  void onTaCursorChanged();
  void onLambdaCursorChanged();
  void onExitCursorChanged();
  void tabChanged(int );
private:
    void setupEditor();
    void setupFileMenu();
    void setupToolsMenu();
    void setupHelpMenu();
    void saveFileToCpp(bool);

    QTextEdit *editorState;
    QTextEdit *editorInit;
    QTextEdit *editorTa;
    QTextEdit *editorDInt;
    QTextEdit *editorDExt;
    QTextEdit *editorLambda;
    QTextEdit *editorExit;
    QTabWidget *tabWidget;
    Highlighter *highlighter;
    QHBoxLayout *qhboxLay;
    QVBoxLayout *qvboxLay;
    QWidget	*qhboxWidget;
    QWidget	*qvboxWidget;
    QDockWidget	*qvboxDockWidget;
    QLabel	*qlabel;
    bool 	dirty,firsttime;
    QString	name;
    QStringList cpps;
    QStringList libs;
    QStringList headers;
    QStringList headersDirs;
    QStringList flags;
    QDockWidget *compilerOut;


};

#endif
