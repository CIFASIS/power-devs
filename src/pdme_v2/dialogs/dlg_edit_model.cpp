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

#include <QFileDialog>
#include <QFileInfo>
#include <QProcess>
#include <QTreeWidget>
#include <QMessageBox>

#include <globals.h>
#include <dialogs/dlg_edit_model.h>
#include <dialogs/dlg_edit_list_parameter.h>
#include <data/atomic.h>
#include <data/coupled.h>
#include <graphics/gpx_block.h>
#include <graphics/gpx_atomic.h>
#include <graphics/gpx_coupled.h>
#include <graphics/gpx_edition_scene.h>

void printParameter(std::ostream &out,const Parameter *p);

DlgEditModel::DlgEditModel(Coupled *c): prev(NULL), _b(NULL), _c(c)
{
  setupUi(this);
  QApplication::setOverrideCursor (QCursor(Qt::ArrowCursor)); 
	validator = new QDoubleValidator(this);
  for (int p=0;p<_c->parameterCount();p++)
  {
	  Parameter *par = c->parameter(p);
    parameters->insertRow(p);
    parameters->setItem(p,0,new QTableWidgetItem(par->name().c_str()));
    parameters->setItem(p,3,new QTableWidgetItem(QString(par->description().c_str()).replace("\\n","\n")));
    if (par->isString()) {
      parameters->setItem(p,1,new QTableWidgetItem("String"));
      StringParameter *sp = dynamic_cast<StringParameter*>(par);
      parameters->setItem(p,2,new QTableWidgetItem(sp->strValue().c_str()));
			_parameters.push_back(new StringParameter(*sp));
    }
    if (par->isValue()) {
      parameters->setItem(p,1,new QTableWidgetItem("Value"));
      ValueParameter *vp = dynamic_cast<ValueParameter*>(par);
      parameters->setItem(p,2,new QTableWidgetItem(QString("%1").arg(vp->value())));
			_parameters.push_back(new ValueParameter(*vp));
    }
    if (par->isList()) {
      parameters->setItem(p,1,new QTableWidgetItem("List"));
      ListParameter *lp = dynamic_cast<ListParameter*>(par);
      parameters->setItem(p,2,new QTableWidgetItem(lp->value().c_str()));
			_parameters.push_back(new ListParameter(*lp));
    }
  }
  valueList->setVisible(false);
  labelList->setVisible(false);
  enableParamValues(false);
	tabWidget->removeTab(0);
	tabWidget->removeTab(1);
}

