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

#include <point.h>

void Point::removeLine(int line)
{
	for (unsigned int i=0;i<_lines.size();i++) {
    	if (_lines[i]==line) {
        	_lines.erase(_lines.begin()+i);
        	_extrems.erase(_extrems.begin()+i);
        	break;
      	}
   	}
}

void Point::clearLines()
{
	_extrems.clear();
    _lines.clear();
}
  
void Point::addLine(int line)
{
	_lines.push_back(line);
    _extrems.push_back(line);
}
