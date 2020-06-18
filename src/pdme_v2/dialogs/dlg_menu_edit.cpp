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

#include <QSettings>
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
#include <QDir>

#include <dialogs/dlg_menu_edit.h>
#include <globals.h>

DlgMenuEdit::DlgMenuEdit(diags which, QString *menuName, QList <QString*> menuItems, QList <UserMenuAction*> *menuActions)
{
	setupUi(this);
	dialog = which;
	_menu = *menuName;
	foreach(QString *item, menuItems){
		QString copy(*item);
		_menuItems << copy;
	}
	for(int i = 0; i < menuActions->length(); i++){
		UserMenuAction copy(menuActions->at(i)->getData());
		_menuActions << copy;
	}
	_menu_local = menuName;
	_menuItems_local = menuItems;
	_menuItems_local_ptr = NULL;
	_menuActions_local = menuActions;
	
	shortcutDisabled = QStringList();
	/* Revisar, tendria que ser general.... */
	for(int i = 0; i < _menuActions.length(); i++)
		shortcutDisabled << _menuActions[i].shortcut().toString();	
	shortcutLst =
	    QStringList() << "None" << "F2" << "F3" << "F4" << "F5" << "F6" <<
	    "F7" << "F8" << "F9" << "F10" << "F11" << "F12" << "Ctrl+F1" <<
	    "Ctrl+F2" << "Ctrl+F3" << "Ctrl+F4" << "Ctrl+F5" << "Ctrl+F6" <<
	    "Ctrl+F7" << "Ctrl+F8" << "Ctrl+F9" << "Ctrl+F10" << "Ctrl+F11" <<
	    "Ctrl+F12";
	shortcutCbx->addItems(shortcutLst);
	setWindowTitle(tr("Edit Menu"));
	addMenuGbx->setVisible(false);
	menuLed->setText(_menu);
	menuEditLstWdt->addItems(_menuItems);
	menuEditLstWdt->setCurrentRow(0);
	menuEditLed->setText(_menuItems[0]);
	fileLed->setText(_menuActions[0].command());
	paramMenuLed->setText(_menuActions[0].args());
	if(_menuActions[0].hasShortcut())
		shortcutCbx->setCurrentIndex(shortcutLst.indexOf(_menuActions[0].shortcut().toString()));
	else
		shortcutCbx->setCurrentIndex(0);
	if(_menuActions[0].hasIcon())
		iconPxm = QPixmap(_menuActions[0].icon());
	else{
		removeBtn->setEnabled(false);
		iconPxm = QPixmap();
	}
	iconPxm = iconPxm.scaled(QSize(30, 30));
	iconLbl->setPixmap(iconPxm);
	if (_menuActions[0].requiresModel())
		needsOpenModel->setCheckState(Qt::Checked);
	else
		needsOpenModel->setCheckState(Qt::Unchecked);
	if (_menuActions[0].requiresSave())
		saveModelBefore->setCheckState(Qt::Checked);
	else
		saveModelBefore->setCheckState(Qt::Unchecked);
	menuLed->setFocus();
}

DlgMenuEdit::DlgMenuEdit(diags which, QString *menuName, QList <QString*> *menuItems, QList <UserMenuAction*> *menuActions)
{
	setupUi(this);
	dialog = which;
	_menu = *menuName;
	for(int i = 0; i < menuItems->length(); i++){
		QString copy(*menuItems->at(i));
		_menuItems << copy;
	}
	for(int i = 0; i < menuActions->length(); i++){
		UserMenuAction copy(menuActions->at(i)->getData());
		_menuActions << copy;
	}
	_menu_local = menuName;
	_menuItems_local_ptr = menuItems;
	_menuActions_local = menuActions;
	
	shortcutDisabled = QStringList();
	for(int i = 0; i < _menuActions.length(); i++)
		shortcutDisabled << _menuActions[i].shortcut().toString();	
	shortcutLst =
	    QStringList() << "None" << "F2" << "F3" << "F4" << "F5" << "F6" <<
	    "F7" << "F8" << "F9" << "F10" << "F11" << "F12" << "Ctrl+F1" <<
	    "Ctrl+F2" << "Ctrl+F3" << "Ctrl+F4" << "Ctrl+F5" << "Ctrl+F6" <<
	    "Ctrl+F7" << "Ctrl+F8" << "Ctrl+F9" << "Ctrl+F10" << "Ctrl+F11" <<
	    "Ctrl+F12";
	shortcutCbx->addItems(shortcutLst);
	setWindowTitle(tr("New Menu"));
	iconPxm = QPixmap();
	iconPxm = iconPxm.scaled(QSize(30, 30));
	iconLbl->setPixmap(iconPxm);
	disableNewMenuButtons();
	menuLed->setFocus();
}


