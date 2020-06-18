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

#ifndef POWERGUI_H
#define POWERGUI_H

#include <QSignalMapper>
#include <QToolBox>
#include <QFileInfo>
#include <QClipboard>
#include <QUndoGroup>

#include <ui_main_window.h>
#include <user_menu_action.h>
#include <graphics/library_window.h>
#include <graphics/gpx_edition_window.h>

using namespace Ui;

class PowerGui: public QMainWindow,public PowerGui_class
{
  Q_OBJECT
public:
	PowerGui();
	~PowerGui();
  bool openFile(QString fileName);

signals:
  void runActionSignal(QObject*);

private slots:
  void runAction(QObject*); 
  void updateMenus();

  void on_actionNew_triggered();
  void on_actionOpen_triggered();
  void on_actionSave_triggered();
  void on_actionSave_as_triggered();
  void on_actionPrint_triggered();
  void openRecentFile();
  
  void on_actionDelete_triggered();
  void on_actionEdit_triggered();
  void on_actionRotate_triggered();
  void on_actionOpen_2_triggered();
  void on_actionSelect_All_triggered();
  void on_actionCopy_triggered();
  void on_actionCut_triggered();
  void on_actionPaste_triggered();

  void on_actionAdd_Remove_library_triggered();
  void on_actionSet_path_tool_triggered();
  void on_actionMenu_setup_triggered();

  void on_actionAbout_triggered();
  void on_actionAddanotation_triggered();
  void on_actionConverttoCoupled_triggered();
  void on_actionUserGuide_triggered();

  void on_actionClose_triggered();
  void dirtyModelClose(GpxEditionWindow *model,QCloseEvent *event);
  void on_actionClose_All_triggered();
  void on_actionModel_triggered();
  void on_actionModel_parameters_triggered();
  void on_actionTile_triggered();
  void on_actionCascade_triggered();
  void on_actionNext_triggered();
  void on_actionPrevious_triggered();
  void on_actionPriority_triggered();
  void setActiveSubWindow(QWidget *window);
  void on_actionParameters_triggered();
#ifdef UNDO
  void setActiveUndoStack();
#endif

  void contextMenu(GpxBlock *,QPoint);
  void parameterDialog(GpxBlock *);
  void parameterDialogCoupled(Coupled*);
  void closeEvent(QCloseEvent * event);
private:
//  void on_actionNew();
  bool loadLibrary();
  bool reloadLibrary();
  bool clearLibrary();
	bool loadUserMenus();
  bool hasActiveModel() { return mdiArea->activeSubWindow()!=NULL; }
  bool hasItemSelected();
  bool hasModelSelected();
  bool hasCoupledSelected();
  bool hasSelection(); 
  bool hasClipboard() { return !QApplication::clipboard()->text().isEmpty(); }
  bool saveFile(Coupled *c, const QString fileName);
  bool updateRecentFiles();
  bool createRecentFiles();
  bool createUndoActions();
  QMdiSubWindow *findModel(const QString &fileName);
  GpxEditionWindow *getActiveWindow(); 
  
private:
  QSignalMapper _userMenuMapper, *windowMapper;
  void updateWindowMenu();
  QList<QAction*> _userActions;
  LibraryWindow dockLib;
  enum { MaxRecentFiles = 5 };
  QAction *recentFileActs[MaxRecentFiles];
  QAction *exitAct;
  QAction *separator;
  QList<Coupled*> _loadedLibs;
  #ifdef UNDO
	  QUndoGroup *_undoGroup;
	  QAction *actionUndo;
	  QAction *actionRedo;
  #endif
};
#endif
