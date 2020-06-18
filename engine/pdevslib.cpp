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

#include "pdevslib.h"
#include "engine.h"

bool debug=true;
void DEBUG(DebugEvent de,Simulator *s, const char *fmt, ...) 
{
  if (!debug) return;
  va_list listPtr;
  va_start( listPtr, fmt );
  vprintf( fmt, listPtr );
  va_end( listPtr );
}



#ifdef __WINDOWS__
typedef short _stdcall (*inpfuncPtr)(short portaddr);
typedef void _stdcall (*oupfuncPtr)(short portaddr, short datum);
#include "pdevslib.win.cpp"
#else
#ifndef RTAIOS
#include "pdevslib.linux.cpp"
#else
#include "pdevslib.rtai.cpp"
#endif
#endif