DlgEditModel::DlgEditModel(GpxBlock *b): prev(NULL), _b(b), _c(NULL)
{
  setupUi(this);
  timer.setInterval(2000);
  validator = new QDoubleValidator(this);
  QApplication::setOverrideCursor (QCursor(Qt::ArrowCursor)); 
  name->setText(_b->model()->name().c_str());
  description->setPlainText(QString(_b->model()->description().c_str()).replace("\\n","\n"));
  inputs->setValue(_b->model()->inPorts());
  outputs->setValue(_b->model()->outPorts());
  qDebug() << "WIDTH: " << _b->getWidth();
  qDebug() << "HEIGHT: " << _b->getHeight();
  width_block->setValue(_b->getWidth());
  height_block->setValue(_b->getHeight());
  comboDirection->setCurrentIndex((int)_b->direction());
  for (unsigned int i=1;i<=15;i++)
    colorCombo->addItem(createColorIcon(intToColor(i)),colorToName(i));
  prevView = new QGraphicsView(gpPreview);
  prevView->setEnabled(false);
  if (_b->model()->isAtomic()) 
  {
    Atomic *a = dynamic_cast<Atomic*>(_b->model());
    Atomic *copy = new Atomic(*a);
    Coupled temp;
    temp.addChild(copy);
    prev = new LibraryScene(this,&temp);
    temp.clearModel();
	  prev->setSceneRect(QRectF(0,0,20,20));
    GpxAtomic *ga = qgraphicsitem_cast<GpxAtomic*>(prev->block());
  	ga->setWidth(45);
	  ga->setHeight(45);
    path->setText(ga->path());
    colorCombo->setCurrentIndex(ga->color());
    iconPath->setText(ga->icon());
    prevView->setScene(prev);
    init_dirs(ga->path());
    connect(&timer,SIGNAL(timeout()),this,SLOT(timer_tick()));  
    timer.start();
  } else {
    Coupled *c = dynamic_cast<Coupled*>(b->model());
    Coupled *copy = new Coupled(*c);
    Coupled temp;
    temp.addChild(copy);
    prev = new LibraryScene(this,&temp);
    temp.clearModel();
	  prev->setSceneRect(QRectF(0,0,20,20));
    GpxCoupled *gc = qgraphicsitem_cast<GpxCoupled*>(prev->block());
  	gc->setWidth(45);
	  gc->setHeight(45);
    colorCombo->setCurrentIndex(gc->color());
    iconPath->setText(gc->icon());
    prevView->setScene(prev);
    inputs->setEnabled(false);
    outputs->setEnabled(false);
    tabWidget->setTabEnabled(2,false);
  }
  layoutPreview->addWidget(prevView);
  for (int p=0;p<_b->model()->parameterCount();p++)
  {
	  Parameter *par = b->model()->parameter(p);
    parameters->insertRow(p);
    parameters->setItem(p,0,new QTableWidgetItem(par->name().c_str()));
    parameters->setItem(p,3,new QTableWidgetItem(QString(par->description().c_str()).replace("\\n","\n")));
    if (par->isString()) {
      parameters->setItem(p,1,new QTableWidgetItem("String"));
      StringParameter *sp = dynamic_cast<StringParameter*>(par);
      parameters->setItem(p,2,new QTableWidgetItem(sp->strValue().c_str()));
			_parameters.push_back(new StringParameter(*sp));
    }
    if (par->isValue()) {
      parameters->setItem(p,1,new QTableWidgetItem("Value"));
      ValueParameter *vp = dynamic_cast<ValueParameter*>(par);
      parameters->setItem(p,2,new QTableWidgetItem(QString("%1").arg(vp->value())));
			_parameters.push_back(new ValueParameter(*vp));
    }
    if (par->isList()) {
      parameters->setItem(p,1,new QTableWidgetItem("List"));
      ListParameter *lp = dynamic_cast<ListParameter*>(par);
      parameters->setItem(p,2,new QTableWidgetItem(lp->value().c_str()));
			_parameters.push_back(new ListParameter(*lp));
   }
  }
  valueList->setVisible(false);
  labelList->setVisible(false);
  enableParamValues(false);
}

QIcon DlgEditModel::createColorIcon(QColor color)
{
	QPixmap pixmap(20, 20);
	QPainter painter(&pixmap);
	painter.setPen(Qt::NoPen);
	painter.fillRect(QRect(0, 0, 20, 20), color);
	return QIcon(pixmap);
}

DlgEditModel::~DlgEditModel()
{
	if (_b) 
	{
  		delete prevView;
		delete prev;
	  	// TODO: Release extra params and blocks
	/*	for(int p = 0; p < _b->model()->parameterCount(); p++){
			_parameters[p] = NULL;
			delete _parameters[p];
		}
		_b = NULL;
		delete _b;
		delete _c;*/
	} else {
		for(int p = 0; p < _c->parameterCount(); p++){
			_parameters[p] = NULL;
			delete _parameters[p];
		}
		delete _b;
		_c = NULL;
		delete _c;
	}
	delete validator;
}
  
void DlgEditModel::on_inputs_valueChanged ( int i )
{
  qDebug() << " Inputs to " << i;
  if (!prev)
    return;
  GpxBlock *b = prev->block();
  if (b) {
    b->setInportsTo(i);
    height_block->setValue(b->getHeight()); 
  }
}

void DlgEditModel::on_outputs_valueChanged ( int i )
{
   if (!prev)
    return;
  GpxBlock *b = prev->block();
  if (b) {
    b->setOutportsTo(i);
    height_block->setValue(b->getHeight()); 
  }
 qDebug() << " Outputs to " << i;
}


void DlgEditModel::on_comboDirection_currentIndexChanged( int i )
{
  if (!prev)
    return;
  GpxBlock *b = prev->block();
  if (b) {
    int d=i-(int)b->direction();
    if (d<0) {
      d=i+(4-(int)b->direction());
    }
    for (int j=0;j<d;j++)
      b->rotate();
  }
 
  qDebug() << " Direction to " << i;
}

void DlgEditModel::on_colorCombo_currentIndexChanged( int i )
{
  qDebug() << " Color to " << i;
  if (!prev)
    return;
  GpxBlock *b = prev->block();
  if (b) {
    b->setColor(i+1);
  }
}

void DlgEditModel::on_name_textChanged ( const QString & text )
{
  if (!prev)
    return;
  GpxBlock *b = prev->block();
  if (b) {
    b->setName(text);
  }

  qDebug() << " Name to " << text;
}

