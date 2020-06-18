//CPP:source/triangular.cpp
#if !defined triangular_h
#define triangular_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "string.h"


class triangular: public Simulator { 
// Declare the state, output
double sigma;
double y[10];
// variables and parameters
double a,f,dQ,pte;
double sgn;
char* Method;
double state;


public:
	triangular(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
