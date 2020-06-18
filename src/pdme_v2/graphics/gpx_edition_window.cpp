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

#include <QDebug>
#include <QLabel>
#include <QTabWidget>
#include <QInputDialog>
#include <QFileInfo>
#include <QCloseEvent>
#include <QMdiSubWindow>

#include <QMessageBox>
#include <data/coupled.h>
#include <graphics/gpx_edition_window.h>
#include <graphics/gpx_edition_scene.h>
#include <graphics/gpx_coupled.h>
#include <dialogs/dlg_change_annotation.h>

GpxEditionWindow::GpxEditionWindow(QWidget *parent, Coupled *c):QTabWidget(parent), _coupledData(c)
{
  setTabsClosable(true);
  setTabPosition(QTabWidget::South);
  setWindowTitle(QString("%1 [*]").arg(c->name().c_str()));
  clearDirty();
  openCoupled(c);

  connect(this, SIGNAL(tabCloseRequested(int)), this, SLOT(on_tabCloseRequested(int)));
  connect(this, SIGNAL(currentChanged(int)), this, SLOT(on_currentChanged(int)));
}

GpxEditionWindow::GpxEditionWindow(QWidget *parent):QTabWidget(parent)
{
  static unsigned int modelNumber = 0;

  setTabsClosable(true);
  setTabPosition(QTabWidget::South);
  setWindowTitle(QString("Model %1 [*]").arg(modelNumber++));
  setDirty();
  _coupledData = new Coupled();
  _coupledData->setName( qPrintable(QString("Model %1").arg(modelNumber-1)));
  openCoupled(_coupledData);

  connect(this, SIGNAL(tabCloseRequested(int)), this, SLOT(on_tabCloseRequested(int)));
  connect(this, SIGNAL(currentChanged(int)), this, SLOT(on_currentChanged(int)));
}

#ifdef UNDO
	GpxEditionWindow::GpxEditionWindow(QWidget *parent, Coupled *c, QUndoGroup *ug):QTabWidget(parent), _coupledData(c)
	{
  		setTabsClosable(true);
  		setTabPosition(QTabWidget::South);
		setWindowTitle(QString("%1 [*]").arg(c->name().c_str()));
  		clearDirty();
  		_undoGroup = ug;
  		openCoupled(c);

  		connect(this, SIGNAL(tabCloseRequested(int)), this, SLOT(on_tabCloseRequested(int)));
  		connect(this, SIGNAL(currentChanged(int)), this, SLOT(on_currentChanged(int)));
	}

	GpxEditionWindow::GpxEditionWindow(QWidget *parent, QUndoGroup *ug):QTabWidget(parent)
	{
  		static unsigned int modelNumber = 0;

  		setTabsClosable(true);
  		setTabPosition(QTabWidget::South);
  		setWindowTitle(QString("Model %1 [*]").arg(modelNumber++));
  		setDirty();
  		_coupledData = new Coupled();
  		_coupledData->setName( qPrintable(QString("Model %1").arg(modelNumber-1)));
  		_undoGroup = ug;
  		openCoupled(_coupledData);

  		connect(this, SIGNAL(tabCloseRequested(int)), this, SLOT(on_tabCloseRequested(int)));
  		connect(this, SIGNAL(currentChanged(int)), this, SLOT(on_currentChanged(int)));
	}
#endif

GpxEditionWindow::~GpxEditionWindow()
{
  delete _coupledData;
}

void GpxEditionWindow::setFileName(QString fileName)
{
  _fileName = fileName; 
  QFileInfo qf(fileName);
  setWindowTitle(QString("%1 [*]").arg(qf.baseName()));
  setTabText(0,QString("%1").arg(qf.baseName()));
}

void GpxEditionWindow::closeEvent(QCloseEvent *event)
{
  if (count()>1 && currentIndex()>0) {
    on_tabCloseRequested(currentIndex());
    event->ignore();
    return;
  }
   
  if (isDirty())
  {
    event->ignore();
    emit saveModel(this,event);
    if (!isDirty())
    {
      event->accept();
    }
    return;
  }
  event->accept();
}