void DlgEditModel::on_iconPath_textChanged ( const QString & text )
{
  qDebug() << " Icon to " << text;
  if (!prev)
    return;
  GpxBlock *b = prev->block();
  if (b) {
    b->setIcon(text);
  }


}

void DlgEditModel::on_findIcon_clicked(bool b)
{
	QString pathLib = getSetting("Path/libPath").toString();
  QString path = pathLib;
  if (iconPath->text() != "None") {
    if (iconPath->text().contains("\%")) {
      QStringList ls = iconPath->text().split("\%");
      qDebug() << ls;
      path = pathLib + "/" + ls.at(1);
    } else {
      QFileInfo qf(iconPath->text());
      path = qf.absoluteFilePath();
    }
  }
  path = QCoreApplication::applicationDirPath() + "/" + path;
  QString icon = QFileDialog::getOpenFileName ( this, "Select icon...", path , "Images (*.png *.ico *.bmp *.jpg *svg" );
  if (!icon.isEmpty()) {
    // Fix for windows
    if (icon.contains("/library/")) {
        icon = QString("\%") + icon.mid(icon.indexOf("/library/") + 9);
        icon = icon.replace(icon.indexOf("/"),1,'%');
        qDebug() << icon;
    } else {
      QDir dir(QCoreApplication::applicationDirPath());
      icon = dir.relativeFilePath(icon);
    }
    iconPath->setText(icon);
  } else {
    iconPath->setText("None");
  }
}

void DlgEditModel::on_up_clicked()
{
  qDebug() << "Up";
  QList<QTableWidgetItem *> sel = parameters->selectedItems();
  if (!sel.size())
    return;
  if (parameters->row(sel.first())>0) 
  {
    int top=parameters->row(sel.first());
    int bottom=top-1;
		
    QList<QTableWidgetItem*> sourceItems = takeRow(bottom);
    QList<QTableWidgetItem*> destItems = takeRow(top);
       
    // set back in reverse order
    setRow(bottom, destItems);
    setRow(top, sourceItems);
		Parameter *p = _parameters[bottom];
		_parameters[bottom] = _parameters[top] ;
		_parameters[top] = p;
    parameters->setCurrentCell(bottom,0);
  }
}

void DlgEditModel::on_down_clicked()
{
  qDebug() << "Down";
  QList<QTableWidgetItem *> sel = parameters->selectedItems();
  if (!sel.size())
    return;
  if (parameters->row(sel.first())<parameters->rowCount()-1) 
  {
    int top=parameters->row(sel.first());
    int bottom=top+1;
    QList<QTableWidgetItem*> sourceItems = takeRow(bottom);
    QList<QTableWidgetItem*> destItems = takeRow(top);
       
    // set back in reverse order
    setRow(bottom, destItems);
    setRow(top, sourceItems);
		Parameter *p = _parameters[bottom];
		_parameters[bottom] = _parameters[top] ;
		_parameters[top] = p;
    parameters->selectRow(bottom);
    qDebug() << "Bottom: " <<bottom;
 }

}

QList<QTableWidgetItem*> DlgEditModel::takeRow(int row)
{
  QList<QTableWidgetItem*> rowItems;
  for (int col = 0; col < parameters->columnCount(); ++col)
  {
    rowItems << parameters->takeItem(row, col);
  }
  return rowItems;
}
 
 // sets the whole row
void DlgEditModel::setRow(int row, const QList<QTableWidgetItem*>& rowItems)
{
  for (int col = 0; col < parameters->columnCount(); ++col)
  {
    parameters->setItem(row, col, rowItems.at(col));
  }
}

void DlgEditModel::on_newParam_clicked()
{
  qDebug() << "New param";
	unsigned int p=parameters->rowCount();
  parameters->insertRow(p);
  parameters->setItem(p,0,new QTableWidgetItem(""));
  parameters->setItem(p,3,new QTableWidgetItem(""));
  parameters->setItem(p,1,new QTableWidgetItem("String"));
  StringParameter *sp = new StringParameter();
  parameters->setItem(p,2,new QTableWidgetItem(""));
  _parameters.push_back(sp);
  parameters->setCurrentCell(p,0);
  nameParam->setText("");
  descParam->setPlainText("");
  valueParam->setText("");
  typeParam->setCurrentIndex(0);
  nameParam->setFocus();
}

