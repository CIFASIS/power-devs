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

#ifndef DLG_EDIT_LIST_PARAMETER_H
#define DLG_EDIT_LIST_PARAMETER_H

#include <vector>
#include <string>

#include "parameter.h"
#include <ui_dlg_edit_list_parameter.h>

class DlgEditListParameter: public QDialog,  public Ui::DlgEditListParameterUI
{
  Q_OBJECT
public:
  DlgEditListParameter(vector<string> &);
  ~DlgEditListParameter();
private slots:
	void on_list_itemDoubleClicked(QListWidgetItem * item );
	void on_add_clicked();
	void on_remove_clicked();
  void on_buttonBox_accepted();
private:
  vector<string> &_ls;
};
#endif
