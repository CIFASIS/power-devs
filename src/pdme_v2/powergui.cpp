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

#include <QToolBox>
#include <QGraphicsView>
#include <QStringList>
#include <QMenu>
#include <QDebug>
#include <QFileDialog>
#include <QMdiSubWindow>
#include <QMessageBox>
#include <QStatusBar>
#include <QCloseEvent>
#include <QProcess>
#include <QFile>
#include <QFileInfo>
#include <QClipboard>
#include <QImage>
#include <QDesktopServices>
#include <QUrl>
#include <QSplashScreen>
#include <QSettings>



#include <powergui.h>
#include <commands.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <globals.h>
#include <graphics/gpx_edition_window.h>
#include <graphics/library_window.h>
#include <graphics/library_scene.h>
#include <graphics/gpx_block.h>
#include <graphics/gpx_atomic.h>
#include <graphics/gpx_coupled.h>
#include <data/coupled.h>

#include <dialogs/dlg_change_priority.h>
#include <dialogs/dlg_change_parameters.h>
#include <dialogs/dlg_edit_model.h>
#include <dialogs/dlg_change_library.h>
#include <dialogs/dlg_set_path_tools.h>
#include <dialogs/dlg_menu_setup.h>
#include <dialogs/dlg_menu_setup.h>


#ifndef Q_OS_WIN32
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/types.h>          
#include <sys/socket.h>
#include <unistd.h>
#endif

        
extern QSplashScreen *splash;
QPoint mouseContext;
PowerGui::PowerGui(): PowerGui_class()
{
	setupUi(this);
	
	windowMapper = new QSignalMapper(this);
  splash->showMessage("Loading user menu...",Qt::AlignLeft | Qt::AlignBottom);
	loadUserMenus();

  splash->showMessage("Loading libraries...",Qt::AlignLeft | Qt::AlignBottom);
  loadLibrary();
	addDockWidget(Qt::LeftDockWidgetArea, &dockLib);
  
  createRecentFiles();
  updateRecentFiles();
  #ifdef UNDO
  	createUndoActions();
  #endif
  updateMenus();

  connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)), this, SLOT(updateMenus()));
  #ifdef UNDO
  connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)), this, SLOT(setActiveUndoStack()));
  #endif
  connect(windowMapper, SIGNAL(mapped(QWidget*)), this, SLOT(setActiveSubWindow(QWidget*)));
  QString scilabPath = getSetting("scilabPath").toString(); 
#ifdef Q_OS_WIN32
   QFileInfo qf(scilabPath);
   if (!qf.exists()) {
   	QSettings scilab( "HKEY_LOCAL_MACHINE\\SOFTWARE\\Scilab", QSettings::NativeFormat);
	   QStringList sl = scilab.allKeys();
	   bool found=false;
	   foreach(QString s, sl) {
		if (s.contains("SCIPATH")) {
			QString path = scilab.value(s).toString() + "\\bin\\Wscilex.exe";
			QFileInfo qf(path);
			if (qf.exists()) {
				scilabPath = path;
				found=true;
				setSetting("scilabPath", path);
			}
		}
	   }
	   if (!found) {
		QMessageBox::about(NULL, "Scilab Error", "PowerDEVS was unable to find a Scilab installation. Some functionality may not work. Please install Scilab 5.2 or higher.");
	  }
   }
#endif
  bool startScilab = getSetting("startScilab").toBool(); 
  if (startScilab) { 
    splash->showMessage("Starting Scilab...",Qt::AlignLeft | Qt::AlignBottom);
    QString outputPath = getSetting("Path/outputPath").toString(); 
    qDebug() << "Starting Scilab from " << scilabPath << " in directory " << outputPath << " with arg " << getSetting("scilabArg").toString();
#ifndef Q_OS_WIN32

    bool scilabRunning = system("ps -e | grep scilab") == 0;
    if (scilabRunning) {
      struct sockaddr_in service;
      int sock=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
      service.sin_family = AF_INET;
      service.sin_addr.s_addr = inet_addr("127.0.0.1");
      int actual_port = 27020+(getuid() % 10000);
      service.sin_port = htons(actual_port);
      if (::connect(sock,(struct sockaddr*)&service,sizeof(service)) == -1) {
	      scilabRunning=false;
      }
      ::close(sock);
    }
 

    
#else
    bool scilabRunning = false;
#endif
    if (!scilabRunning) {
      if (getSetting("scilabArg").toString().trimmed().size())
        QProcess::startDetached(scilabPath, QStringList() << "-f" << getSetting("scilabArg").toString().replace("$HOMEAPP",QCoreApplication::applicationDirPath()), outputPath); 
      else
        QProcess::startDetached(scilabPath, QStringList(), outputPath); 
    } else {
      qDebug() << "Scilab is already running";
    }
  }
}

