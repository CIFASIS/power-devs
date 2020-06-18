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

#ifndef DLG_MENU_EDIT_H
#define DLG_MENU_EDIT_H

#include <ui_dlg_menu_edit.h>
#include <user_menu_action.h>

typedef enum {EDIT_DIALOG, ADD_DIALOG} diags;

class DlgMenuEdit: public QDialog,  public Ui::DlgMenuEditUI
{
  	Q_OBJECT
public:
  	DlgMenuEdit(diags,QString *, QList <QString *>, QList <UserMenuAction*> *);
  	DlgMenuEdit(diags,QString *, QList <QString *>*, QList <UserMenuAction*> *);
	~DlgMenuEdit();	
private slots:
	void on_menuEditLstWdt_currentRowChanged(int);
	void on_selectBtn_clicked();
	void on_removeBtn_clicked();
	void on_menuEditLed_textChanged(const QString &);
	void on_fileLed_textChanged(const QString &);
	void on_fileBtn_clicked();
	void on_paramMenuLed_textChanged(const QString &);
	void on_shortcutCbx_currentIndexChanged(const QString &);
	void on_needsOpenModel_stateChanged(int);
	void on_saveModelBefore_stateChanged(int);
	void on_addBtn_clicked();
	void on_removeItemBtn_clicked();
	void on_buttonBox_accepted();
	void on_menuLed_textChanged(const QString &);
private:
	void disableNewMenuButtons();
	bool checkMenuValues();
	QStringList shortcutLst;
	QStringList shortcutDisabled;
	int dialog;
	QString _menu;
	QStringList _menuItems;
	QList <UserMenuAction> _menuActions;
	QString *_menu_local;
	QList <QString *> _menuItems_local;
	QList <QString *> *_menuItems_local_ptr;
	QList <UserMenuAction*> *_menuActions_local;
	QPixmap iconPxm;
};
#endif
