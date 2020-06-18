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

#include "simulator.h"
#include "coupling.h"
#include "engine.h"
#include "pdevslib.h"
#include "root_simulator.h"

Simulator::Simulator(const char *name)
{
	Simulator::name = name;

}

Simulator::Simulator()
{

}
Simulator::~Simulator()
{

}

void Simulator::imessage(Coupling *f, int my, Time t){
	myself=my;
	father=f;
	e=0;
	tl=t;
	tn=t+ta(t);
	DEBUG(INIT,this,"[%s] Execution Init Function at %g\n",name,t);
}


Event Simulator::lambdamessage(Time t){
	Event out=lambda(t);
	DEBUG(LAMBDA,this,"[%s] Execution Output Function at %g\n",name,t);
	int ret = 0;
	if (out.getRealTimeMode()) {
		ret = waitFor(t-getRealSimulationTime(),out.getRealTimeMode());	 
	}

	if (ret == -1) { // In IRQ has happend
	  DEBUG(LAMBDA,this,"[%s] IRQ Detected %g\n",name,t);
	  out.setNullEvent();
	  out.setInterrupted();
	}
	return out;
}

void Simulator::dintmessage(Time t){
	DEBUG(DINT,this,"[%s] Execution Internal Transition at %g\n",name,t);
	e=t-tl;
	dint(t);
	tl=t;
	tn=t+ta(t);
}

void Simulator::dextmessage(Event x, Time t){
	DEBUG(DEXT,this,"[%s] Execution External Transition at %g\n",name,t);
	e=t-tl;
	dext(x, t);
	tl=t;
	tn=t+ta(t);
}


void Simulator::externalinput(Time t, int irq){
	DEBUG(DEXT,this,"[%s] Execution External Input at %g\n",name,t);
	Event x;
	x.port = -1;
	x.value = (void*)irq;
	e = t - tl;
	dext(x,t);
	tl=t;
	tn=t+ta(t);
	father->changemytn(tn,myself);
	DEBUG(DEXT,this,"[%s] Next event is at %g\n",name,tn);
};

void Simulator::exit(){

};



