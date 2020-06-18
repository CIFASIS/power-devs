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

#include <QListWidgetItem>

#include <globals.h>
#include <dialogs/dlg_edit_list_parameter.h>

DlgEditListParameter::DlgEditListParameter(vector<string> &ls): _ls(ls)
{
	setupUi(this);
	vector<string>::iterator i=_ls.begin();
	for (;i!=ls.end();i++) {
		list->addItem(i->c_str());
	}
}

DlgEditListParameter::~DlgEditListParameter()
{
}

void DlgEditListParameter::on_list_itemDoubleClicked(QListWidgetItem * item )
{
	item->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled);
	list->editItem(item);
}

void DlgEditListParameter::on_remove_clicked()
{

  qDebug() << list->count();
  list->takeItem(list->currentRow());
  qDebug() << list->count();
}
void DlgEditListParameter::on_add_clicked()
{
  QListWidgetItem *item = new QListWidgetItem();
	item->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled);
	list->addItem(item);
	list->setCurrentItem(item);
	list->editItem(item);
}

void DlgEditListParameter::on_buttonBox_accepted()
{
  qDebug() << list->count();
  _ls.clear();
	for (int i=0;i<list->count();i++) {
  		if(list->item(i)->text() != "")
  			_ls.push_back(qPrintable(list->item(i)->text()));
  }

}
