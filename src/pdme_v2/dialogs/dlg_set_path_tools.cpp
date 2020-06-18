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
#include <QFileDialog>
#include <QSettings>
#include <dialogs/dlg_set_path_tools.h>
#include <globals.h>

DlgSetPathTools::DlgSetPathTools()
{
	setupUi(this);
	
	QString atomicsEditor = getSetting("Path/atomicsEditor").toString();
	QString parametersEditor = getSetting("Path/parametersEditor").toString();
	QString atomicsPath = getSetting("Path/atomicsPath").toString();
	QString filterPath = getSetting("Path/filterPath").toString();
	QString helpFile = getSetting("Path/helpFile").toString();
	
	editorLed->setText(atomicsEditor);
	paramLed->setText(parametersEditor);
	pathLed->setText(atomicsPath);
	filterLed->setText(filterPath);
	helpLed->setText(helpFile);
}

void DlgSetPathTools::on_editorBtn_clicked()
{
	QDir dir(QCoreApplication::applicationDirPath());
	QString binPath = getSetting("Path/binPath").toString();
	QString fileName =
	    QFileDialog::getOpenFileName(this, tr("Atomics Editor"), binPath,
					 "Program Files *;;All Files (*.*)");
	if (!fileName.isEmpty())
		editorLed->setText(dir.relativeFilePath(fileName));
}

void DlgSetPathTools::on_pathBtn_clicked()
{
	QDir dir(QCoreApplication::applicationDirPath());
	QString atomicsPath = getSetting("Path/atomicsPath").toString();
	QString dirName =
	    QFileDialog::getExistingDirectory(this, tr("Path Browser"), atomicsPath,
					      QFileDialog::
					      ShowDirsOnly | QFileDialog::
					      DontResolveSymlinks);
	if (!dirName.isEmpty())
		pathLed->setText(dir.relativeFilePath(dirName));
}

void DlgSetPathTools::on_helpBtn_clicked()
{
	QDir dir(QCoreApplication::applicationDirPath());
	QString binPath = getSetting("Path/binPath").toString();
	QString fileName =
	    QFileDialog::getOpenFileName(this, "Help File", binPath,
					 "Help Files *;;HyperText Files (*.html);;All Files (*.*)");
	if (!fileName.isEmpty())
		helpLed->setText(dir.relativeFilePath(fileName));
	return;
}

void DlgSetPathTools::on_buttonBox_accepted()
{
	QString atomicsEditor = editorLed->text();
	QString parametersEditor = paramLed->text();
	QString atomicsPath = pathLed->text();
	QString filterPath = filterLed->text();
	QString helpFile = helpLed->text();
#ifdef Q_OS_LINUX
    atomicsEditor = atomicsEditor.replace('\\','/');
    atomicsPath = atomicsPath.replace('\\','/');
    filterPath = filterPath.replace('\\','/');
    helpFile = helpFile.replace('\\','/');
#else
    atomicsEditor = atomicsEditor.replace('/','\\');
    atomicsPath = atomicsPath.replace('/','\\');
    filterPath = filterPath.replace('/','\\');
    helpFile = helpFile.replace('/','\\');
#endif
	setSetting("Path/atomicsEditor", atomicsEditor);
	setSetting("Path/parametersEditor", parametersEditor);
	setSetting("Path/atomicsPath", atomicsPath);
	setSetting("Path/filterPath", filterPath);
	setSetting("Path/helpFile", helpFile);
}