void DlgEditModel::on_deleteParam_clicked()
{
  qDebug() << "Remove";
  QList<QTableWidgetItem *> sel = parameters->selectedItems();
  if (!sel.size())
    return;
  unsigned row = (parameters->row(sel.first()));
  parameters->removeRow(row);
  nameParam->setText("");
  descParam->setPlainText("");
  valueParam->setText("");
  typeParam->setCurrentIndex(0);
  editList->setEnabled(false);
  valueList->setVisible(false);
  labelList->setVisible(false);
  valueParam->setVisible(true);
  labelValue->setVisible(true);
	_parameters.erase(_parameters.begin()+row);
}

void DlgEditModel::on_parameters_itemSelectionChanged()
{
  QList<QTableWidgetItem *> sel = parameters->selectedItems();
  if (!sel.size())
    return;
  enableParamValues(true);
  int row=parameters->row(sel.first());
  qDebug() << "change " << row;
  nameParam->setText(parameters->item(row,0)->text());
  editList->setEnabled(false);
  QString type=parameters->item(row,1)->text();
  if (type=="String")
  {
    typeParam->setCurrentIndex(0);
    valueParam->setText(parameters->item(row,2)->text());
    valueParam->setValidator(NULL);
    valueList->setVisible(false);
    labelList->setVisible(false);
    valueParam->setVisible(true);
    labelValue->setVisible(true);
  }
  if (type=="Value")
  {
    typeParam->setCurrentIndex(1);
    valueParam->setText(parameters->item(row,2)->text());
    valueList->setVisible(false);
    labelList->setVisible(false);
    valueParam->setVisible(true);
    valueParam->setValidator(validator);
    labelValue->setVisible(true);
  }
  if (type=="List")
  {
    editList->setEnabled(true);
    typeParam->setCurrentIndex(2);
    valueList->setVisible(true);
    labelList->setVisible(true);
    valueParam->setVisible(false);
    labelValue->setVisible(false);
		printParameter(cout,_parameters[row]);
		valueList->blockSignals(true);
		valueList->clear();
		ListParameter *p = dynamic_cast<ListParameter*>(_parameters[row]);
		const vector<string> &l = p->list();
		vector<string>::const_iterator i=l.begin();
		for (;i!=l.end();i++)
		{
			string s(*i);
			valueList->addItem(s.c_str());
		}	
	  valueList->setCurrentIndex(p->selected()-1);
		valueList->blockSignals(false);
	
  }
  //name->setText(parameters->itemAt(row,0).text());
  descParam->setPlainText(parameters->item(row,3)->text());
}

void DlgEditModel::on_descParam_textChanged ( )
{
  qDebug() << "Change desc";
  QList<QTableWidgetItem *> sel = parameters->selectedItems();
  if (!sel.size())
    return;
  int row=parameters->row(sel.first());
  qDebug() << "change " << row;
	_parameters[row]->setDescription(qPrintable(QString(descParam->toPlainText().trimmed()).replace("\n","\\n")));
  parameters->item(row,3)->setText(descParam->toPlainText());
}

void DlgEditModel::on_nameParam_textChanged ( const QString & text )
{
  qDebug() << "Change name";
  QList<QTableWidgetItem *> sel = parameters->selectedItems();
  if (!sel.size())
    return;
  int row=parameters->row(sel.first());
  qDebug() << "change " << row;
	_parameters[row]->setName(qPrintable(text));
  parameters->item(row,0)->setText(text);
}

void DlgEditModel::on_editList_clicked()
{
  QList<QTableWidgetItem *> sel = parameters->selectedItems();
  if (!sel.size())
    return;
  int row=parameters->row(sel.first());
  qDebug() << "Edit List";
	ListParameter *lp=dynamic_cast<ListParameter*> (_parameters[row]);
	vector<string> s(lp->list());
	DlgEditListParameter d(s);
  if (d.exec()==QDialog::Accepted)
  {
    lp->setList(s);
	printParameter(cout,lp);
    on_parameters_itemSelectionChanged();
    parameters->item(row,2)->setText(lp->value().c_str());
  }
}

void DlgEditModel::on_valueList_currentIndexChanged(int index)
{
  QList<QTableWidgetItem *> sel = parameters->selectedItems();
  if (!sel.size() || index<0)
    return;
  int row=parameters->row(sel.first());
	ListParameter *p = dynamic_cast<ListParameter*>(_parameters[row]);
	//const vector<string> &l = p->list();
	p->setSelected(index+1);	
  parameters->item(row,2)->setText(p->value().c_str());
	qDebug() << "Changing index to " << p->selected();
}

