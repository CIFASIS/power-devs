//CPP:hybrid/hquantizer.cpp
#if !defined hquantizer_h
#define hquantizer_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "math.h"


class hquantizer: public Simulator { 
// Declare the state, output
double y[10];
double u,mu,pu;
double sigma;
// variables and parameters
double xl,xu;
double bal,dq,inf;


public:
	hquantizer(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
