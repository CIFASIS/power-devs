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

#ifndef ATOMIC_H
#define ATOMIC_H

#include "model.h"
#include <string>
#include <iostream>
using namespace std;

class Atomic: public Model
{
public:
	Atomic(Coupled *father, string name = "", string description = "", string path = ""): Model(father,name,description), _path(path)
  { 
      _type = ATOMIC;
  }
	Atomic(): Model(NULL) {};
  virtual ~Atomic() 
  {
  };
	
  virtual bool isAtomic() { return true; }
  virtual bool isCoupled() { return false; }
	string path() const{ return _path; };
	void setPath(string path) { _path = path; };
	Graphic &graphic() { return _graphic; } ;
	const Graphic const_graphic() const { return _graphic; } ;
	void setGraphics (Graphic graphic) { _graphic=graphic; };
  friend ostream &operator<<(ostream &out, const Atomic &a);
  friend istream &operator>>(istream &in, Atomic &a);
	
private:
	string _path;
	Graphic _graphic;
};

#endif
