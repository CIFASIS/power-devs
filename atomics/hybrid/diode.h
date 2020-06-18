//CPP:hybrid/diode.cpp
#if !defined diode_h
#define diode_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"



class diode: public Simulator { 
// Declare the state, output
// variables and parameters
double sigma,u,mu;
double inf;
double y[2];


public:
	diode(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
