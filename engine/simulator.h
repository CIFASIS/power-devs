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

#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "types.h"
#include "event.h"
#include "pdevslib.h"

class Coupling;

/*! \brief This class represents a DEVS atomic model. */
class Simulator  
{
public:
	/*! Name of this atomic */
	const char *name;
	/*! Pointer to father coupled. This is used in certain circumstances to notify a change to the parent Coupled. */
	Coupling * father;
	/*! An index inside the fater Coupled */
	int myself;
	/*! Elapsed time from last transition */
	Time e;
	/*! Time of last transition */
	Time tl;
	/*! Time of next transition */
	Time tn;
	/*! Temporary storage to the output event computed in lambda function */
	Event output;

	/*! Empty constructor */
	Simulator() ;
	/*! Constructor with name */
	Simulator(const char *n);
	virtual ~Simulator();
	/*! Internal transition function. This function is virtual so an specific atomic overrides it with it's own implementation */
	virtual void dint(Time)=0;
	/*! Ouput function. This function is virtual so an specific atomic overrides it with it's own implementation */
	virtual Event lambda(Time)=0;
	/*! External transition function. This function is virtual so an specific atomic overrides it with it's own implementation */
	virtual void dext(Event, Time)=0;
	/*! Initialization function. This function is called every time a simulation is started.
	*   This function is virtual so an specific atomic overrides it with it's own implementation */
	virtual void init(Time, ...)=0;
	/*! Time advance. This function must return the time of life of the current state.
	*    This function is virtual so an specific atomic overrides it with it's own implementation */
	virtual double ta(Time)=0;
	/*! Exit function. This function is called every time a simulation is ended.
	*   This function is virtual so an specific atomic overrides it with it's own implementation */
	virtual void exit();
	/*! This function is called internally by the simulation engine */
	void imessage(Coupling*, int, Time);
	/*! This function is called internally by the simulation engine */
	Event lambdamessage(Time); 
	/*! This function is called internally by the simulation engine */
	void dintmessage (Time);
	/*! This function is called internally by the simulation engine */
	void dextmessage(Event, Time);
	/*! This function is called internally by the simulation engine */
	void externalinput(Time, int);
	/*! Returns the name of this atomic */
	const char *getName() { return name; };
};

#endif

