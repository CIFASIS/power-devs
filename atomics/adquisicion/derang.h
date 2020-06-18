//CPP:adquisicion/adquiutil.cpp
//CPP:adquisicion/derang.cpp
#if !defined derang_h
#define derang_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"



class derang: public Simulator { 
// Declare the state,
// output variables
// and parameters

public:
	derang(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