PowerGui::~PowerGui()
{
  
  setSetting("LibraryWidth",dockLib.widget()->width());
  
  foreach(QAction *a,_userActions) 
    delete (UserMenuAction*) _userMenuMapper.mapping(a);
  foreach(Coupled *c,_loadedLibs) 
    delete c;
}

bool PowerGui::loadUserMenus()
{
	QStringList menus = getSetting("userMenus").toStringList();
 	_userActions.clear(); 
	foreach(QString menuName,menus)
	{
		QMenu *menu = new QMenu(menuName);
		QStringList actionList = getSetting("menu/" + menuName).toStringList();
		foreach (QString actionName, actionList)
		{
			QAction *actionUserMenu =	menu->addAction(actionName);
			QStringList userMenuData = getSetting("menu/" + menuName + "/" + actionName).toStringList();
			UserMenuAction *userAction = new UserMenuAction(userMenuData);
			if (userAction->hasShortcut()) 
				actionUserMenu->setShortcut(userAction->shortcut());
			if (userAction->hasIcon()) {
				actionUserMenu->setIcon(QIcon(userAction->icon()));
        toolBar->addAction(actionUserMenu);
			}
      _userMenuMapper.setMapping(actionUserMenu,userAction);
      connect(actionUserMenu,SIGNAL(triggered()),&_userMenuMapper,SLOT(map()));
      actionUserMenu->setData(userMenuData);
      _userActions << actionUserMenu;
		}
		menuBar()->insertMenu(menuWindow->menuAction(),menu);
	}
  connect(&_userMenuMapper,SIGNAL(mapped(QObject *)),this,SIGNAL(runActionSignal(QObject*)));
  connect(this,SIGNAL(runActionSignal(QObject *)),this,SLOT(runAction(QObject*)));
	return true;
}

void PowerGui::runAction(QObject *objectAction)
{
    bool active=hasActiveModel();
    GpxEditionWindow *model;
    UserMenuAction *userAction = (UserMenuAction*)objectAction;
    if (active)
      model =  getActiveWindow(); 
    if (active && userAction->requiresSave()) {
      on_actionSave_triggered();
      if (model->fileName().isEmpty())
        return;
    }
    QString arg = userAction->args();
    QStringList args = arg.split(" ");
    if (active && userAction->requiresModel()) 
    {
      QString fileName = model->fileName();
      if (fileName.isEmpty())
        return;
       qDebug() << args;
       args.replaceInStrings("\%F",fileName);
       args.replaceInStrings("\%D",fileName.replace(".pdm",".pdd"));
    }
    if (arg.contains("\%P")) {
      QString examplePath = QCoreApplication::applicationDirPath() + "/" + getSetting("Path/examplePath").toString();
      QString fileName = QFileDialog::getOpenFileName(this, "Open structure", examplePath, "PowerDEVS Structure *.pds (*.pds)");
      if (fileName.isEmpty()) 
        return;
      args.replaceInStrings("\%P",fileName);
    }
    qDebug() << "Pressed on " << userAction->command() << " " << args << "Running it on " << userAction->runInFolder();
    if (userAction->runInFolder().isEmpty())
      QProcess::startDetached(userAction->command(),args);
    else
      QProcess::startDetached(userAction->command(),args,userAction->runInFolder());
}

void PowerGui::updateMenus()
{
  bool active=hasActiveModel();
  bool item=hasItemSelected();
  bool selection=hasSelection();
  bool model_selected=hasModelSelected();
  bool has_coupled=hasCoupledSelected();


  // File menu
  actionSave->setEnabled(active);
  actionSave_as->setEnabled(active);
	actionPrint->setEnabled(active);

  // Edit menu
  actionCopy->setEnabled(selection);
  actionCut->setEnabled(selection);
  actionPaste->setEnabled(hasClipboard() && active);
  actionSelect_All->setEnabled(active);
  actionDelete->setEnabled(selection);
  actionRotate->setEnabled(item);
  actionPriority->setEnabled(active);
  actionEdit->setEnabled(model_selected);
  actionOpen_2->setEnabled(has_coupled);
  actionModel->setEnabled(active);
  actionAddanotation->setEnabled(active);
  #ifdef UNDO
  	actionUndo->setEnabled(active);
  	actionRedo->setEnabled(active);
  #endif
  actionConverttoCoupled->setEnabled(selection);

  // View menu
  actionParameters->setEnabled(model_selected);
  actionModel_parameters->setEnabled(active);

  // User actions 
  foreach(QAction *action, _userActions)
  {
    UserMenuAction userAction(action->data().toStringList());
    if (userAction.requiresModel() && !active)
      action->setEnabled(false);
    else
      action->setEnabled(true);
  }

  // Window menu
  actionClose->setEnabled(active);
  actionClose_All->setEnabled(active);
  actionTile->setEnabled(active);
  actionCascade->setEnabled(active);
  actionNext->setEnabled(active);
  actionPrevious->setEnabled(active);
  updateWindowMenu();
}

