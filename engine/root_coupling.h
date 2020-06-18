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

#ifndef ROOT_COUPLING_H
#define ROOT_COUPLING_H

#include "coupling.h"
#include "root_simulator.h"

/*! \brief This class is the root of all models */
class RootCoupling : public Coupling  
{
public:
	RootCoupling(char *name): Coupling(name) {};
	RootCoupling()  ;
	virtual ~RootCoupling();	
	/*! rootSim points to the RootSimulator in use */
	RootSimulator *rootSim;
	void changemytn(Time , unsigned  int);
};

#endif