void DlgEditModel::on_newCode_clicked()
{
  QProcess::startDetached(getSetting("Path/atomicsEditor").toString());
}

void DlgEditModel::on_editCode_clicked()
{
    if (path->text().isEmpty())
     return;
    QString s_path = QCoreApplication::applicationDirPath()  + "/" + getSetting("Path/atomicsPath").toString() + "/" + path->text();
#ifdef Q_OS_LINUX
    s_path = s_path.replace('\\','/');
#else
    s_path = s_path.replace('/','\\');
#endif
    qDebug() << s_path;
    if (!path->text().isEmpty())
      QProcess::startDetached(getSetting("Path/atomicsEditor").toString(),QStringList() << s_path);
}

void DlgEditModel::on_directory_changed( QTreeWidgetItem * current, QTreeWidgetItem * previous )
{
  qDebug() << current->text(0);
}

void DlgEditModel::init_dirs(QString code)
{
    QString path = QCoreApplication::applicationDirPath () + "/" + getSetting("Path/atomicsPath").toString() + "/";
#ifdef Q_OS_LINUX
    path = path.replace('\\','/');
#else
    path = path.replace('/','\\');
#endif
    QDir d(path);
    d.setFilter(QDir::Dirs);
    QFileInfoList dir = d.entryInfoList ();
    QTreeWidgetItem *item = new QTreeWidgetItem();
    item->setText(0,"Atomics");
    QTreeWidgetItem *current=NULL;
    QStringList ls = code.replace('\\','/').split("/");
    foreach (QFileInfo qf, dir)
    {
      if (qf.isDir() && qf.baseName()!="") {
        QTreeWidgetItem *i = new QTreeWidgetItem();
        if (ls.size() && ls.first()==qf.baseName())
          current=i;
        i->setText(0,qf.baseName());
        item->addChild(i);
      }
    }
    dirs->insertTopLevelItems(0,QList<QTreeWidgetItem*> () << item);
    item->setExpanded(true);
    dirs->setHeaderLabel("Folders");
    if (current)
      dirs->setCurrentItem(current);
    if (ls.size()>1)
      update_files(ls.at(1)); 
    else
      update_files();

}

void DlgEditModel::update_files(QString file)
{
    files->clear();
    QString path = QCoreApplication::applicationDirPath () + "/" + getSetting("Path/atomicsPath").toString() + "/";
#ifdef Q_OS_LINUX
    path = path.replace('\\','/');
#else
    path = path.replace('/','\\');
#endif
    if (!dirs->currentItem())
        return;
    path = path + dirs->currentItem()->text(0);
    QDir d(path);
    d.setNameFilters(QStringList() << "*.h");
    QFileInfoList dir = d.entryInfoList ();
    int row=-1;
    foreach (QFileInfo qf, dir)
    {
      files->addItem(qf.fileName());
      if (qf.fileName()==file)
        row = (files->count()-1);
    }
    if (row>=0)
      files->setCurrentRow(row);
}

void DlgEditModel::on_dirs_currentItemChanged ( QTreeWidgetItem * current, QTreeWidgetItem * previous ) 
{
  update_files();
}

void DlgEditModel::on_files_itemSelectionChanged()
{
  QList<QListWidgetItem *> items = files->selectedItems ();
  if (items.size())
  {
    path->setText(dirs->currentItem()->text(0) + "/" + files->currentItem()->text());
  }
}

void DlgEditModel::timer_tick()
{
  QStringList ls = path->text().replace('\\','/').split('/');
  if (ls.size()>1)
    update_files(ls.at(1)); 
  else
    update_files();
}