void PowerGui::on_actionNew_triggered()
{
  #ifdef UNDO
  	GpxEditionWindow *newModel = new GpxEditionWindow(this,_undoGroup);  
  #else
  	GpxEditionWindow *newModel = new GpxEditionWindow(this);  
  #endif
  mdiArea->addSubWindow(newModel);
  newModel->show();
  newModel->setWindowState(Qt::WindowMaximized);
  /*******************************************************************************************************************************/
  connect(newModel,SIGNAL(saveModel(GpxEditionWindow *,QCloseEvent*) ),SLOT(dirtyModelClose(GpxEditionWindow *,QCloseEvent *)));
  connect(newModel,SIGNAL(selectionChanged()),SLOT(updateMenus()));
  connect(newModel,SIGNAL(currentChanged(int)),SLOT(updateMenus()));
  connect(newModel,SIGNAL(contextMenu(GpxBlock*,QPoint)),SLOT(contextMenu(GpxBlock*,QPoint)));
  connect(newModel,SIGNAL(parameterDialog(GpxBlock*)),SLOT(parameterDialog(GpxBlock*)));
  connect(newModel,SIGNAL(parameterDialogCoupled(Coupled *)),SLOT(parameterDialogCoupled(Coupled *)));
  /*******************************************************************************************************************************/
  updateWindowMenu();
}

bool PowerGui::loadLibrary()
{
  QToolBox *toolBox = new QToolBox();
	dockLib.setWindowTitle(tr("Library"));
	dockLib.setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

	QString pathLib = getSetting("Path/libPath").toString();
	QStringList libs = getSetting("Libraries").toStringList();
  qDebug() << "Loading from " << pathLib << " " << libs;
	QStringList libsEnabled = getSetting("libraryEnabled").toStringList();
  //Q_ASSERT(libs.size()==libsEnabled.size());
  int libCount = 0;
	_loadedLibs.clear();
  Coupled *coupled_lib;

	foreach(QString fileLib, libs) {
    //if (!libsEnabled.at(libCount++).toInt()==0)
    if (libsEnabled.at(libCount++)!="true") 
      continue;
    qDebug() << "Loading lib " << fileLib;  
    coupled_lib = parseFile(fileLib);
    if (coupled_lib==NULL)
    {
      QMessageBox::critical(this,"PowerDEVS",QString("The library %1 could not be loaded.").arg(fileLib));
      continue;
    }
    _loadedLibs << coupled_lib;
    LibraryScene *scene_lib = new LibraryScene(this,coupled_lib);
		QGraphicsView *view = new QGraphicsView(scene_lib,this);
    view->setRenderHints(QPainter::Antialiasing);
    view->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->resize(getSetting("LibraryWidth").toInt(),view->height());
		toolBox->addItem(view, QIcon(getImage(coupled_lib->graphic().icon().c_str())), coupled_lib->name().c_str());
  }
	dockLib.setWidget(toolBox);
  return true;
}

bool PowerGui::clearLibrary()
{
  QToolBox *toolBox = qobject_cast<QToolBox*>(dockLib.widget());
  while(toolBox->count()) { // Clear the toolBox
    QGraphicsView *view = qobject_cast<QGraphicsView*>(toolBox->widget(0));
    if (view==NULL)
      return false;
    toolBox->removeItem(0);
    delete view;
  }
  return true;
}

bool PowerGui::reloadLibrary()
{
  return clearLibrary() && loadLibrary();
}

void PowerGui::on_actionClose_triggered()
{
  mdiArea->closeActiveSubWindow();
}

void PowerGui::on_actionClose_All_triggered()
{
  mdiArea->closeAllSubWindows();
}

bool PowerGui::updateRecentFiles()
{
  QStringList files = getSetting("recentFileList").toStringList();
  int numRecentFiles = qMin(files.size(), (int)MaxRecentFiles);
  for (int i = 0; i < numRecentFiles; ++i) {
    QFileInfo fi(files[i]);
    QString text = tr("&%1 %2").arg(i + 1).arg(fi.completeBaseName());
    recentFileActs[i]->setText(text);
    recentFileActs[i]->setData(files[i]);
    recentFileActs[i]->setVisible(true);
   }
   for (int j = numRecentFiles; j < MaxRecentFiles; ++j)
    recentFileActs[j]->setVisible(false);
   separator->setVisible(numRecentFiles > 0);
   return true;
}