void GpxEditionWindow::openCoupled(Coupled *c)
{
  GpxEditionScene *ges;
  qDebug() << "Opening " << c->name().c_str() << "with father" << (c->father() ? c->father()->name().c_str() : "");
  if (_openIn.contains(c)) {
    setCurrentIndex(_openIn[c]);    
    return;
  }
  ges = new GpxEditionScene(this, c);
  #ifdef UNDO
  	_undoGroup->addStack(ges->undoStack());
  #endif
  QGraphicsView *gv = new QGraphicsView(ges);
  gv->setRenderHint(QPainter::Antialiasing);
  //gv->setAlignment(Qt::AlignLeft | Qt::AlignTop);
  gv->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
  QString tabName;
  if (!count()) { 
      tabName = windowTitle();
      tabName = tabName.remove(" [*]");
  }
  else 
      tabName = completePathName(c);
  /***************************************************************************************/
  connect(ges,SIGNAL(selectionChanged()),this,SLOT(on_sceneSelectionChanged()));
  connect(ges,SIGNAL(contextMenu(GpxBlock*,QPoint)),this,SLOT(on_contextMenu(GpxBlock*,QPoint)));
  connect(ges,SIGNAL(showParameterDialog(GpxBlock*)),this,SLOT(on_parameterDialog(GpxBlock*)));
  connect(ges,SIGNAL(closeAllChildsOf(Coupled*)),this,SLOT(closeAllChildsOf(Coupled*)));
  connect(ges,SIGNAL(add_Port(Coupled*, Port*)),this,SLOT(on_add_Port(Coupled*, Port*)));
  connect(ges,SIGNAL(remove_Port(Coupled*, Port*)),this,SLOT(on_remove_Port(Coupled*, Port*)));
  connect(ges,SIGNAL(modified()),this,SLOT(on_modified()));
  connect(ges,SIGNAL(modelParameter(Coupled*)),this,SLOT(on_parameter(Coupled*)));
  connect(ges,SIGNAL(setBlockName(GpxBlock*)),this,SLOT(on_setBlockName(GpxBlock*)));
  /***************************************************************************************/
  addTab(gv, tabName);
  _openIn.insert(c,count()-1);
  setCurrentIndex(count()-1);    
  #ifdef UNDO
  	on_currentChanged(count()-1);    
  #endif
  qDebug() << _openIn;
}

void GpxEditionWindow::on_setBlockName(GpxBlock *b)
{
	if(b->isCoupled()){
		GpxCoupled *c = dynamic_cast<GpxCoupled*>(b);
		Coupled *coupled = c->coupledData();
		if(_openIn.contains(coupled))
			setTabText(_openIn[coupled],completePathName(coupled));
	}
}

void GpxEditionWindow::on_tabCloseRequested(int index)
{
	if (index == 0) 
		(QMdiSubWindow*)parentWidget()->close();
  else
  {
    QGraphicsView *gv = qobject_cast<QGraphicsView*>(widget(index));
    if (gv !=NULL)
    {
      GpxEditionScene *ges = qobject_cast<GpxEditionScene*>(gv->scene());
      Coupled *c = ges->coupledData();
      QMap<Coupled*,unsigned>::iterator i=_openIn.begin();
      for (;i!=_openIn.end();i++)
      {
        if (i.value()>=(unsigned)index)
          _openIn[i.key()]=_openIn[i.key()]-1;
      }
      _openIn.remove(c); 
      // Close all childs

    }
		removeTab(index);
  }
}

#ifdef UNDO
	void GpxEditionWindow::on_currentChanged(int index)
	{
		QGraphicsView *gv = qobject_cast<QGraphicsView*>(widget(index));
    	if (gv !=NULL) {
      		GpxEditionScene *ges = qobject_cast<GpxEditionScene*>(gv->scene());
      		_undoGroup->setActiveStack(ges->undoStack());
    	}
	}
#endif

void GpxEditionWindow::on_sceneSelectionChanged()
{
  emit selectionChanged();
}

bool GpxEditionWindow::hasModelSelected()
{
  int tab = currentIndex();
  if (tab<0) // This can never happen
    return false;
  QGraphicsView *gv = qobject_cast<QGraphicsView*>(widget(tab));
  if (gv !=NULL)
  {
      GpxEditionScene *ges = qobject_cast<GpxEditionScene*>(gv->scene());
      if (ges!=NULL && ges->selectedItems().size()==1 && ges->selectedItems().first()->type()==GpxBlock::Type) {
        GpxBlock *b = qgraphicsitem_cast<GpxBlock*>(ges->selectedItems().first());
        return b->isAtomic() || b->isCoupled();
      }
  }
  return false;
}

bool GpxEditionWindow::hasCoupledSelected()
{
  int tab = currentIndex();
  if (tab<0) // This can never happen
    return false;
  QGraphicsView *gv = qobject_cast<QGraphicsView*>(widget(tab));
  if (gv !=NULL)
  {
      GpxEditionScene *ges = qobject_cast<GpxEditionScene*>(gv->scene());
      if (ges!=NULL && ges->selectedItems().size()==1 && ges->selectedItems().first()->type()==GpxBlock::Type) {
        GpxBlock *b = qgraphicsitem_cast<GpxBlock*>(ges->selectedItems().first());
        return b->isCoupled();
      }
  }
  return false;
  
}


