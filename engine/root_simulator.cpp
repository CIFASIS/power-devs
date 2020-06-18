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


#include "root_simulator.h"
#include "pdevslib.h"
#include <stdio.h>
#include <stdlib.h>

Time realTi;
bool overRun;
int exitStatus=0;
double realTiSimulation;

void RootSimulator::init(){
	t=ti;
	printLog("Simulation Initialized\n");
	mainCoupling->init(t);
	mainCoupling->imessage(0,0,t);
	mainCoupling->ta(t);
	realTi = getTime();
	lastTransition=0;
	stepsCount=0;
  	initLib();
#ifdef RTAIOS
	enterRealTime();
#endif
}

bool RootSimulator::step(){
		t=mainCoupling->tn;
		if (lastTransition==t)
		{
			stepsCount++;
		} else
		{
			lastTransition=t;
			stepsCount=0;
		}
		if (stepsCount>maxSteps) {
			printf("Illegitimate model. Aborting simulation\n");
			fflush(stdout);
			exit();
			return true;
		}
		if (t<=tf) {
 			Event e = mainCoupling->lambdamessage(t);
			if (!e.isInterrupted()) {
				mainCoupling->dintmessage(t);
			}
		    return false;
		}
		else {
			exit();
			return true;
		};
}

void RootSimulator::exit() 
{
#ifdef RTAIOS
	leaveRealTime();
#endif
	mainCoupling->exit();
	printLog("Simulation Ended (%.3g sec)\n",getTime()-realTi);
	cleanLib();
}

bool RootSimulator::step(int s) {
	int i;
	Time temp=getRealSimulationTime();
	
	for (i=0;i<s;i++) {
	  if (getRealSimulationTime()-temp>0.5) 
		  return false;
		if (timed) {
			waitFor(t-getRealSimulationTime(),REALTIME);
		}
		if (step()) 
			return true;

	}
	return false;
}

void RootSimulator::run() {
	while (!step(1)) ;
}
void RootSimulator::changemytn(Time tnext){
	DEBUG(TA,(Simulator*)this,"Changing tn at RootSimulator %g\n",tnext);
	t=tnext;
	mainCoupling->ta(t);
};
