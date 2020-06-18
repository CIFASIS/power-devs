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

#include <dialogs/dlg_change_priority.h>
#include <QDebug>

DlgChangePriority::DlgChangePriority(Coupled *c): _coupledData(c)
{
  setupUi(this);
  for (int chld=0; chld<c->childCount();chld++) {
    _child_list->addItem(c->child(chld)->name().c_str());
  }
  if (_child_list->count())
    _child_list->setCurrentRow(0);
  _temp_order = c->getChildList();
  connect(this,SIGNAL(accepted()),SLOT(on_accept()));
}

void DlgChangePriority::on__down_clicked()
{
  int row = _child_list->currentRow(); 
 if (row < 0 || row>=_child_list->count()-1)
  return;
 QListWidgetItem *i = _child_list->takeItem (row);
 _child_list->insertItem(row+1,i);
 _child_list->setCurrentRow(row+1);
 Model *t = _temp_order.at(row+1);
 _temp_order.at(row+1)= _temp_order.at(row);
 _temp_order.at(row)= t;
}

void DlgChangePriority::on__up_clicked()
{
 int row = _child_list->currentRow(); 
 if (row < 1 || row>_child_list->count())
  return;
 QListWidgetItem *i = _child_list->takeItem (row);
 _child_list->insertItem(row-1,i);
 _child_list->setCurrentRow(row-1);
 Model *t = _temp_order.at(row-1);
 _temp_order.at(row-1)= _temp_order.at(row);
 _temp_order.at(row)= t;
}

void DlgChangePriority::on_accept()
{
  _coupledData->setChildList(_temp_order);
}