bool GpxEditionWindow::hasItemSelected()
{
  int tab = currentIndex();
  if (tab<0) // This can never happen
    return false;
  QGraphicsView *gv = qobject_cast<QGraphicsView*>(widget(tab));
  if (gv !=NULL)
  {
      GpxEditionScene *ges = qobject_cast<GpxEditionScene*>(gv->scene());
      if (ges!=NULL && ges->selectedItems().size()==1) {
        return ges->selectedItems().first()->type()==GpxBlock::Type;
      }
  }
  return false;
  
}
bool GpxEditionWindow::hasSelection()
{
  int tab = currentIndex();
  if (tab<0) // This can never happen
    return false;
  QGraphicsView *gv = qobject_cast<QGraphicsView*>(widget(tab));
  if (gv !=NULL)
  {
      GpxEditionScene *ges = qobject_cast<GpxEditionScene*>(gv->scene());
      if (ges!=NULL)
        return ges->selectedItems().size();
  }
  return false;
}

void GpxEditionWindow::rotate()
{
  int tab = currentIndex();
  if (tab<0) // This can never happen
    return ;
  QGraphicsView *gv = qobject_cast<QGraphicsView*>(widget(tab));
  if (gv !=NULL)
  {
      GpxEditionScene *ges = qobject_cast<GpxEditionScene*>(gv->scene());
      if (ges!=NULL)
        ges->rotate();
  }
}

void GpxEditionWindow::deleteSelection()
{
  int tab = currentIndex();
  if (tab<0) // This can never happen
    return ;
  QGraphicsView *gv = qobject_cast<QGraphicsView*>(widget(tab));
  if (gv !=NULL)
  {
      GpxEditionScene *ges = qobject_cast<GpxEditionScene*>(gv->scene());
      if (ges!=NULL)
        ges->deleteSelection();
  }
}

void GpxEditionWindow::selectAll()
{
  int tab = currentIndex();
  if (tab<0) // This can never happen
    return ;
  QGraphicsView *gv = qobject_cast<QGraphicsView*>(widget(tab));
  if (gv !=NULL)
  {
      GpxEditionScene *ges = qobject_cast<GpxEditionScene*>(gv->scene());
      if (ges!=NULL)
        ges->selectAll();
  }
}
void GpxEditionWindow::on_contextMenu(GpxBlock *b,QPoint p)
{
  //qDebug() << "Block is at " << b << endl;
  emit contextMenu(b,p);
}

GpxEditionScene *GpxEditionWindow::currentScene()
{
  int tab = currentIndex();
  if (tab<0) // This can never happen
    return NULL;
  QGraphicsView *gv = qobject_cast<QGraphicsView*>(widget(tab));
  if (gv !=NULL )
  {
    GpxEditionScene *ges = qobject_cast<GpxEditionScene*>(gv->scene());
    if (ges!=NULL) {
      return ges;
    }
  }
  return NULL;
}
 
void GpxEditionWindow::on_parameterDialog(GpxBlock *b)
{
  emit parameterDialog(b);
}

void GpxEditionWindow::openCoupled()
{
  int tab = currentIndex();
  if (tab<0) // This can never happen
    return;
  QGraphicsView *gv = qobject_cast<QGraphicsView*>(widget(tab));
  if (gv !=NULL)
  {
      GpxEditionScene *ges = qobject_cast<GpxEditionScene*>(gv->scene());
      if (ges!=NULL && ges->selectedItems().size()) {
        if (ges->selectedItems().first()->type()==GpxBlock::Type) 
        {
          GpxBlock *b = qgraphicsitem_cast<GpxBlock*>(ges->selectedItems().first());
          if (b->isCoupled()) {
            GpxCoupled *gpx_coupled = qgraphicsitem_cast<GpxCoupled*>(b);
            openCoupled(gpx_coupled->coupledData());
          }

         
        }
      }
  }
}

QString GpxEditionWindow::completePathName(Coupled *c)
{
  QString name = QString(c->name().c_str());
  while (c!=NULL)
  {
    c=c->father();
    if (c!=NULL)
      name = QString("%1\\%2").arg(c->name().c_str()).arg(name);
  }
  return name;
}

void GpxEditionWindow::closeAllChildsOf(Coupled *c)
{
  qDebug() << "Closing " << c->name().c_str();
  if (_openIn.contains(c)) {
      on_tabCloseRequested(_openIn[c]); 
  }
  for (int i=0;i<c->childCount();i++)
  {
    Model *m=c->child(i);
    if (m->isCoupled()) {
      Coupled *child=dynamic_cast<Coupled*>(m);
      closeAllChildsOf(child);
    }
  }
}

