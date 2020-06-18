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

#include <list>
#include <iostream>
#include <algorithm>
#include <map>
#include <stdlib.h>
#include <QDebug>

#include "coupled.h"

using namespace std;
Coupled::~Coupled()
{
  for (unsigned int j=0;j<_childs.size();j++)
    delete _childs[j];
  for (unsigned int j=0;j<_lines.size();j++)
    delete _lines[j];
  for (unsigned int j=0;j<_points.size();j++)
    delete _points[j];
  for (unsigned int j=0;j<_ports.size();j++)
    delete _ports[j];
}

bool Coupled::isCoupled() 
{
 return true;
}

bool Coupled::isAtomic() 
{
 return false; 
}

void printDebugLines(Coupled &c)
{
  cout <<  "Coupled" << endl;
  cout <<  "{" << endl;
	for(int point=0;point<c.pointCount();point++)
  {
		cout << "Point " << point+1 << endl;
    cout << *c.pointAt(point);
  }
  for(int line=0;line<c.lineCount();line++)
  {
		cout << "Line " << line+1 << endl;
    cout << *c.lineAt(line);
  }
  cout << "}" << endl;
}

void Coupled::addChild(Model *child) 
{ 
  _childs.push_back(child); 
}

void Coupled::removeChild(Model *child) 
{ 
  unsigned int j = childIndex(child);
  _childs.erase(_childs.begin()+j);
  for (unsigned int i=0;i<lineCount();i++) {
	  Line *l=lineAt(i);	
	  if (l->sinkType()==Line::COMPONENT) {
      vector<int> s=l->sinks();
		  if (s[0]>j) {
        s[0]=s[0]-1;
				l->setSink(s);
			}
		} 
		if (l->sourceType()==Line::COMPONENT) {
		  vector<int> s=l->sources();
		  if (s[0]>j) {
		    s[0]=s[0]-1;
			  l->setSources(s);
			}
		} 
	}
}

unsigned int Coupled::childIndex(Model *child) { 
    unsigned int j = 0;
    for (;j<_childs.size();j++)
    {
      if (_childs.at(j)==child)
      {
        return j;
      }
    }
    return 0;
};
 
void Coupled::addInport(Port *p) 
{ 
  p->setPortNumber(_inPortsLst.size()); 
  p->setCoupledPort(_inPortsLst.size()+1); 
  _inPortsLst.push_back(p); 
  _ports.push_back(p); 
  setInports(_inPortsLst.size());
}

void Coupled::removeInport(Port *p) 
{ 
  int removing=p->portNumber(); 
  _inPortsLst.erase (_inPortsLst.begin()+p->portNumber()); 
  vector<Port*>::iterator i;
  int port=0;
  for (i=_inPortsLst.begin();i!=_inPortsLst.end();i++,port++)
  {
    (*i)->setPortNumber(port);
  }
  port=0;
  for (i=_ports.begin();i!=_ports.end();i++,port++)
    if ((*i)==p) break;
  _ports.erase(_ports.begin()+port);
  port++;
  setInports(_inPortsLst.size());
  for (unsigned int i=0;i<lineCount();i++) {
   		Line *l=lineAt(i);	
  		if (l->sinkType()==Line::PORT) {
   			vector<int> s=l->sinks();
	  		if (s[0] > port) {
	    		s[0]=s[0]-1;
		  		l->setSink(s);
			}
		} 
		if (l->sourceType()==Line::PORT) {
    		vector<int> s=l->sources();
	  		if (s[0] > port) {
	    		s[0]=s[0]-1;
		  		l->setSources(s);
	  		}
		} 
	}
  if (father()!=NULL) {
    int myself=father()->childIndex(this);
    for (int i=0;i<father()->lineCount();i++) {
      Line *l=father()->lineAt(i);
      cout << *l;
      if (l->sourceType()==Line::COMPONENT && l->sources()[0]==myself+1 && l->sources()[1]>removing && l->sources()[2]==-1) {
        vector<int> s=l->sources();
        s[1]=s[1]-1;
        l->setSources(s);
      }
      if (l->sinkType()==Line::COMPONENT && l->sinks()[0]==myself+1 && l->sinks()[1]>removing && l->sinks()[2]==-1) {
        vector<int> s=l->sinks();
        s[1]=s[1]-1;
        l->setSink(s);
      }
    }
  }
}