DlgMenuEdit::~DlgMenuEdit()
{
	_menu_local = NULL;		
	_menuItems_local_ptr = NULL;
	_menuActions_local = NULL;
}	

void DlgMenuEdit::disableNewMenuButtons()
{
	commandGbx->setEnabled(false);
	menuEditLed->setEnabled(false);
	menuEditLed->setText("");
	menuEditLstWdt->setEnabled(false);
	toolbarGbx->setEnabled(false);
	buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
	removeItemBtn->setEnabled(false);
	needsOpenModel->setEnabled(false);
	saveModelBefore->setEnabled(false);
}

void DlgMenuEdit::on_buttonBox_accepted()
{
	if (!checkMenuValues())
		return;
	int i,j;
	switch(dialog){
		case EDIT_DIALOG:
			*_menu_local = _menu;
			i = 0;
			foreach(QString item, _menuItems){
				*_menuItems_local[i] = item;
				j = 0;
				foreach(UserMenuAction action, _menuActions){
					_menuActions_local->at(j)->setCommand(action.command());
					_menuActions_local->at(j)->setArgs(action.args());
					_menuActions_local->at(j)->setShortcut(action.shortcut().toString());
					_menuActions_local->at(j)->setIcon(action.icon());
					_menuActions_local->at(j)->setRequiresModel(action.requiresModel());
					_menuActions_local->at(j)->setRequiresSave(action.requiresSave());
					j++;
				}
				i++;
			}
			break;
		case ADD_DIALOG:
			*_menu_local = _menu;
			foreach(QString item, _menuItems){
				QString *_item = new QString(item);
				_menuItems_local_ptr->append(_item);
				foreach(UserMenuAction action, _menuActions){
					QStringList _action;
					_action << action.command();
					_action << action.args();
					_action << action.shortcut().toString();
					_action << action.icon();
					if(action.requiresModel())
						_action << "1";
					else
						_action << "0";
					if(action.requiresSave())
						_action << "1";
					else
						_action << "0";
					UserMenuAction *_menu_action = new UserMenuAction(_action);
					_menuActions_local->append(_menu_action);
				}
			}
			break;
	}
}

void DlgMenuEdit::on_addBtn_clicked()
{
	_menuItems << "";
	QStringList add;
	add << "" << "" << "" << "" << "0" << "0";
	UserMenuAction action(add);
	_menuActions.append(action);
	menuEditLstWdt->addItem("");
	menuEditLstWdt->setCurrentRow(menuEditLstWdt->count() - 1);
	commandGbx->setEnabled(true);
	menuEditLed->setEnabled(true);
	menuEditLstWdt->setEnabled(true);
	toolbarGbx->setEnabled(true);
	removeBtn->setEnabled(false);
	removeItemBtn->setEnabled(true);
	needsOpenModel->setEnabled(true);
	saveModelBefore->setEnabled(true);
	buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
	menuEditLed->setFocus();
}

void DlgMenuEdit::on_removeItemBtn_clicked()
{
	int rowToDelete = menuEditLstWdt->currentRow();
	QListWidgetItem *item = menuEditLstWdt->currentItem();
	QString str = item->text();
	menuEditLstWdt->removeItemWidget(item);
	delete item;
	_menuItems.removeAt(rowToDelete);
	_menuActions.removeAt(rowToDelete);
	if (menuEditLstWdt->count() == 0)
		disableNewMenuButtons();
	return;
}

void DlgMenuEdit::on_menuLed_textChanged(const QString & text)
{
	_menu = text;
}