void GpxEditionWindow::on_remove_Port(Coupled *c, Port *p)
{
  if (_openIn.contains(c->father())) {
    // The father is open
    int tab = _openIn[c->father()];
    if (tab<0) // This can never happen
      return;
    QGraphicsView *gv = qobject_cast<QGraphicsView*>(widget(tab));
    if (gv !=NULL )
    {
      GpxEditionScene *ges = qobject_cast<GpxEditionScene*>(gv->scene());
      if (ges!=NULL) 
        ges->removePort(c,p);
    }
  } else { //if (_openIn.contains(c)){
  	if(p->type() == Port::Inport)
    	c->removeInport(p);
	else
		c->removeOutport(p);
  }
}

void GpxEditionWindow::on_add_Port(Coupled *c, Port *p)
{
  if (_openIn.contains(c->father())) {
    // The father is open
    int tab = _openIn[c->father()];
    if (tab<0) // This can never happen
      return;
    QGraphicsView *gv = qobject_cast<QGraphicsView*>(widget(tab));
    if (gv !=NULL )
    {
      GpxEditionScene *ges = qobject_cast<GpxEditionScene*>(gv->scene());
      if (ges!=NULL) 
        ges->addPort(c,p);
    }
  } else { // if (_openIn.contains(c)){
  	if(p->type() == Port::Inport)
    	c->addInport(p);
	else
		c->addOutport(p);
  }
  /*} else { // If the father is closed.
		for(int i = 0; i < _coupledData->childCount(); i++){
			Model *m = _coupledData->child(i);
			if(m->isCoupled())
				Coupled *child = dynamic_cast<Coupled*>(m);
				if(child == c)
					c*/
	/*int tab = _openIn[c];
    QGraphicsView *gv = qobject_cast<QGraphicsView*>(widget(tab));
    if (gv !=NULL )
    {
      GpxEditionScene *ges = qobject_cast<GpxEditionScene*>(gv->scene());
      if (ges!=NULL) 
        ges->addPort(c,p);
    }*/
  	
  //}

}

/*
void GpxEditionWindow::on_setOutports(Coupled *c, unsigned p)
{
  qDebug() << "Set outports to" << p;
  if (_openIn.contains(c->father())) {
    // The father is open
    int tab = _openIn[c->father()];
    if (tab<0) // This can never happen
      return;
    QGraphicsView *gv = qobject_cast<QGraphicsView*>(widget(tab));
    if (gv !=NULL )
    {
      GpxEditionScene *ges = qobject_cast<GpxEditionScene*>(gv->scene());
      if (ges!=NULL) 
        ges->setOutportsOf(c,p);
    }
  	c->setOutports(p);
  } else {
		Coupled *father=c->father();
		if (father!=NULL) {
			int child=father->childIndex(c)+1;
			for (int i=0;i<father->lineCount();i++) {
				Line *l = father->lineAt(i);
				if (l->sourceType()==Line::COMPONENT && l->sourceAt(0)==child && l->sourceAt(1)>p) 
					father->removeLine(l);	
				if (l->sinkType()==Line::COMPONENT && l->sinkAt(0)==child && l->sinkAt(1)>p) 
					father->removeLine(l);	
				father->simplifyLines();
			}
		}
  	c->setOutports(p);
	}
}
*/

void GpxEditionWindow::on_modified()
{
  setDirty(); 
}

void GpxEditionWindow::on_parameter(Coupled *c) { 
		emit parameterDialogCoupled(c);
}

#ifdef UNDO
	void GpxEditionWindow::setActiveUndoStack()
	{
		if(count())
			on_currentChanged(currentIndex());
	}
#endif

QString GpxEditionWindow::getSelection()
{
	if (currentScene()!=NULL) {
        return currentScene()->getSelection();
    }
    return "";
}

Coupled *GpxEditionWindow::getCurrentCoupledData()
{
    if (currentScene()!=NULL) {
        return currentScene()->coupledData();
	}
    return NULL;
}

GpxBlock *GpxEditionWindow::getCurrentItem() 
{
    if (hasModelSelected() && currentScene()!=NULL) {
        return qgraphicsitem_cast<GpxBlock*>(currentScene()->selectedItems().first());
    }
    return NULL;
}

void GpxEditionWindow::paste(QString s) {
    if (currentScene()!=NULL) {
        currentScene()->paste(s);
    }
}

void GpxEditionWindow::addAnnotation(QPoint p)
{
    if (currentScene()!=NULL) {
        DlgChangeAnnotation dlg(NULL);
        QString annotation = dlg.getAnnotation();
        if (!annotation.isNull()) {
          currentScene()->addAnnotation(annotation,p);
          setDirty();
        }
    }
}
void GpxEditionWindow::addAnnotation() {
  addAnnotation(QPoint());
}

void GpxEditionWindow::convertToCoupled() {
  if (currentScene()!=NULL) {
    currentScene()->convertToCoupled();
    setDirty();
  }
}
