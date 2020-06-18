/****************************************************************************
**
**  Copyright (C) 2009 Facultad de Ciencia Exactas Ingeniería y Agrimensura
**     Universidad Nacional de Rosario - Argentina.
**  Contact: PowerDEVS Information (kofman@fceia.unr.edu.ar, fbergero@fceia.unr.edu.ar)
**
**  This file is part of PowerDEVS.
**
**  PowerDEVS is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  PowerDEVS is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with PowerDEVS.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

#include <QLineEdit>
#include <QDoubleValidator>
#include <QLabel>
#include <QDebug>
#include <QComboBox>
#include <QSpinBox>
#include <QFileDialog>

#include <dialogs/dlg_change_library.h>
#include <data/coupled.h>
#include <parser.h>
#include <globals.h>

DlgChangeLibrary::DlgChangeLibrary()
{
	setupUi(this);
	libLst = getSetting("libraries").toStringList();
	libEnabled = getSetting("libraryEnabled").toStringList();
	QString libPath = getSetting("Path/libPath").toString();
	foreach(QString str, libLst) {
		Coupled *model = parseModel(str);
		QString iconLib = QString::fromStdString(model->graphic().icon());
		qDebug() << iconLib;
		if (iconLib == "None")
			iconLib = "";
		if (iconLib.startsWith("\%"))
			iconLib =
			    libPath + "/" + iconLib.mid(1).replace("\%",
								   "/").
			    toLower();
		QString name = QString::fromStdString(model->name());
		QListWidgetItem *it =
		    new QListWidgetItem(QIcon(iconLib), name, lsLstWdt);
		QString desc;
		desc.fromStdString(model->description());
		it->setToolTip(desc);
		if (libEnabled.value(libLst.indexOf(str)) == "true")
			it->setCheckState(Qt::Checked);
		else
			it->setCheckState(Qt::Unchecked);
	}
	if (lsLstWdt->count() > 0) {
		lsLstWdt->setFocus();
		lsLstWdt->item(0)->setSelected(true);
	}
	pathLbl->setText(libLst.value(0));
}

void DlgChangeLibrary::on_buttonBox_accepted()
{
	setSetting("libraries",libLst);
	int items = lsLstWdt->count();
	for (int k = 0; k < items; k++) {
		if (lsLstWdt->item(k)->checkState())
			libEnabled.replace(k, "true");
		else
			libEnabled.replace(k, "false");
	}
	setSetting("libraryEnabled", libEnabled);

}

void DlgChangeLibrary::on_addBtn_clicked()
{
	QDir dir(QCoreApplication::applicationDirPath());
	QString libpath = getSetting("path/libPath").toString();
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open Library"), "../bin",
					 "Libraries of PowerDEVS (*.pdl);;All Files (*.*)");
	if (fileName == NULL)
		return;
	Coupled *model = parseModel(dir.relativeFilePath(fileName));
	QString iconLib = QString::fromStdString(model->graphic().icon());
	if (iconLib == "None")
		iconLib = "";
	if (iconLib.startsWith("\%"))
		iconLib =
		    libpath + "/" + iconLib.mid(1).replace("\%", "/").toLower();
	QString name = QString::fromStdString(model->name());
	QListWidgetItem *it =
	    new QListWidgetItem(QIcon(iconLib), name, lsLstWdt);
	QString desc;
	desc.fromStdString(model->description());
	it->setToolTip(desc);
	it->setCheckState(Qt::Checked);
	libLst << dir.relativeFilePath(fileName);
	libEnabled << "true";
}

void DlgChangeLibrary::on_removeBtn_clicked()
{
	if(lsLstWdt->count()){
		libLst.removeAt(lsLstWdt->currentRow());
		libEnabled.removeAt(lsLstWdt->currentRow());
		lsLstWdt->takeItem(lsLstWdt->currentRow());
	}
}

void DlgChangeLibrary::on_upBtn_clicked()
{
	int row = lsLstWdt->currentRow();
	if (row > 0) {
		libLst.move(row, row - 1);
		libEnabled.move(row, row - 1);
		QListWidgetItem *tmp = lsLstWdt->takeItem(row);
		lsLstWdt->insertItem(row - 1, tmp);
		lsLstWdt->setCurrentItem(tmp);
	}
}

void DlgChangeLibrary::on_downBtn_clicked()
{
	int row = lsLstWdt->currentRow();
	if (row < (lsLstWdt->count() - 1) && row >= 0) {
		libLst.move(row, row + 1);
		libEnabled.move(row, row + 1);
		QListWidgetItem *tmp = lsLstWdt->takeItem(row);
		lsLstWdt->insertItem(row + 1, tmp);
		lsLstWdt->setCurrentItem(tmp);
	}
}	

void DlgChangeLibrary::on_lsLstWdt_currentRowChanged(int row)
{
	pathLbl->setText(libLst.value(row));
}

