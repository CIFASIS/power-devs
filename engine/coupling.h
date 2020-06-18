/****************************************************************************
**
**  Copyright (C) 2009 Facultad de Ciencia Exactas Ingeniería y Agrimensura
**		       Universidad Nacional de Rosario - Argentina.
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

#ifndef COUPLING_H
#define COUPLING_H

#include <list>
#include <vector>

#include "simulator.h"
#include "event.h"
#include "connection.h"

struct ModelTime 
{
	unsigned int model;
	double tn;
};

class ChildHeap
{
public:
	void create(Simulator **, unsigned int);
	void update(unsigned int);
	int next();
	void dump();

private:
	std::vector<std::vector<ModelTime> > heap;
	Simulator **D;
	unsigned int size;
};

/*! \brief This class represents a Coupled DEVS model
* 
*  A Coupled DEVS model has a number DSize of child models (D),
*  ICSize internal connections (IC), EICSize external input connections (EIC),
*  EOCSize external output connections (EOC).
*
*/
class Coupling:public Simulator  
{
private:
	std::vector<Simulator*> childs;
	std::vector<std::vector<unsigned int> > IC_Index;
public:
	ChildHeap heap;
	//*! Array of pointers to childs models */
	Simulator ** D;
	/*! Array of pointers to internal connections */
	Connection** IC;
	/*! Array of pointers to external input connections */
	Connection** EIC;
	/*! Array of pointers to external ouput connections */
	Connection** EOC;
	//*! Number of childs */
	unsigned int Dsize;
	/*! Number of internal connections */
	unsigned int ICsize;
	/*! Number of external input connections */
	unsigned int EICsize;
	/*! Number of external output connections */
	unsigned int EOCsize;
	//! transitionChild is an index indicating which is the child with minumu advance time
	unsigned int transitionChild;

	//! Empty constructor
	Coupling() ;
	//! Constructor with name initialization
	Coupling(const char *name);
	~Coupling();
	void init(Time t, ...);
	void exit();
	void dint(Time);
	/*! Propagates an event through internal connections and external output connections */
	void propagate(Event, Time);
	Event lambda(Time);
	void dext(Event, Time);
	double ta(Time);
	/*! Initializes D, DSize, IC, ICSize,  EIC, EICSize, EOC, EOCSize */
	void setup(Simulator**, int, Connection**, int, Connection**, int, Connection**, int);
	/*! Initializes D, DSize, IC, ICSize */
	void setup(Simulator**, int, Connection**, int);
	/*! This function is called when a child model change its advance time */
	virtual	void changemytn(Time, unsigned int);
};

#endif