void Coupled::addOutport(Port *p) 
{ 
  p->setPortNumber(_outPortsLst.size());
  p->setCoupledPort(_outPortsLst.size()+1); 
  _outPortsLst.push_back(p); 
  _ports.push_back(p); 
  setOutports(_outPortsLst.size());
}

void Coupled::removeOutport(Port *p) 
{ 
  int removing = p->portNumber();
	_outPortsLst.erase (_outPortsLst.begin()+p->portNumber()); 
  vector<Port*>::iterator i;
  int port=0;
  for (i=_outPortsLst.begin();i!=_outPortsLst.end();i++,port++)
    (*i)->setPortNumber(port);
  port=0;
  for (i=_ports.begin();i!=_ports.end();i++,port++)
	  if ((*i)==p) break;
  _ports.erase(_ports.begin()+port);
  port++;
  setOutports(_outPortsLst.size());
	for (unsigned int i=0;i<lineCount();i++) {
   		Line *l=lineAt(i);	
  		if (l->sinkType()==Line::PORT) {
   			vector<int> s=l->sinks();
	  		if (s[0] > port) {
	    		s[0]=s[0]-1;
		  		l->setSink(s);
			}
		} 
		if (l->sourceType()==Line::PORT) {
    		vector<int> s=l->sources();
	  		if (s[0] > port) {
	    		s[0]=s[0]-1;
		  		l->setSources(s);
	  		}
		} 
	}
  if (father()!=NULL) {
    int myself=father()->childIndex(this);
    for (int i=0;i<father()->lineCount();i++) {
      Line *l=father()->lineAt(i);
      cout << *l;
      if (l->sourceType()==Line::COMPONENT && l->sources()[0]==myself+1 && l->sources()[1]>removing && l->sources()[2]==0) {
        vector<int> s=l->sources();
        s[1]=s[1]-1;
        l->setSources(s);
      }
      if (l->sinkType()==Line::COMPONENT && l->sinks()[0]==myself+1 && l->sinks()[1]>removing && l->sinks()[2]==0) {
        vector<int> s=l->sinks();
        s[1]=s[1]-1;
        l->setSink(s);
      }
    }
  }

}

int Coupled::portIndex(Port *p) const 
{ 
  for (unsigned int i=0;i<_ports.size();i++) 
    if (_ports[i]==p) return i; 
  return -1;
} 

int Coupled::lineIndex(Line *l) const 
{ 
  for (unsigned int i=0;i<_lines.size();i++) 
    if (_lines[i]==l) return i; 
  return -1;
} 


void Coupled::addPoint(Point *p) 
{ 
  _points.push_back(p);  
}

void Coupled::addLine(Line *l) 
{ 
  _lines.push_back(l); 
}

void Coupled::removeLine(Line *l) 
{ 
  for(unsigned int i=0;i<_lines.size();i++) 
    if (l==_lines[i]) {
      _lines.erase(_lines.begin()+i); 
      break;
    }
}

void Coupled::removePoint(Point *p) 
{ 
  unsigned int removing=pointIndex(p);
  _points.erase(_points.begin()+removing);
  for (int i=0;i<lineCount();i++) {
    Line *l=lineAt(i);	
	  if (l->sinkType()==Line::NODE) {
      vector<int> s=l->sinks();
		  if (s[0]>removing) {
		    s[0]=s[0]-1;
        if (s[0]<=0) {
          cout << l[0] <<  " and removing " << removing  << endl;
        }
			  l->setSink(s);
			}
		} 
		if (l->sourceType()==Line::NODE) {
	    vector<int> s=l->sources();
		  if (s[0]>removing) {
		    s[0]=s[0]-1;
        if (s[0]<=0) {
          cout << l[0] <<  " and removing " << removing  << endl;
        }
			  l->setSources(s);
		  }
		} 
  }
}

