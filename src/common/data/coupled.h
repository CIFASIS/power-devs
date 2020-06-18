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

#ifndef COUPLED_H
#define COUPLED_H

#include <vector>
#include <iostream>

#include "model.h"
#include "atomic.h"
#include "point.h"
#include "port.h"
#include "line.h"
#include "connection.h"

using namespace std;

class Coupled: public Model
{
public:
	Coupled(): Model(NULL)
	{
	};
  virtual ~Coupled();
  virtual bool isCoupled(); 
  virtual bool isAtomic(); 
	void addChild(Model *child);
	void removeChild(Model *child);
	unsigned int childIndex(Model *child);
  void addInport(Port *p);
  void removeInport(Port *p);
  void addOutport(Port *p);
  void removeOutport(Port *p);
	Model *child(int i) const ;
	int childCount() const ;
  Graphic &graphic() { return _graphic; }
  void setGraphic(Graphic graphic) { _graphic=graphic; }
  Graphic const_graphic() const { return _graphic; }
  int inportCount() const { return _inPortsLst.size(); }
  int outportCount() const { return _outPortsLst.size(); }
  int portCount() const { return _ports.size(); }
  Port *inport(int i) const { return _inPortsLst[i]; }
  Port *outport(int i) const  { return _outPortsLst[i]; }
  Port *port(int i) const  { return _ports[i]; }
  int portIndex(Port *p) const ;
  int lineIndex(Line *) const ;
  vector<Model*> getChildList() const { return _childs; }
  void setChildList(vector<Model*> childs);
  void setChildAt(Model* child,int i) { _childs[i] = child;}
  friend ostream &operator<<(ostream &out, const Coupled &c);
	void addPoint(Point *p); 
	void addLine(Line *l);
	unsigned int pointCount() const { return _points.size(); };
	Point *pointAt(unsigned int i) const { return _points[i]; }
	unsigned int lineCount() const { return _lines.size(); };
	Line *lineAt(unsigned int i) const { return _lines[i]; }
  void removeLine(Line *l);
  void removePoint(Point *p); 
  int pointIndex(Point *p);
  void simplifyLines();
  string getParameter(string par);
  void updatePoints(bool checkPoints=true);
  void clearModel();
  void sortChilds();
#ifdef USE_CONNECTIONS
  QList<Connection*> lsEIC,lsEOC,lsIC;
#endif
private:
	vector<Model*> _childs;
  vector<Port*> _inPortsLst;
  vector<Port*> _outPortsLst;
  vector<Port*> _ports;
  vector<Point*> _points;
  vector<Line*> _lines;
  Graphic _graphic;
};

#endif
