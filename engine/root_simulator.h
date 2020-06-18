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

#ifndef ROOT_SIMULATOR_H
#define ROOT_SIMULATOR_H

#include <stdlib.h>
#include "types.h"
#include "coupling.h"
#include "pdevslib.h"

extern Time realTi;
extern bool overRun;
extern int exitStatus;
extern double realTiSimulation;

/*! \brief The RootSimulator class is used to simulate a DEVS model */
class RootSimulator
{
	bool state;
	const char *name;
	Time lastTransition;
	int stepsCount;
	int maxSteps;
public:
	bool timed;
	Time ti;
	Time tf;
	Time t;
	RootSimulator(const char *name): name(name) {timed=false;};
	Coupling *mainCoupling;
	/*! This function tells how much of the simulation has been completed */
	int  percentageDone() { return int((t/tf)*100); };
	/*! Set the final time */
	void setFinalTime(Time t) { tf=t; };
	/*! Set the initial time, by default 0 */
	void setInitialTime(Time t) { ti=t; };
	/*! Sets if the simulation should synchronize all events */ 
	void setRealTime() { timed=true; };
	/*! Sets the simulation break count */ 
	void setBreakCount (int b) { maxSteps=b; };
	void init();
	/*! Performs a simulation step */
	bool step();
	/*! Performs 'N' simulations steps */
	bool step(int N);
	/*! Runs the simulation until the final time is reached */
	void run();
	void exit();
	void changemytn(Time);
  int getExitStatus() { return exitStatus; }
};

 #endif
