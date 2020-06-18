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

#ifndef PORT_H
#define PORT_H

#include <vector>
#include <iostream>

using namespace std;

class Port {
public:
  	Port() {}
  	Port(int type, string name, string desc, Graphic g):
    	_type(type),
    	_name(name),
    	_desc(desc),
    	_graphic(g),
    	_portNumber(0)
  	{
  	}
  	int type() const { return _type; }
  	void setType(int type) { _type = type; };
  	string name() const { return _name; }
  	string desc() const { return _desc; }
  	unsigned portNumber() { return _portNumber; }
  	Graphic &graphic() { return _graphic; }
  	Graphic const_graphic() const { return _graphic; }
  	void setPortNumber(unsigned p) { _portNumber = p; }
  	void setName(string name) { _name = name; }
  	unsigned coupledPort() const { return _coupledPort; }
  	void setCoupledPort(unsigned p) { _coupledPort=p; }
  	enum { Inport, Outport} portType;
  	friend ostream &operator<< (ostream &out, const Port&);
  	friend istream &operator>> (istream &in, Port&);
private:
	int _type;
	string _name;
	string _desc;
	Graphic _graphic;
  	unsigned _portNumber;
  	unsigned _coupledPort;
};
#endif
