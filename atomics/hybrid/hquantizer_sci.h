//CPP:hybrid/hquantizer_sci.cpp
#if !defined hquantizer_sci_h
#define hquantizer_sci_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "math.h"


class hquantizer_sci: public Simulator { 
// Declare the state, output
double y[10];
double u,mu,pu;
double sigma;
// variables and parameters
double xl,xu;
double bal,dq,inf;


public:
	hquantizer_sci(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
