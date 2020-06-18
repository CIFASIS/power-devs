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

#ifndef DLG_SET_PATH_TOOLS_DIALOG_H
#define DLG_SET_PATH_TOOLS_DIALOG_H

#include <ui_dlg_set_path_tools.h>

class DlgSetPathTools: public QDialog,  public Ui::DlgSetPathToolsUI
{
  	Q_OBJECT
public:
  	DlgSetPathTools();
private slots:
	void on_helpBtn_clicked();
	void on_pathBtn_clicked();
	void on_editorBtn_clicked();
	void on_buttonBox_accepted();
};
#endif
