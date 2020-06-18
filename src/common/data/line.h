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

#ifndef LINE_H
#define LINE_H

#include <vector>
#include <iostream>

using namespace std;

class Line {
public:
	typedef	enum { PORT, NODE, CROSS, COMPONENT }PointType ;
	void setSourceType(PointType source) { _sourceType = source; };
	void setSinkType(PointType sink) { _sinkType = sink; };
	void setPointsXY(vector<int>, vector<int>);	
	void setSources(vector<int>);
	void setSink(vector<int>);
	vector<int> sinks() { return _sinks; };
	vector<int> sources() { return _sources; };
	int sinkAt(unsigned int i) const { return _sinks[i]; }
	int sourceAt(unsigned int i) const { return _sources[i]; }
	unsigned pointCount() const { return _pointX.size(); }
	int pointX(unsigned i) const { return _pointX[i]; }
	int pointY(unsigned i) const { return _pointY[i]; }
	PointType sinkType() const { return _sinkType; }
	PointType sourceType() const { return _sourceType; }
  	void clearPoints();
  	void addPoint(int, int);
  	void preaddPoint(int, int);
  	friend ostream &operator<<(ostream &out, const Line &l);
private:
	PointType _sourceType;
	PointType _sinkType;
	vector< int > _sources;
	vector< int > _sinks;
	vector< int > _pointX;
	vector< int > _pointY;
};
#endif
