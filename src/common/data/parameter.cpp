/****************************************************************************
**
**  Copyright (C) 2009 Facultad de Ciencia Exactas Ingeniería y Agrimensura
**										 Universidad Nacional de Rosario - Argentina.
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

#include <parameter.h>

string StringParameter::strEscapedValue() const 
{ 
    string s(_strValue);
    int pos=0;
    while ((pos=s.find(";",pos)) != string::npos)
    {
      s.insert(pos,"\\");
      pos+=2;
    }
    return s;
}

string ListParameter::value() 
{ 
	assert(_selected<=_list.size());
	return _list[_selected-1];
}

void ListParameter::setList(const vector<string> list) 
{ 
	_list = list; 
	if (_selected>_list.size()) 
		_selected=1;
}
