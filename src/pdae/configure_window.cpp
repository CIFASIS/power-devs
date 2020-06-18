#include "configure_window.h"
#include <QDebug>
#include <QFileDialog>
 ConfigWindow::ConfigWindow(QWidget * parent, QStringList * h, QStringList * l, QStringList * inc, QStringList * c, QStringList * f):
QDialog
    (parent)
{
	setWindowTitle("Configure");
	qHBox = new QHBoxLayout();
	qHBoxAddFile = new QHBoxLayout();
	qVBox = new QVBoxLayout();
	bAdd = new QPushButton("Add");
	bRemove = new QPushButton("Remove");
	bExit = new QPushButton("Exit");
	bInclude = new QPushButton("Include");
	qHWidget = new QWidget();
	qHAddFileWidget = new QWidget();
	tabWidget = new QTabWidget();
	lvHeaders = new QListWidget();
	lvLibs = new QListWidget();
	lvInclude = new QListWidget();
	lvFlags = new QListWidget();
	lvCPP = new QListWidget();
	addFile = new QLineEdit();
	headers = h;
	libs = l;
	include = inc;
	cpps = c;
	flags = f;
	QStringList::const_iterator i;
	for (i = headers->begin(); i != headers->end(); i++) {
		lvHeaders->addItem(*i);
	}
	for (i = libs->begin(); i != libs->end(); i++) {
		lvLibs->addItem(*i);
	}
	for (i = include->begin(); i != include->end(); i++) {
		lvInclude->addItem(*i);
	}
	for (i = cpps->begin(); i != cpps->end(); i++) {
		lvCPP->addItem(*i);
	}
	for (i = flags->begin(); i != flags->end(); i++) {
		lvFlags->addItem(*i);
	}
	tabWidget->addTab(lvHeaders, "Headers File");
	tabWidget->addTab(lvLibs, "Libs");
	tabWidget->addTab(lvInclude, "Include dirs");
	tabWidget->addTab(lvCPP, "CPP Files");
	tabWidget->addTab(lvFlags, "Compilation Flags");
	qHBox->addWidget(bAdd);
	qHBox->addWidget(bRemove);
	qHBox->addWidget(bExit);
	qHBoxAddFile->addWidget(addFile);
	qHBoxAddFile->addWidget(bInclude);
	qHAddFileWidget->setLayout(qHBoxAddFile);
	qHWidget->setLayout(qHBox);
	qVBox->addWidget(tabWidget);
	qVBox->addWidget(qHAddFileWidget);
	qVBox->addWidget(qHWidget);
	setLayout(qVBox);
	setMinimumWidth(500);
	QObject::connect(bAdd, SIGNAL(clicked()), this, SLOT(addItem()));
	QObject::connect(bRemove, SIGNAL(clicked()), this, SLOT(removeItem()));
	QObject::connect(bExit, SIGNAL(clicked()), this, SLOT(close()));
	QObject::connect(bInclude, SIGNAL(clicked()), this,
			  SLOT(includeText()));
};

void ConfigWindow::addItem()
{
	QString fileName;
	switch (tabWidget->currentIndex()) {
	case 0:
		fileName =
		    QFileDialog::getOpenFileName(this, tr("Include File"), "",
						 "Header Files (*.h)");
		if (fileName != "") {
      if (fileName.contains("atomics",Qt::CaseInsensitive))
      { // If the file is inside the atomics dir, make it relavite
        fileName = fileName.mid(fileName.indexOf("atomics",Qt::CaseInsensitive)+8);
      }
        

			lvHeaders->addItem(fileName);
			headers->append(fileName);
		}
		break;
	case 1:
		fileName =
		    QFileDialog::getOpenFileName(this, tr("Librarie File"), "",
						 "Lib Files (*.lib)");
		if (fileName != "") {
			lvLibs->addItem(fileName);
			libs->append(fileName);
		}
		break;
	case 2:
		fileName =
		    QFileDialog::getExistingDirectory(this,
						      tr("Include directory"),
						      "",
						      QFileDialog::
						      ShowDirsOnly);
		if (fileName != "") {
			lvInclude->addItem(fileName);
			include->append(fileName);
		}
		break;
	case 3:
		fileName = QFileDialog::getOpenFileName(this, tr("Include File"), "", "C++ Files (*.cpp)");
		if (fileName != "") {
      if (fileName.contains("atomics",Qt::CaseInsensitive))
      { // If the file is inside the atomics dir, make it relavite
        fileName = fileName.mid(fileName.indexOf("atomics",Qt::CaseInsensitive)+8);
      }
			lvCPP->addItem(fileName);
			cpps->append(fileName);
		}
		break;
	}
}

void ConfigWindow::removeItem()
{
	QList < QListWidgetItem * >ql;
	switch (tabWidget->currentIndex()) {
	case 0:
		ql = lvHeaders->selectedItems();
		if (!ql.empty()) {
			int row = lvHeaders->row(ql.front());
			lvHeaders->takeItem(row);
			headers->removeAt(row);
		}
		break;
	case 1:
		ql = lvLibs->selectedItems();
		if (!ql.empty()) {
			int row = lvLibs->row(ql.front());
			lvLibs->takeItem(row);
			libs->removeAt(row);
		}
		break;
	case 2:
		ql = lvInclude->selectedItems();
		if (!ql.empty()) {
			int row = lvInclude->row(ql.front());
			lvInclude->takeItem(row);
			include->removeAt(row);
		}
		break;
	case 3:
		ql = lvCPP->selectedItems();
		if (!ql.empty()) {
			int row = lvCPP->row(ql.front());
			lvCPP->takeItem(row);
			cpps->removeAt(row);
		}
		break;
	case 4:
		ql = lvFlags->selectedItems();
		if (!ql.empty()) {
			int row = lvFlags->row(ql.front());
			lvFlags->takeItem(row);
			flags->removeAt(row);
		}
		break;
	}
}

void ConfigWindow::includeText()
{
	QString fileName;
	switch (tabWidget->currentIndex()) {
	case 0:
		if (addFile->text().length()) {
			lvHeaders->addItem(addFile->text());
			headers->append(addFile->text());
			addFile->clear();
		}
		break;
	case 1:
		if (addFile->text().length()) {
			lvLibs->addItem(addFile->text());
			libs->append(addFile->text());
			addFile->clear();
		}
	case 2:
		if (addFile->text().length()) {
			lvInclude->addItem(addFile->text());
			include->append(addFile->text());
			addFile->clear();
		}
		break;
	case 3:
		if (addFile->text().length()) {
			lvCPP->addItem(addFile->text());
			cpps->append(addFile->text());
			addFile->clear();
		}
		break;
	case 4:
		if (addFile->text().length()) {
			lvFlags->addItem(addFile->text());
			flags->append(addFile->text());
			addFile->clear();
		}
		break;
	}
}

void ConfigWindow::exitButton()
{
} 