#ifdef UNDO
	bool PowerGui::createUndoActions()
	{
	  _undoGroup = new QUndoGroup(this);
	  actionUndo = _undoGroup->createUndoAction(this);
	  actionUndo->setObjectName(QString::fromUtf8("actionUndo"));
	  QIcon undoIcon;
	  undoIcon.addFile(QString::fromUtf8(":/images/undo.png"), QSize(), QIcon::Normal, QIcon::On);
	  actionUndo->setIcon(undoIcon);
	  actionRedo = _undoGroup->createRedoAction(this);
	  actionNew->setObjectName(QString::fromUtf8("actionRedo"));
	  QIcon redoIcon;
	  redoIcon.addFile(QString::fromUtf8(":/images/redo.png"), QSize(), QIcon::Normal, QIcon::On);
	  actionRedo->setIcon(redoIcon);
	  toolBar->addAction(actionUndo);
	  toolBar->addAction(actionRedo);
	  menuEdit->addSeparator();
	  menuEdit->addAction(actionUndo);
	  menuEdit->addAction(actionRedo);
	  connect(actionUndo,SIGNAL(triggered()), _undoGroup, SLOT(undo()));
	  connect(actionRedo,SIGNAL(triggered()), _undoGroup, SLOT(redo()));
	  return true;
	}
#endif

bool PowerGui::createRecentFiles()
{
  for (int i = 0; i < MaxRecentFiles; ++i) {
    recentFileActs[i] = new QAction(this);
    recentFileActs[i]->setVisible(false);
    connect(recentFileActs[i], SIGNAL(triggered()), this, SLOT(openRecentFile()));
  }

 exitAct = new QAction(tr("E&xit"), this);
 //exitAct->setShortcuts(QKeySequence::Quit);
 exitAct->setStatusTip(tr("Exit the application"));
 separator = menuFile->addSeparator();
  for (int i = 0; i < MaxRecentFiles; ++i)
    menuFile->addAction(recentFileActs[i]);
 menuFile->addSeparator();
 menuFile->addAction(exitAct);
 connect(exitAct, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));
 return true;
}

void PowerGui::openRecentFile()
{
  QAction *action = qobject_cast<QAction *>(sender());
  if (action) {
    QStringList files = getSetting("recentFileList").toStringList();
    QString fileName = action->data().toString();
    files.move(files.indexOf(fileName),0);
    setSetting("recentFileList",files);
    updateRecentFiles();
    openFile(fileName);
  }
}

bool PowerGui::openFile(QString fileName)
{
  QMdiSubWindow *existingModel = findModel(fileName);
  if (existingModel) {
    mdiArea->setActiveSubWindow(existingModel);
		existingModel->widget()->setWindowState(Qt::WindowMaximized);
		return false;
	}
  Coupled *c = parseFile(fileName);
  if (c==NULL) {
      QMessageBox::critical(this,"PowerDEVS","File does not exists or is corrupted");
      return false;
  } 
  #ifdef UNDO
  	GpxEditionWindow *newModel= new GpxEditionWindow(this,c,_undoGroup);  
  #else
  	GpxEditionWindow *newModel= new GpxEditionWindow(this,c);  
  #endif
  newModel->setFileName(fileName);
  newModel->clearDirty();
  mdiArea->addSubWindow(newModel);
  newModel->show();
  newModel->setWindowState(Qt::WindowMaximized);
  /*******************************************************************************************************************************/
  connect(newModel,SIGNAL(saveModel(GpxEditionWindow *,QCloseEvent*) ),SLOT(dirtyModelClose(GpxEditionWindow *,QCloseEvent *)));
  connect(newModel,SIGNAL(selectionChanged()),SLOT(updateMenus()));
  connect(newModel,SIGNAL(currentChanged(int)),SLOT(updateMenus()));
  connect(newModel,SIGNAL(contextMenu(GpxBlock*,QPoint)),SLOT(contextMenu(GpxBlock*,QPoint)));
  connect(newModel,SIGNAL(parameterDialog(GpxBlock*)),SLOT(parameterDialog(GpxBlock*)));
  connect(newModel,SIGNAL(parameterDialogCoupled(Coupled *)),SLOT(parameterDialogCoupled(Coupled *)));
  /*******************************************************************************************************************************/
  QStringList files = getSetting("recentFileList").toStringList();
  if (files.indexOf(fileName)<0)
  {
    files.prepend(fileName);
  } else {
    files.move(files.indexOf(fileName),0);
  }
  if (fileName.endsWith(".pdm"))
    setSetting("recentFileList",files);
  updateRecentFiles();
  updateWindowMenu();
  return true;
}

void PowerGui::on_actionOpen_triggered()
{
  QString examplePath = QCoreApplication::applicationDirPath() + "/" + getSetting("Path/examplePath").toString();
	QString fileName = QFileDialog::getOpenFileName(this, "Open model", examplePath, "PowerDEVS Model *.pdm (*.pdm);;PowerDEVS Library .pdl (*.pdl)");
	if (!fileName.isEmpty()) 
    openFile(fileName);
}

