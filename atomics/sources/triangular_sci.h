//CPP:source/triangular_sci.cpp
#if !defined triangular_sci_h
#define triangular_sci_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "string.h"


class triangular_sci: public Simulator { 
// Declare the state, output
double sigma;
double y[10];
// variables and parameters
double a,f,dQ,pte;
double sgn;
char* Method;
double state;


public:
	triangular_sci(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
