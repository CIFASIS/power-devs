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
#include <QSettings>
#include <QSignalMapper>
#include <QDebug>

#include <dialogs/dlg_menu_setup.h>
#include <dialogs/dlg_menu_edit.h>
#include <globals.h>

DlgMenuSetup::DlgMenuSetup()
{
	setupUi(this);

	menus = getSetting("userMenus").toStringList();
  
	foreach(QString menuName,menus)
	{
		QStringList actionList = getSetting("menu/" + menuName).toStringList();
		QList <QString*> _actionList;
		foreach(QString action, actionList){
			QString *_action = new QString(action);
			_actionList << _action;
		}
		menuItems << _actionList;
		QList <UserMenuAction*> *actions = new QList <UserMenuAction*>();
		foreach (QString actionName, actionList)
		{
			QStringList userMenuData = getSetting("menu/" + menuName + "/" + actionName).toStringList();
			UserMenuAction *userAction = new UserMenuAction(userMenuData);
			actions->append(userAction);
		}
		menuActions << actions;
	}
	menuLstWdt->addItems(menus);
	menuLstWdt->setCurrentRow(0);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

void DlgMenuSetup::on_buttonBox_accepted()
{
	QStringList userMenus = getSetting("userMenus").toStringList();
 
	foreach(QString menuName, userMenus)
	{
		removeSetting(menuName);
	}
	setSetting("userMenus",menus);
	
 	int i = 0;
	foreach(QString menuName, menus)
	{
		QList <QString*> items = menuItems[i];
		QStringList add;
		foreach(QString *item, items)
			add << *item;
		setSetting("menu/" + menuName,add); 
		QList <UserMenuAction*> *actions = menuActions[i];
		int j = 0;	
		foreach(QString *menuItem, items){
			setSetting("menu/"+ menuName + "/" + *menuItem,actions->at(j)->getData());
			j++;
		}
		i++;
	}
}

void DlgMenuSetup::on_removeBtn_clicked()
{
	menus.removeAt(menuLstWdt->currentRow());
	menuItems.removeAt(menuLstWdt->currentRow());
	menuActions.removeAt(menuLstWdt->currentRow());
	QListWidgetItem *item = menuLstWdt->currentItem();
	menuLstWdt->removeItemWidget(item);
	delete item;
}

void DlgMenuSetup::on_editBtn_clicked()
{
	int row = menuLstWdt->currentRow();
	QString *menu = new QString(menus[row]);
	DlgMenuEdit me(EDIT_DIALOG,menu, menuItems[row], menuActions[row]);
	if(me.exec()){
		menus[row] = *menu;
		menuLstWdt->currentItem()->setText(*menu);
	}
}

void DlgMenuSetup::on_addBtn_clicked()
{
	QString *newMenu = new QString();
	QList <QString*> *newMenuItems = new QList <QString*>();
	QList <UserMenuAction *> *newActions = new QList <UserMenuAction*>();
	DlgMenuEdit me(ADD_DIALOG,newMenu, newMenuItems, newActions);
	if(me.exec()){
		menus << *newMenu;
		menuLstWdt->addItem(*newMenu);
		menuItems << *newMenuItems;
		menuActions << newActions;
	}
}