QMdiSubWindow *PowerGui::findModel(const QString &fileName)
{
  QString canonicalFilePath = QFileInfo(fileName).canonicalFilePath();
  foreach (QMdiSubWindow *window, mdiArea->subWindowList()) {
    GpxEditionWindow *mdiChild = qobject_cast<GpxEditionWindow *>(window->widget());
    Q_ASSERT(mdiChild);
    if (mdiChild->fileName() == canonicalFilePath)
      return window;
  }
  return NULL;
}

void PowerGui::on_actionTile_triggered()
{
  mdiArea->tileSubWindows();
}

void PowerGui::on_actionNext_triggered()
{
  mdiArea->activateNextSubWindow();
}

void PowerGui::on_actionPrevious_triggered()
{
  mdiArea->activatePreviousSubWindow();
}

void PowerGui::on_actionCascade_triggered()
{
  mdiArea->cascadeSubWindows();
}

void PowerGui::on_actionSave_triggered()
{
  GpxEditionWindow *model =  getActiveWindow(); 
  Q_ASSERT(model!=NULL);
  if ( (!model->fileName().isEmpty()) && model->isDirty()) {
    saveFile(model->getCoupled(),model->fileName());
    model->clearDirty();
  }
  if (model->fileName().isEmpty())
  {
    on_actionSave_as_triggered();
  }
}

void PowerGui::on_actionSave_as_triggered()
{
  GpxEditionWindow *model =  getActiveWindow(); 
  QString name = model->getCoupled()->name().c_str();
  QString examplePath = QCoreApplication::applicationDirPath() + "/" + getSetting("Path/examplePath").toString() + "/" + name + "1.pdm";
  QFileDialog d;
  d.setDefaultSuffix("pdm");
	QString fileName = d.getSaveFileName(this, "Save model", examplePath, "PowerDEVS Model *.pdm (*.pdm);;PowerDEVS Library *.pdl (*.pdl)");
  qDebug() << d.defaultSuffix() << " name " << fileName << endl;

  Q_ASSERT(model!=NULL);

	if (!fileName.isEmpty()) {
    QFileInfo fi(fileName);
    model->getCoupled()->setName(qPrintable(fi.baseName()));
    model->setFileName(fileName);
    saveFile(model->getCoupled(),model->fileName());
    model->clearDirty();
  }
}

GpxEditionWindow *PowerGui::getActiveWindow()
{ 
  if (mdiArea->activeSubWindow()->widget())
    return qobject_cast<GpxEditionWindow*>(mdiArea->activeSubWindow()->widget());
  return NULL;
}

bool PowerGui::saveFile(Coupled *c, const QString fileName)
{
  qDebug() << "Saving file " << fileName;
  c->updatePoints();
  ofstream stream(fileName.toLatin1(),std::ofstream::out);
  ::depth=0;
  stream << c[0];
  /*
  QFile f(fileName);
  f.open(QIODevice::WriteOnly);
  f.write(stream.str().c_str());
  f.close();*/
  stream.close();
  QStringList files = getSetting("recentFileList").toStringList();
  if (files.indexOf(fileName)<0)
  {
    files.prepend(fileName);
  } else {
    files.move(files.indexOf(fileName),0);
  }
  if (fileName.endsWith(".pdm"))
    setSetting("recentFileList",files);
  updateRecentFiles();
  statusBar()->showMessage(tr("File saved"), 2000);
	return true;
}

void PowerGui::setActiveSubWindow(QWidget *window)
{
  if (!window)
    return;
  mdiArea->setActiveSubWindow(qobject_cast<QMdiSubWindow *>(window));
}

#ifdef UNDO
	void PowerGui::setActiveUndoStack()
	{
	  if(mdiArea->activeSubWindow()){
	  	GpxEditionWindow *gew = qobject_cast<GpxEditionWindow*>(mdiArea->activeSubWindow()->widget());
	  	gew->setActiveUndoStack();
	  }
	}
#endif
void PowerGui::dirtyModelClose(GpxEditionWindow *model, QCloseEvent *event)
{
	QMessageBox msgBox;
	msgBox.setWindowTitle("Save file");
	msgBox.setText(QString("Save changes to %1").arg(model->windowTitle()));
	msgBox.addButton(QMessageBox::Yes);
	msgBox.addButton(QMessageBox::No);
	msgBox.addButton(QMessageBox::Cancel);
	int res = msgBox.exec();
  if (res==QMessageBox::No) {
    model->clearDirty();
    model->close();
  }
  else if (res==QMessageBox::Cancel) {
    return;
  }
  else if (!model->fileName().isEmpty())
    saveFile(model->getCoupled(),model->fileName());
  else {
    QMdiSubWindow *window = qobject_cast<QMdiSubWindow*>(model->parent());
	  mdiArea->setActiveSubWindow(window);
    on_actionSave_as_triggered();
  }
	event->accept();
}

bool PowerGui::hasItemSelected() { 
  if (!hasActiveModel())
    return false;
  GpxEditionWindow * gep = qobject_cast<GpxEditionWindow*>(mdiArea->activeSubWindow()->widget());
  Q_ASSERT(gep!=NULL);
  return gep->hasItemSelected();
}

