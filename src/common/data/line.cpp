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

#include <line.h>
#include <assert.h>

void Line::setPointsXY(vector<int> pointX, vector<int> pointY) 
{ 
	_pointX = pointX; 
	_pointY = pointY; 
}	

void Line::clearPoints() 
{ 
	_pointX.clear(); 
	_pointY.clear(); 
}

void Line::addPoint(int x, int y) 
{ 
	_pointX.push_back(x); 
	_pointY.push_back(y); 
}

void Line::preaddPoint(int x, int y) 
{ 
	_pointX.insert(_pointX.begin(),x); 
	_pointY.insert(_pointY.begin(),y); 
}

void Line::setSources(vector<int> sources) 
{ 
	_sources = sources;  
}

void Line::setSink(vector<int> sinks) 
{ 
	_sinks = sinks; 
}
