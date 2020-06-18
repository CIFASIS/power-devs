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

#ifndef DLG_CHANGE_LIBRARY_H
#define DLG_CHANGE_LIBRARY_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QGroupBox>
#include <ui_dlg_change_library.h>

class DlgChangeLibrary: public QDialog, public Ui::DlgChangeLibraryUI
{
  Q_OBJECT
public:
  DlgChangeLibrary();
private slots:
	void on_buttonBox_accepted();
	void on_addBtn_clicked();
	void on_removeBtn_clicked();
	void on_upBtn_clicked();
	void on_downBtn_clicked();
	void on_lsLstWdt_currentRowChanged(int);
private:
	QStringList libLst;
	QStringList libEnabled;
};
#endif