bool PowerGui::hasModelSelected() { 
  if (!hasActiveModel())
    return false;
  GpxEditionWindow * gep = qobject_cast<GpxEditionWindow*>(mdiArea->activeSubWindow()->widget());
  Q_ASSERT(gep!=NULL);
  return gep->hasModelSelected();
}

bool PowerGui::hasCoupledSelected() { 
  if (!hasActiveModel())
    return false;
  GpxEditionWindow * gep = qobject_cast<GpxEditionWindow*>(mdiArea->activeSubWindow()->widget());
  Q_ASSERT(gep!=NULL);
  return gep->hasModelSelected();
}



bool PowerGui::hasSelection() 
{
  if (!hasActiveModel())
    return false;
  GpxEditionWindow * gep = qobject_cast<GpxEditionWindow*>(mdiArea->activeSubWindow()->widget());
  Q_ASSERT(gep!=NULL);
  return gep->hasSelection();
}

void PowerGui::on_actionRotate_triggered()
{
  if (!hasActiveModel())
    return ;
  GpxEditionWindow * gep = qobject_cast<GpxEditionWindow*>(mdiArea->activeSubWindow()->widget());
  Q_ASSERT(gep!=NULL);
  gep->rotate();
}

void PowerGui::on_actionDelete_triggered()
{
  if (!hasActiveModel())
    return ;
  GpxEditionWindow * gep = qobject_cast<GpxEditionWindow*>(mdiArea->activeSubWindow()->widget());
  Q_ASSERT(gep!=NULL);
  gep->deleteSelection();
}

void PowerGui::on_actionSelect_All_triggered()
{
  if (!hasActiveModel())
    return ;
  GpxEditionWindow * gep = qobject_cast<GpxEditionWindow*>(mdiArea->activeSubWindow()->widget());
  Q_ASSERT(gep!=NULL);
  gep->selectAll();
}

void PowerGui::contextMenu(GpxBlock *block,QPoint p)
{

  QMenu qMenu(NULL);
  qMenu.addAction(actionCut);
  qMenu.addAction(actionCopy);
  qMenu.addAction(actionPaste);
  qMenu.addAction(actionDelete);
  qMenu.addSeparator();

  qMenu.addAction(actionAddanotation);
  qMenu.addAction(actionParameters);
  qMenu.addAction(actionEdit);
  qMenu.addSeparator();
  qMenu.addAction(actionConverttoCoupled);
                    

  QAction *qActEditCode = NULL;
  /*
  editAtomicAction->setEnabled(true);
  parametersAction->setEnabled(true);
  block->setSelected(true);
  GpxAtomic *a;
  */
  if (block!=NULL) 
  {
    if (block->isAtomic()) {
      qActEditCode = qMenu.addAction("Edit Code");
    } else {
      qMenu.addAction(actionOpen_2);
    }
  }
  mouseContext = p;
  QAction *selected = qMenu.exec(p);
  // If the action cut is selected, the block is deleted.
  if(selected == qActEditCode){
	  if (block && block->isAtomic()) 
	  {
	    GpxAtomic *a = qgraphicsitem_cast<GpxAtomic*>(block);
	    QString path = getSetting("Path/atomicsPath").toString() + "/" + a->atomicData()->path().c_str();
		#ifdef Q_OS_LINUX
		    path = path.replace('\\','/');
		#else
    		path = path.replace('/','\\');
		#endif
    	if (strlen(a->atomicData()->path().c_str()))
   	   		QProcess::startDetached(getSetting("Path/atomicsEditor").toString(),QStringList() << path);
    	else
       		QMessageBox::warning(this,"PowerDEVS","The path has not been set");
  	}
  }
}

void PowerGui::on_actionPriority_triggered()
{
  if (!hasActiveModel())
    return;
  GpxEditionWindow * gep = qobject_cast<GpxEditionWindow*>(mdiArea->activeSubWindow()->widget());
  DlgChangePriority pd(gep->getCurrentCoupledData());
  if (pd.exec()) {
  	gep->setDirty();
  }
  connect(&pd,SIGNAL(finished(int)), this, SLOT(updateMenus()));
}

void PowerGui::on_actionParameters_triggered()
{
  if (!hasModelSelected())
    return;
  GpxEditionWindow * gep = qobject_cast<GpxEditionWindow*>(mdiArea->activeSubWindow()->widget());
  GpxBlock *b = gep->getCurrentItem();
  if (b->isAtomic() || b->isCoupled()) {
    DlgChangeParameters pd(b);
    if (pd.exec()) {
			gep->setDirty();
		}
  	connect(&pd,SIGNAL(finished(int)), this, SLOT(updateMenus()));
  }
}

