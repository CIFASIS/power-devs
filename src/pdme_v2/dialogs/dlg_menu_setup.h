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

#ifndef DLG_MENU_SETUP_H
#define DLG_MENU_SETUP_H

#include <QSignalMapper>

#include <ui_dlg_menu_setup.h>
#include <user_menu_action.h>


class DlgMenuSetup: public QDialog,  public Ui::DlgMenuSetupUI
{
  	Q_OBJECT
public:
  	DlgMenuSetup();
signals:
	void updateMenus();
private slots:
	void on_buttonBox_accepted();
	void on_removeBtn_clicked();
	void on_editBtn_clicked();
	void on_addBtn_clicked();
private:
	QStringList menus;
	QList <QList <QString *>  > menuItems;
	QList <QList <UserMenuAction *> *> menuActions;
};
#endif
