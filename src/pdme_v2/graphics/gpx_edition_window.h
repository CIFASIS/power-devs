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

#ifndef GPXEDITIONWINDOW_H
#define GPXEDITIONWINDOW_H

#include <QTabWidget>
#include <QMap>
#include <QDebug>
#include <QGraphicsView>
#ifdef UNDO
	#include <QUndoGroup>
#endif
#include <graphics/gpx_block.h>
#include <graphics/gpx_edition_scene.h>
#include <data/coupled.h>

class GpxEditionWindow:public QTabWidget
{
  Q_OBJECT
public:
	GpxEditionWindow(QWidget *parent,Coupled *c);
	GpxEditionWindow(QWidget *parent);
	#ifdef UNDO
		GpxEditionWindow(QWidget *parent,Coupled *c, QUndoGroup *ug);
		GpxEditionWindow(QWidget *parent, QUndoGroup *ug);
	#endif	
	~GpxEditionWindow();
	bool isDirty() { return isWindowModified(); }
  	void setDirty() { setWindowModified(true); }
    void addAnnotation();
    void convertToCoupled();
    void addAnnotation(QPoint);
  	void clearDirty() { setWindowModified(false); }
	  QString fileName() { return _fileName;}
  	void setFileName(QString fileName);
  	Coupled *getCoupled() { _coupledData->updatePoints(); return _coupledData; }
  	GpxEditionScene *currentScene();
  	QString getSelection();
  	Coupled *getCurrentCoupledData();
  	GpxBlock *getCurrentItem();
  	void paste(QString);
  	bool hasItemSelected();
  	bool hasModelSelected();
  	bool hasCoupledSelected();
  	bool hasSelection();
  	void rotate();
  	void deleteSelection();
  	void selectAll();
  	void openCoupled();
#ifdef UNDO
	void setActiveUndoStack();
#endif
signals:
	void saveModel(GpxEditionWindow *,QCloseEvent *);
  	void selectionChanged();
  	void contextMenu(GpxBlock *, QPoint);
  	void parameterDialog(GpxBlock *);
  	void parameterDialogCoupled(Coupled*);

protected:
	void closeEvent(QCloseEvent * event);
private slots:
  	void closeAllChildsOf(Coupled *);
  	void on_tabCloseRequested(int);
  	void on_sceneSelectionChanged();
  	void on_contextMenu(GpxBlock*,QPoint);
  	void on_parameterDialog(GpxBlock*);
  	void on_remove_Port(Coupled *, Port *);
  	void on_add_Port(Coupled *, Port *);
  	void on_modified();
  	void on_parameter(Coupled *c);
  	void on_setBlockName(GpxBlock*);
  	#ifdef UNDO
		void on_currentChanged(int);
	#endif
private:
  	void openCoupled(Coupled *c);
  	QString completePathName(Coupled *c);
	bool _dirty;
  	QString _fileName;
  	Coupled *_coupledData;
  	QTabWidget _tabs;
  	QMap<Coupled*, unsigned int> _openIn;
	#ifdef UNDO
  		QUndoGroup *_undoGroup;
	#endif
};
#endif
