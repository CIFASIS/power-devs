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

#ifndef PARAMETER_H
#define PARAMETER_H

#include <assert.h>
#include <stdio.h>

#include <string>
#include <vector>

#include <iostream>


using namespace std;


class Parameter {

public:
	Parameter() {};
	Parameter(string name): _name(name) {};
	Parameter(string name,string description): _name(name), _description(description) {};
  	virtual ~Parameter() { }
	virtual bool isString() const = 0;
	virtual bool isValue()  const = 0;
	virtual bool isList()   const = 0;
	string name() const { return _name; }
	void setName(string name) { _name = name; };
	string description() const { return _description; };
	void setDescription(string description) { _description = description; };
protected:
	string _name;
	string _description;
};

class StringParameter: public Parameter
{
public:
  StringParameter() {}
	StringParameter(string name, string description, string value): Parameter(name,description), _strValue(value)
	{
	};
	bool isString() const { return true;}
	bool isValue()  const { return false;}
	bool isList()   const { return false;}
  	string strValue() const { return _strValue; }
  	string strEscapedValue() const;  
  	void setValue(string s) { _strValue = s; }
protected:
  	string _strValue;
};

class ValueParameter: public Parameter
{
public:
	ValueParameter(string name, string description, double value): Parameter(name,description), _dValue(value)
	{
	};
	bool isString() const { return false;}
	bool isValue()  const { return true;}
	bool isList()   const { return false;}
  	ValueParameter() {}
  	double value() const { return _dValue; };
  	void setValue(double value) { _dValue = value; };
private:
	double _dValue;
};

class ListParameter: public Parameter
{
public:
	ListParameter(string name, string description, vector<string> list, int selected): 
				Parameter(name,description), 
				_selected(selected),
				_list(list)
	{
	};
	bool isString() const { return false;}
	bool isValue()  const { return false;}
	bool isList()   const { return true;}
  	ListParameter(): _selected(1) {};
  	string value(); 
	vector<string> list() const { return _list; };
	void setList(const vector<string>); 
	unsigned int selected() const { return _selected; };
	void setSelected(int selected) { _selected = selected; };
private:
	unsigned int _selected;
	vector<string>	_list;
};
#endif