void PowerGui::on_actionEdit_triggered()
{
  qDebug("Show edit dialog");
  if (!hasModelSelected())
    return;
  GpxEditionWindow * gep = qobject_cast<GpxEditionWindow*>(mdiArea->activeSubWindow()->widget());
  GpxBlock *b = gep->getCurrentItem();
  DlgEditModel pd(b);
  if (pd.exec()) {
		gep->setDirty();
	}
  connect(&pd,SIGNAL(finished(int)), this, SLOT(updateMenus()));
  qDebug("End");
}

void PowerGui::parameterDialog(GpxBlock *b)
{
  if (b->isAtomic() || b->isCoupled()) {
    DlgChangeParameters pd(b);
  	if (pd.exec()) {
  		if (hasActiveModel()) {
  			GpxEditionWindow * gep = qobject_cast<GpxEditionWindow*>(mdiArea->activeSubWindow()->widget());
				gep->setDirty();
			}
		}
  	connect(&pd,SIGNAL(finished(int)), this, SLOT(updateMenus()));
  }

}

void PowerGui::on_actionOpen_2_triggered()
{
  qDebug() << "Open coupled";
  if (!hasModelSelected())
    return;
  GpxEditionWindow * gep = qobject_cast<GpxEditionWindow*>(mdiArea->activeSubWindow()->widget());
  gep->openCoupled();
}

void PowerGui::parameterDialogCoupled(Coupled *c)
{
	DlgChangeParameters pd(c);
	GpxEditionWindow * gep = NULL;
  if (hasActiveModel())
    gep = qobject_cast<GpxEditionWindow*>(mdiArea->activeSubWindow()->widget());
  if (pd.exec()) {
  	if (hasActiveModel() || c->father()==NULL) {
      qDebug() << mdiArea->activeSubWindow();
			gep->setDirty();
		}
	}
  connect(&pd,SIGNAL(finished(int)), this, SLOT(updateMenus()));
}

void PowerGui::on_actionUserGuide_triggered() {
  QDesktopServices::openUrl(QUrl(QString("file://%1/../doc/PD_UserGuide.pdf").arg(QCoreApplication::applicationDirPath())));
}

void PowerGui::on_actionConverttoCoupled_triggered() {
  GpxEditionWindow * gep = qobject_cast<GpxEditionWindow*>(mdiArea->activeSubWindow()->widget());
  if (gep==NULL)
    return;
	gep->convertToCoupled();
}

void PowerGui::on_actionAddanotation_triggered() {
  GpxEditionWindow * gep = qobject_cast<GpxEditionWindow*>(mdiArea->activeSubWindow()->widget());
  if (gep==NULL)
    return;
  if (actionAddanotation->menu()!=menuEdit)
	  gep->addAnnotation(mouseContext);
  else
	  gep->addAnnotation();
}

void PowerGui::on_actionAbout_triggered() {
#include "authors.h"
  QFile qf(QCoreApplication::applicationDirPath() + "/../version");
  QString version("2.2");
  if (qf.exists()) {
    qf.open(QIODevice::ReadOnly);
    version = qf.readAll();
    qf.close();
  }
	QMessageBox::about(this, "About PowerDEVS",
			   QString("<h2>PowerDEVS %1</h2>"
			      "<p>Integrated Tool for Edition and Simulation "
			      "of Discrete Event Systems.</p>"
			      "<p>Developed by:</p> "
			      "%2"
			      "<p>Directed by:</p> "
			      "<p> Ernesto Kofman (kofman@fceia.unr.edu.ar) </p>").arg(version).arg(authors));
}

void PowerGui::updateWindowMenu()
{
	menuWindow->clear();
  menuWindow->addAction(actionClose);
  menuWindow->addAction(actionClose_All);
	menuWindow->addSeparator();
  menuWindow->addAction(actionTile);
  menuWindow->addAction(actionCascade);
	menuWindow->addSeparator();
  menuWindow->addAction(actionPrevious);
  menuWindow->addAction(actionNext);

  if (!hasActiveModel())
    return ;
  GpxEditionWindow * gep = qobject_cast<GpxEditionWindow*>(mdiArea->activeSubWindow()->widget());
	QList < QMdiSubWindow * >windows = mdiArea->subWindowList();
	if (!windows.isEmpty())
  {
	  menuWindow->addSeparator();
  }

	delete windowMapper;
	windowMapper = new QSignalMapper(this);
  connect(windowMapper, SIGNAL(mapped(QWidget*)), this, SLOT(setActiveSubWindow(QWidget*)));
	for (int i = 0; i < windows.size(); i++) {
		GpxEditionWindow *mw = qobject_cast <GpxEditionWindow * >(windows.at(i)->widget());
		QAction *action = menuWindow->addAction(mw->windowTitle());
		action->setCheckable(true);
		action->setChecked(mw == gep);
		connect(action, SIGNAL(triggered()), windowMapper, SLOT(map()));
		windowMapper->setMapping(action, windows.at(i));
	}
}