void DlgEditModel::on_buttonBox_clicked ( QAbstractButton * button )
{
  //TODO: check this update when the outports/inports change in the blocks, also, check for the coupled blocks.
  if (button == buttonBox->button(QDialogButtonBox::Apply) || button == buttonBox->button(QDialogButtonBox::Ok))
  {
  	if (_b && !prev)
    	return;
		if (_b) {
		  GpxBlock *b = prev->block();
      	  GpxEditionScene *scene = dynamic_cast<GpxEditionScene*>(_b->scene());
	      Model *m = _b->model();
          if (scene->canUseName(name->text(),_b)) {
	        m->setName(qPrintable(name->text()));
	        _b->setName(name->text());
          } else {
            QMessageBox::warning(this,"PowerDEVS","The name is already in use");
            name->setText(_b->name());
          }
 	      if (_b->isAtomic())
	      {
	      GpxAtomic *a = qgraphicsitem_cast<GpxAtomic*>(_b);
		  if(_b->direction() != b->direction())
	    	a->setDirection((Graphic::Direction)comboDirection->currentIndex());
		    a->setIcon(iconPath->text());
	      a->setWidth(width_block->value());
	      a->setHeight(height_block->value());
	      a->setColor(colorCombo->currentIndex()+1);
	      a->setPath(path->text());
	      a->setInportsTo(inputs->value());
	      a->setOutportsTo(outputs->value());
        _b->itemChange(QGraphicsItem::ItemPositionHasChanged,QVariant());
	      } else if (_b->isCoupled()) {
	      GpxCoupled *c = qgraphicsitem_cast<GpxCoupled*>(_b);
		  if(_b->direction() != b->direction())
	    	c->setDirection((Graphic::Direction)comboDirection->currentIndex());
		    c->setIcon(iconPath->text());
	      c->setWidth(width_block->value());
	      c->setHeight(height_block->value());
	      c->setColor(colorCombo->currentIndex()+1);
	      c->setInportsTo(inputs->value());
	      c->setOutportsTo(outputs->value());
        _b->itemChange(QGraphicsItem::ItemPositionHasChanged,QVariant());
	      }
	      m->setDescription(qPrintable(description->toPlainText().replace("\n","\\n")));
        _b->setToolTip(description->toPlainText());
	      m->setParameters(_parameters);
		  } else {
	      _c->setParameters(_parameters);
		 }
   }
  qDebug() << "Apply changes";
}

void DlgEditModel::on_typeParam_currentIndexChanged(int index)
{
  qDebug() << "Change type";
  QList<QTableWidgetItem *> sel = parameters->selectedItems();
  if (!sel.size())
    return;
  int row=parameters->row(sel.first());
  Parameter *p = _parameters[row];
  if (index==0 && !p->isString())
  {
    editList->setEnabled(false);
    _parameters[row] = new StringParameter();
    _parameters[row]->setName(p->name());
    _parameters[row]->setDescription(p->description());
    delete p;
    p = _parameters[row];
    parameters->item(row,1)->setText(("String"));
    parameters->item(row,2)->setText("");
    valueList->setVisible(false);
    labelList->setVisible(false);
    valueParam->setVisible(true);
    valueParam->setText("");
    labelValue->setVisible(true);
    valueParam->setValidator(NULL);
  }
  if (index==1 && !p->isValue())
  {
    valueParam->setValidator(validator);
    editList->setEnabled(false);
    _parameters[row] = new ValueParameter();
    _parameters[row]->setName(p->name());
    _parameters[row]->setDescription(p->description());
    delete p;
    p = _parameters[row];
    parameters->item(row,1)->setText(("Value"));
    parameters->item(row,2)->setText("0");
    valueList->setVisible(false);
    labelList->setVisible(false);
    valueParam->setVisible(true);
    labelValue->setVisible(true);
    valueParam->setText("");
  }
  if (index==2 && !p->isList())
  {
    valueParam->setValidator(NULL);
    editList->setEnabled(true);
    _parameters[row] = new ListParameter();
    _parameters[row]->setName(p->name());
    _parameters[row]->setDescription(p->description());
    delete p;
    p = _parameters[row];
    parameters->item(row,1)->setText(("List"));
    parameters->item(row,2)->setText("");
    valueList->setVisible(true);
	valueList->clear();
    labelList->setVisible(true);
    valueParam->setVisible(false);
    labelValue->setVisible(false);
  }

}

void DlgEditModel::on_valueParam_textChanged ( const QString & text )
{
  qDebug() << "Change value";
  QList<QTableWidgetItem *> sel = parameters->selectedItems();
  if (!sel.size())
    return;
  int row=parameters->row(sel.first());
  Parameter *p = _parameters[row];
  parameters->item(row,2)->setText(text);
  if (p->isString())
  {
    StringParameter *sp = dynamic_cast<StringParameter*>(p);
    sp->setValue(qPrintable(text));
  }
  if (p->isValue())
  {
    ValueParameter *vp = dynamic_cast<ValueParameter*>(p);
	vp->setValue(text.toDouble());
  }
}

void DlgEditModel::enableParamValues(bool enable)
{
	nameParam->setEnabled(enable);
	typeParam->setEnabled(enable);
	valueParam->setEnabled(enable);
	valueList->setEnabled(enable);
	descParam->setEnabled(enable);
}
