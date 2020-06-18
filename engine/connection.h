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

#ifndef CONNECTION_H
#define CONNECTION_H

#include "types.h"

/*! \brief This class represents the connection of two DEVS models through ports.
* 
*  A connection object connets port sourcePort of the childSource model
*  with the sinkPort of the childSink model
*
*/
class Connection{
public:
	/*! Index to source child */
	unsigned int childSource;
	/*! Port number in source child */
	Port sourcePort;
	/*! Index to sink child */
	unsigned int childSink;
	/*! Port number in sink child */
	Port sinkPort;
	//! This method is called to initialize the values
	void setup(int chldSrc, Port srcPrt, int chldSnk, Port snkPrt);
};

#endif
