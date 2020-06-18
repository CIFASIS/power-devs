//CPP:modelica/outvars.cpp
#if !defined outvars_h
#define outvars_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"



class outvars: public Simulator { 
// Declare the state,
// output variables
// and parameters
double sigma;
public:
	outvars(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
