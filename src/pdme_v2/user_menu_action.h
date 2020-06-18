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

#ifndef USER_MENU_ACTION_H
#define USER_MENU_ACTION_H

#include <QFileInfo>

class UserMenuAction: public QObject
{
public:
	UserMenuAction(QStringList data): QObject()
	{
		_command = data.at(0);
		_args = data.at(1);
		_shortcut = QKeySequence(data.at(2));
		_icon = data.at(3);
		_must_have_a_model = data.at(4) == "1";
		_must_save_before_running = data.at(5) == "1";
                if (data.size()>6)
                  _runInFolder = data.at(6);
	}
	UserMenuAction(const UserMenuAction &ua): QObject()
	{
		QStringList data = ua.getData();
		_command = data.at(0);
		_args = data.at(1);
		_shortcut = QKeySequence(data.at(2));
		_icon = data.at(3);
		_must_have_a_model = data.at(4) == "1";
		_must_save_before_running = data.at(5) == "1";
                if (data.size()>6)
                  _runInFolder = data.at(6);
	}
	UserMenuAction operator=(UserMenuAction ua)
	{
		UserMenuAction ret(ua);
		return ret;
	}
	
	bool hasShortcut() { return ! _shortcut.isEmpty(); }
        bool hasIcon()
        {
            QFileInfo qf(_icon);
            return ! _icon.isEmpty() && qf.exists();
        }
	QString icon() { return _icon; }
	QKeySequence shortcut() { return _shortcut; }
  	QString command() { return _command; }
  	bool requiresModel() { return _must_have_a_model; };
  	bool requiresSave() { return _must_save_before_running; };
  	QString args() { return _args; };
        QString runInFolder() { return _runInFolder; };
	QStringList getData() const
	{
		QStringList _ret;
		_ret << _command;
		_ret << _args;
		_ret << _shortcut.toString();
		_ret << _icon; 
		if(_must_have_a_model)
			_ret << "1";
		else
			_ret << "0";
		if(_must_save_before_running)
			_ret << "1";
		else
			_ret << "0";
                _ret << _runInFolder;
		return _ret;
	};
	void setCommand(QString cmd) { _command = cmd; };
	void setArgs(QString arg) { _args = arg; };
	void setShortcut(QString shc) { _shortcut = QKeySequence(shc); };
	void setIcon(QString icon) { _icon = icon; };
	void setRequiresModel(bool rm) { _must_have_a_model = rm; };
	void setRequiresSave(bool rs) { _must_save_before_running = rs; };
        void setRunInFolder(QString folder) { _runInFolder = folder; };
private:
	QString _command;
	QString _args;
	QKeySequence _shortcut;
	QString _icon;
	bool _must_have_a_model;
	bool _must_save_before_running;
        QString _runInFolder;
};

#endif