void DlgMenuEdit::on_menuEditLstWdt_currentRowChanged(int row)
{
	if (row < 0)
		row = 0;
	if (row >= menuEditLstWdt->count())
		row = menuEditLstWdt->count() - 1;
	if (menuEditLstWdt->count()){
		UserMenuAction action = _menuActions[row];
		menuEditLed->setText(_menuItems[row]);
		fileLed->setText(action.command());
		paramMenuLed->setText(action.args());
		shortcutCbx->setCurrentIndex(shortcutLst.indexOf(action.shortcut().toString()));
		if (action.hasIcon()){
			iconPxm.load(action.icon());
			iconPxm = iconPxm.scaled(QSize(30, 30));
			removeBtn->setEnabled(true);
		}else{
			iconPxm = QPixmap();
			removeBtn->setEnabled(false);
		}
		iconLbl->setPixmap(iconPxm);
		if (action.requiresModel())
			needsOpenModel->setCheckState(Qt::Checked);
		else
			needsOpenModel->setCheckState(Qt::Unchecked);
		if (action.requiresSave())
			saveModelBefore->setCheckState(Qt::Checked);
		else
			saveModelBefore->setCheckState(Qt::Unchecked);
	}else{
		menuEditLed->setText("");
		menuEditLed->setEnabled(false);
		menuEditLstWdt->setEnabled(false);
		removeItemBtn->setEnabled(false);
		removeBtn->setEnabled(false);
		fileLed->setText("");
		paramMenuLed->setText("");
		shortcutCbx->setCurrentIndex(0);
		iconPxm = QPixmap();
		iconLbl->setPixmap(iconPxm);
		needsOpenModel->setCheckState(Qt::Unchecked);
		saveModelBefore->setCheckState(Qt::Unchecked);
	}
}

void DlgMenuEdit::on_selectBtn_clicked()
{
	QDir dir(QCoreApplication::applicationDirPath());
	QString fileName =
	    QFileDialog::getOpenFileName(this, "Select Icon", "../bin",
					 "Image Files (*.bmp *.jpg *.png);;All Files (*.*)");
	if (!fileName.isEmpty()) {
		int row = menuEditLstWdt->currentRow();
		iconPxm.load(dir.relativeFilePath(fileName));
		iconPxm = iconPxm.scaled(QSize(30, 30));
		iconLbl->setPixmap(iconPxm);
		_menuActions[row].setIcon(dir.relativeFilePath(fileName));
		removeBtn->setEnabled(true);
	}
}

void DlgMenuEdit::on_removeBtn_clicked()
{
	int row = menuEditLstWdt->currentRow();
	_menuActions[row].setIcon("");
	iconPxm = QPixmap();
	iconLbl->setPixmap(iconPxm);
}

void DlgMenuEdit::on_menuEditLed_textChanged(const QString & text)
{
	int row = menuEditLstWdt->currentRow();
	_menuItems[row] = text;
	menuEditLstWdt->currentItem()->setText(text);
}

void DlgMenuEdit::on_fileLed_textChanged(const QString & text)
{
	int row = menuEditLstWdt->currentRow();
	_menuActions[row].setCommand(text);
}

void DlgMenuEdit::on_needsOpenModel_stateChanged(int state)
{
	int row = menuEditLstWdt->currentRow();
	if(state == 2)
		_menuActions[row].setRequiresModel(true);
	else
		_menuActions[row].setRequiresModel(false);
}

void DlgMenuEdit::on_saveModelBefore_stateChanged(int state)
{
	int row = menuEditLstWdt->currentRow();
	if(state == 2)
		_menuActions[row].setRequiresSave(true);
	else
		_menuActions[row].setRequiresSave(false);
}

void DlgMenuEdit::on_paramMenuLed_textChanged(const QString & text)
{
	int row = menuEditLstWdt->currentRow();
	_menuActions[row].setArgs(text);
}

void DlgMenuEdit::on_shortcutCbx_currentIndexChanged(const QString & text)
{
	if(menuEditLstWdt->count()){
		int row = menuEditLstWdt->currentRow();
		QString sc =_menuActions[row].shortcut().toString();
		if (shortcutDisabled.contains(text) && text != sc) {
			QMessageBox qMessage(QMessageBox::Critical,
					     QString("PowerDEVS"),
					     QString
					     ("The shorcut is already selected"));
			qMessage.exec();
			shortcutCbx->setCurrentIndex(shortcutLst.indexOf(sc));
			} else {
				_menuActions[row].setShortcut(text);
				shortcutDisabled.removeOne(sc);
				shortcutDisabled << text;
			}
	}
}

bool DlgMenuEdit::checkMenuValues()
{
	// Check if new menu values are OK.
	if (menuLed->text() == "") {
		QMessageBox qMessage(QMessageBox::Critical,
				     QString("PowerDEVS"),
				     QString("Wrong Menu Name!!!"));
		qMessage.exec();
		return false;
	}
	return true;
}

void DlgMenuEdit::on_fileBtn_clicked()
{
	QDir dir(QCoreApplication::applicationDirPath());
	QString fileName =
	    QFileDialog::getOpenFileName(this, "Select File", "../bin", "All Files (*.*)");
	if (!fileName.isEmpty()) {
		int row = menuEditLstWdt->currentRow();
		_menuActions[row].setCommand(dir.relativeFilePath(fileName));
		fileLed->setText(dir.relativeFilePath(fileName));
	}
}