int Coupled::pointIndex(Point *p) 
{ 
  for(unsigned int i=0;i<_points.size();i++) 
    if (p==_points[i]) 
      return i; 
  return -1;
}

Model *Coupled::child(int i) const
{ 
  return _childs[i]; 
}

int Coupled::childCount() const
{ 
  return _childs.size(); 
}

void Coupled::simplifyLines()
{
  vector<int> grade;
  vector<list<Line*> > lines;
  bool simplified;
  do {
    simplified=false;
    grade.resize(_points.size());
    lines.resize(_points.size());
    for(unsigned int i=0;i<grade.size();++i)
      grade[i]=0;
    for(unsigned int i=0;i<_lines.size();++i)
    {
      if (_lines[i]->sourceType()==Line::NODE) {
        grade[_lines[i]->sourceAt(0)-1]++;
        lines[_lines[i]->sourceAt(0)-1].push_back(_lines[i]);
      }
      if (_lines[i]->sinkType()==Line::NODE) {
        grade[_lines[i]->sinkAt(0)-1]++;
        lines[_lines[i]->sinkAt(0)-1].push_back(_lines[i]);
      }
    } 
    for(unsigned int i=0;i<grade.size();i++) {
			if (grade[i]==1) {
      	cout << "Node number " << i << " has grade " << grade[i] << endl;
				//printDebugLines(*this);
        Line *l = lines[i].front();
        if (l->sourceType()==Line::NODE && l->sourceAt(0)==i+1) 
          l->setSourceType(Line::CROSS);
        else if (l->sinkType()==Line::NODE && l->sinkAt(0)==i+1) 
          l->setSinkType(Line::CROSS);
        Point *p=_points[i];
        removePoint(p);
				delete p;
        simplified=true;
        i=grade.size();
				updatePoints(false);
				//printDebugLines(*this);
      } else if (grade[i]==2) {
      	cout << "Node number " << i << " has grade " << grade[i] << endl;
				//printDebugLines(*this);
        Line *l1 = lines[i].front();
        Line *l2 = lines[i].back();
				removeLine(l2);
				updatePoints(false);
				bool l1_ends_in_node=true;
				bool l2_ends_in_node=true;
				if (l1->sourceType()==Line::NODE && l1->sourceAt(0)==i+1) 
					l1_ends_in_node=false;
				if (l2->sourceType()==Line::NODE && l2->sourceAt(0)==i+1) 
					l2_ends_in_node=false;
				if (l2_ends_in_node) {
					if (l1_ends_in_node) {
						l1->setSinkType(l2->sourceType());
						l1->setSink(l2->sources());
						for (int j=l2->pointCount()-1;j>=0;j--)     
							l1->addPoint(l2->pointX(j),l2->pointY(j));
					} else {
							l1->setSourceType(l2->sourceType());
							l1->setSources(l2->sources());
						for (int j=l2->pointCount()-1;j>=0;j--)     
							l1->preaddPoint(l2->pointX(j),l2->pointY(j));
					}
				} else {
					if (l1_ends_in_node) {
						l1->setSinkType(l2->sinkType());
						l1->setSink(l2->sinks());
						for (int j=0;j<l2->pointCount();j++)     
							l1->addPoint(l2->pointX(j),l2->pointY(j));
					} else {
							l1->setSourceType(l2->sinkType());
							l1->setSources(l2->sinks());
						for (int j=0;j<l2->pointCount();j++)     
							l1->preaddPoint(l2->pointX(j),l2->pointY(j));
					} 
				} 
				delete l2;
        i=grade.size();
        simplified=true;
      }
    }
  } while (simplified);
  // Finally remove disconnected points
  int offset=0;
  grade.resize(_points.size());
  lines.resize(_points.size());
  for(unsigned int i=0;i<grade.size();++i)
    grade[i]=0;
  for(unsigned int i=0;i<_lines.size();++i)
  {
    if (_lines[i]->sourceType()==Line::NODE) {
      grade[_lines[i]->sourceAt(0)-1]++;
      lines[_lines[i]->sourceAt(0)-1].push_back(_lines[i]);
    }
    if (_lines[i]->sinkType()==Line::NODE) {
      grade[_lines[i]->sinkAt(0)-1]++;
      lines[_lines[i]->sinkAt(0)-1].push_back(_lines[i]);
    }
  } 
  for(unsigned int i=0;i<grade.size();i++) {
		if (grade[i]==0) {
        Point *p=_points[i-offset];
        removePoint(p);
				delete p;
        offset++;
		}
	}
	//printDebugLines(*this);
}

