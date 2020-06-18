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

#ifndef EVENT_H
#define EVENT_H

#include "types.h"

/*! En enumeratd type telling what kind of synchronization must be used for an event */
typedef enum { NOREALTIME, REALTIME, PRECISEREALTIME } RealTimeMode;

/*! \brief This class represents an output event, result of evaluating the lambda function of a DEVS model*/
class Event
{
	/*! Tells which synchronization should be used for this event */
	RealTimeMode mode;
public:
	/*! The value carried by an event is defined to be a void pointer for flexibility.
	*   Most of the blocks in the library (by convention) use this value pointing to a double array.
	*   Therefore an input value can be retrieved with:
	*	double *v = (double*) event.value;
	*
	*	v[0] is the value 
	*	v[1] is the first derivate
	*/
	void *value;
	/*!  The port value is used to know from which port the event came from */
	Port port;
	/*!  This bool value is used in the RTAI distribution. When set, indicates that an external input event
	*    ocurred while synchronzing this event, then, it should not be propagated. */
	bool interrupted;
	Event();
	Event( void*, Port);
	virtual ~Event();

	/*! A null event is defined as one with value=NULL. */
	void setNullEvent();
	bool isNotNull();

	void setRealTimeMode(RealTimeMode m) { mode=m; };
	RealTimeMode getRealTimeMode() { return mode; };
	/*! Retrieves the i'th double value of this event */
	double getDouble(int i) { return ((double*)value)[i]; };
	/*! Retrieves the first double value of this event */
	double getDouble() { return getDouble(0); };	
	/*! Retrieves the first int value of this event */
	double getInt()	  { return ((int*)value)[0]; };
	void   setDouble(double &v) { value=&v; };
	void   setInt(int &v) { value=&v; };
	/*!  This is used internally by the simulation engine to notify an external input event */
	void setInterrupted() { interrupted=true; }
	bool isInterrupted() { return interrupted; }
};

#endif
