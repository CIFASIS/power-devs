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

#ifndef DLG_EDIT_MODEL_H
#define DLG_EDIT_MODEL_H

#include <vector>
#include <QGraphicsView>
#include <QDoubleValidator>
#include <QFileSystemModel>
#include <QFileSystemWatcher>
#include <QTimer>

#include "parameter.h"
#include <ui_dlg_edit_model.h>
#include <graphics/gpx_block.h>
#include <graphics/library_scene.h>

class DlgEditModel: public QDialog,  public Ui::DlgEditModelUI
{
  Q_OBJECT
public:
  DlgEditModel(GpxBlock *b);
  DlgEditModel(Coupled *c);
  ~DlgEditModel();
private slots:
  void on_inputs_valueChanged ( int );
  void on_outputs_valueChanged ( int );
  void on_comboDirection_currentIndexChanged ( int index );
  void on_colorCombo_currentIndexChanged ( int index );
  void on_name_textChanged ( const QString & text );
  void on_nameParam_textChanged ( const QString & text );
  void on_descParam_textChanged ( );
  void on_valueParam_textChanged ( const QString &text);
  void on_iconPath_textChanged ( const QString & text );
  void on_findIcon_clicked(bool);
  void on_up_clicked();
  void on_down_clicked();
  void on_editList_clicked();
  void on_deleteParam_clicked();
  void on_newParam_clicked();
  void on_newCode_clicked();
  void on_editCode_clicked();
  void on_typeParam_currentIndexChanged(int);
  void on_parameters_itemSelectionChanged();
  void on_valueList_currentIndexChanged(int index);
  void on_directory_changed( QTreeWidgetItem * current, QTreeWidgetItem * previous );
  void on_dirs_currentItemChanged ( QTreeWidgetItem * current, QTreeWidgetItem * previous ) ;
  void on_files_itemSelectionChanged();
  void timer_tick();
  void on_buttonBox_clicked ( QAbstractButton * button );
private:
  QTimer timer;
  void setRow(int row, const QList<QTableWidgetItem*>& rowItems);
  void init_dirs(QString);
  void update_files(QString = "");
  void enableParamValues(bool enable);
  QList<QTableWidgetItem*> takeRow(int row);
  QIcon createColorIcon(QColor color);
  LibraryScene *prev;
  QGraphicsView *prevView;
	vector< Parameter* > _parameters;
	QDoubleValidator *validator;
  GpxBlock *_b;
	Coupled *_c;
};
#endif