void Coupled::setChildList(vector<Model*> childs)
{ 
  // Update connections 
  std::map<unsigned,Model*> map;
  std::map<Model*,unsigned> map2;
  if (_childs.size()!=childs.size()) {
    printf("Changing the number of childs wrongly\n");
    exit(-1);
  }
  for (int i=0;i<_childs.size();i++) 
  {
    map.insert(pair<unsigned,Model*>(i,_childs[i]));
    map2.insert(pair<Model*,unsigned>(childs[i],i));
  }
  for (int i=0;i<_lines.size();i++) 
  {
    if (_lines[i]->sourceType()==Line::COMPONENT)
    {
      vector<int> li = _lines[i]->sources();
      li[0] = map2[map[li[0]-1]]+1;
      _lines[i]->setSources(li);
    }
    if (_lines[i]->sinkType()==Line::COMPONENT)
    {
      vector<int> li = _lines[i]->sinks();
      li[0] = map2[map[li[0]-1]]+1;
      _lines[i]->setSink(li);
    }
  
  }
  _childs=childs; 
}

string Coupled::getParameter(string par) 
{ 
  for (unsigned int i=0; i<_parameters.size();++i)
  {
    if (_parameters[i]->name()==par) {
      if (_parameters[i]->isString()) {
        string value =  (dynamic_cast<StringParameter*>(_parameters[i]))->strValue();
        if (value[0]=='%') {
          if (father())
            return father()->getParameter(par); 
          return father()->getParameter(value.substr(1)); 
        }
        return value;
      }
      if (_parameters[i]->isList())
        return (dynamic_cast<ListParameter*>(_parameters[i]))->value();
      double v=(dynamic_cast<ValueParameter*>(_parameters[i]))->value();
      char buff[128];
      sprintf(buff,"%g",v); 
      string s=buff;
      return s;
    }
 }
 if (father())
  return father()->getParameter(par); 
 return "Error looking for parameter";
}

void Coupled::updatePoints(bool checkPoints)
{
  for (unsigned int i=0; i<_points.size();++i)
    _points[i]->clearLines();
  for (unsigned int i=0; i<_lines.size();++i)
  {
    if (_lines[i]->sourceType()==Line::NODE) {
      assert(_points.size()>_lines[i]->sourceAt(0)-1);
      assert(_points[_lines[i]->sourceAt(0)-1]->lineCount()<3);
      _points[_lines[i]->sourceAt(0)-1]->addLine(i+1);
    }
    if (_lines[i]->sinkType()==Line::NODE) {
      assert(_points.size()>_lines[i]->sinkAt(0)-1);
      assert(_points[_lines[i]->sinkAt(0)-1]->lineCount()<3);
      _points[_lines[i]->sinkAt(0)-1]->addLine(i+1);
    }
  }
  for (unsigned int i=0; i<_points.size();++i)
    if(_points[i]->lineCount()!=3)
      if (checkPoints) {
        cout << *this;
        assert(false);
      }
  for (unsigned int i=0; i<_childs.size();++i)
    if (_childs[i]->isCoupled())
      (dynamic_cast<Coupled*>(_childs[i]))->updatePoints();
}

bool sortPredicate(const Model *c1, const Model  *c2)
{
  return c1->priority()< c2->priority();
}

void Coupled::sortChilds()
{
  std::sort(_childs.begin(), _childs.end(),sortPredicate);
}

void Coupled::clearModel()
{
  _childs.clear();
  _lines.clear();
  _points.clear();
  _ports.clear();
}
