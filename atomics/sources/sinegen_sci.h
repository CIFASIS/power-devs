//CPP:Source/sinegen_sci.cpp
#if !defined sinegen_sci_h
#define sinegen_sci_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "math.h"
#include "string.h"


class sinegen_sci: public Simulator { 
// Declare here the state,
// variables and parameters
double sigma;
double y[10];
double a,f,phi,k,dt,w;
char* Method;










public:
	sinegen_sci(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
