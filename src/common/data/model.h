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

#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <vector>

#include "parameter.h"
#include "graphic.h"

using namespace std;
class Coupled;

class Model {

public:
	Model(Coupled *father, string name = string(), string description =string());
  	enum modelType { ATOMIC, COUPLED } ;
  	virtual ~Model();
	virtual bool isAtomic() = 0;
	virtual bool isCoupled() = 0;
	int inPorts() const { return _inPorts; }; 
	int outPorts() const { return _outPorts; };
	void setInports(int inPorts) { _inPorts = inPorts;};
	void setOutports(int outPorts) { _outPorts = outPorts;};
	Coupled *father() const { return _father; };
	void setFather(Coupled *father) { _father = father; };
  	int parameterCount() const { return _parameters.size(); }
	vector<Parameter *> parameters() { return _parameters; }
	Parameter *parameter(unsigned int p) const;
	void setParameters(vector<Parameter*> parameters) { _parameters = parameters; };
	string name() const { return _name; };
	void setName(string name) { _name = name; };
	string description() const { return _description; };
	void setDescription(string description) { _description = description; };
	int priority() const { return _priority; };
	void setPriority(int priority) { _priority = priority;};
  void setExtra(vector<string> e) { _extra = e; };
  vector<string> getExtra() const { return _extra; };
protected:
	Coupled *_father;
	int _inPorts;
	int _outPorts;
	int _priority;
	string _name;
	string _description;
	vector< Parameter* > _parameters;
  vector<string> _extra;
	modelType _type;
};
#endif
