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

#ifndef POINT_H
#define POINT_H

#include <vector>
#include <iostream>
#include <graphic.h>

using namespace std;

class Point {
public:
	Graphic &graphic() { return _g; };
	Graphic const_graphic() const { return _g; };
	void setLines(vector<int> lines) { _lines=lines; }
	void setExtrems(vector<int> extrems) { _extrems=extrems; }
	int lineCount() const { return _lines.size(); }
	int lineAt(unsigned i) const { return _lines[i]; }
  	vector<int> &lines() { return _lines; }
	int extremsCount() const { return _extrems.size(); }
	int extremsAt(unsigned i) const { return _extrems[i]; }
	void removeLine(int);
	void clearLines();
	void addLine(int);
  	friend ostream &operator<<(ostream &out, const Point &p);
private:
	vector< int > _lines;
	vector< int > _extrems;
	Graphic _g;
};
#endif
