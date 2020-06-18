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

#ifndef DLG_CHANGE_PARAMETERS_H
#define DLG_CHANGE_PARAMETERS_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QGroupBox>
#include <QPlainTextEdit>
#include <QVBoxLayout>
#include <QFormLayout>

#include <vector>
#include <data/model.h>
#include <graphics/gpx_block.h>
#include <data/parameter.h>


class DlgChangeParameters: public QDialog
{
  Q_OBJECT
public:
  	DlgChangeParameters(GpxBlock*);
  	DlgChangeParameters(Coupled*);

private slots:
  	void apply();

private:
  	bool fromBlock;
  	GpxBlock *_b;
  	Coupled *_c;
  	QPlainTextEdit *_desc;
  	QVBoxLayout *_desc_layout;
  	QVBoxLayout *_layout;
  	QVBoxLayout *_params_layout;
  	QGroupBox *_desc_group;
  	QGroupBox *_params_group;
  	QDialogButtonBox *_buttons;
	vector< Parameter* > _parameters;
	vector< QWidget* > _widgets;
};
#endif
