//CPP:continuous/pintegrator_sci.cpp
#if !defined pintegrator_sci_h
#define pintegrator_sci_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "math.h"
#include "string.h"


class pintegrator_sci: public Simulator { 
// Declare here the state, output
// variables and parameters
double u,mu,pu,X;
double inf;
double sigma;
double y[10]; //output




public:
	pintegrator_sci(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
