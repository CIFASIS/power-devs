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

#ifndef DLG_CHANGE_PRIORITY_H
#define DLG_CHANGE_PRIORITY_H

#include <vector>

#include <ui_dlg_change_priority.h>
#include <data/coupled.h>

class DlgChangePriority: public QDialog,  public Ui::DlgChangePriorityUI
{
  Q_OBJECT
public:
  DlgChangePriority(Coupled *c);

protected slots:
  void on__up_clicked();
  void on__down_clicked();
  void on_accept();

private:
  Coupled *_coupledData;
  std::vector<Model*> _temp_order;
};
#endif