void PowerGui::closeEvent(QCloseEvent * event)
{
	foreach(QMdiSubWindow * mdisw, mdiArea->subWindowList()) {
		mdiArea->setActiveSubWindow(mdisw);
		if (!mdisw->close()) {
			event->ignore();
			return;
		}
	}
	event->accept();
}

void PowerGui::on_actionPaste_triggered()
{
  QClipboard *clipboard = QApplication::clipboard();
  //qDebug() << clipboard->text();
  if (!hasActiveModel())
    return;
  GpxEditionWindow * gep = qobject_cast<GpxEditionWindow*>(mdiArea->activeSubWindow()->widget());
  gep->paste(clipboard->text());
	gep->setDirty();
}

void PowerGui::on_actionCopy_triggered()
{
  if (!hasActiveModel())
    return;
  GpxEditionWindow *gep = qobject_cast<GpxEditionWindow*>(mdiArea->activeSubWindow()->widget());
  QClipboard *clipboard = QApplication::clipboard();
  clipboard->setText(gep->getSelection());
  updateMenus();
}

void PowerGui::on_actionCut_triggered()
{
  if (!hasActiveModel())
    return;
  GpxEditionWindow * gep = qobject_cast<GpxEditionWindow*>(mdiArea->activeSubWindow()->widget());
  QClipboard *clipboard = QApplication::clipboard();
  clipboard->setText(gep->getSelection());
  gep->deleteSelection();
	gep->setDirty();
  updateMenus();
}

void PowerGui::on_actionModel_triggered()
{
  if (!hasActiveModel())
    return;
  GpxEditionWindow * gep = qobject_cast<GpxEditionWindow*>(mdiArea->activeSubWindow()->widget());
  Coupled *c = gep->getCoupled();
  DlgEditModel pd(c);
 	if (pd.exec()) {
		gep->setDirty();
	}
  connect(&pd,SIGNAL(finished(int)), this, SLOT(updateMenus()));
}

void PowerGui::on_actionModel_parameters_triggered()
{
  if (!hasActiveModel())
    return;
  GpxEditionWindow * gep = qobject_cast<GpxEditionWindow*>(mdiArea->activeSubWindow()->widget());
	DlgChangeParameters pd(gep->getCoupled());
  if (pd.exec()) {
		gep->setDirty();
	}
  connect(&pd,SIGNAL(finished(int)), this, SLOT(updateMenus()));
}

void PowerGui::on_actionPrint_triggered()
{
  if (!hasActiveModel())
    return;
  GpxEditionWindow * gep = qobject_cast<GpxEditionWindow*>(mdiArea->activeSubWindow()->widget());
  QString examplePath = QCoreApplication::applicationDirPath() + "/" + getSetting("Path/examplePath").toString();
	QString fileName = QFileDialog::getSaveFileName(this, "Print model", examplePath, "Images (*.png *.jpg)");
	if (fileName.isEmpty())
		return;
  GpxEditionScene * ges = gep->currentScene();
  ges->clearSelection();
	QImage im(ges->sceneRect().width(),ges->sceneRect().height(),QImage::Format_RGB32);
	im.fill(0xffffff);
  QPainter painter(&im);
  ges->invalidate();
	ges->render(&painter);
	im.save(fileName);
}


void PowerGui::on_actionAdd_Remove_library_triggered()
{
  DlgChangeLibrary ld;
  if(ld.exec()){
  	reloadLibrary();
  }
  connect(&ld,SIGNAL(finished(int)), this, SLOT(updateMenus()));
}

void PowerGui::on_actionSet_path_tool_triggered()
{
	DlgSetPathTools spt;
	spt.exec();
  	connect(&spt,SIGNAL(finished(int)), this, SLOT(updateMenus()));
}

void PowerGui::on_actionMenu_setup_triggered()
{
	DlgMenuSetup ms;
	QStringList menus = getSetting("userMenus").toStringList();
	if(ms.exec()){
		QList <QAction*> actions = menuBar()->actions();
		foreach(QAction *_action, actions){
			QMenu *menu = _action->menu();	
			if(menu != NULL){
				if(menus.contains(menu->title())){
					menu->clear();
					menuBar()->removeAction(_action);
				}
			}
		}
		actions.clear();
		actions = toolBar->actions();
		foreach(QAction *_action, actions){
			if(_userActions.contains(_action))
					toolBar->removeAction(_action);
		}
		loadUserMenus();
		// User actions 
  		foreach(QAction *action, _userActions)
  		{
    		UserMenuAction userAction(action->data().toStringList());
  	 	 	if (userAction.requiresModel())
      			action->setEnabled(false);
    		else
      			action->setEnabled(true);
 	 	}	
	}
  	connect(&ms,SIGNAL(finished(int)), this, SLOT(updateMenus()));
}
