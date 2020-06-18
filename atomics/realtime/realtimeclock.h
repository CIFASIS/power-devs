//CPP:realtime/realtimeclock.cpp
#if !defined realtimeclock_h
#define realtimeclock_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"



class realtimeclock: public Simulator { 
// Declare the state, output
// variables and parameters
double sigma;
double y[10];
double dblRealTime;



public:
	realtimeclock(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
