//CPP:sources/trapezoidal_sci.cpp
#if !defined trapezoidal_sci_h
#define trapezoidal_sci_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "string.h"


class trapezoidal_sci: public Simulator { 
// Declare the state, output
// variables and parameters
double sigma;
int k;
double dq;
double v[4],mv[4];
double tv[4];
double Vu,Vl,Tu,Tl,Tr,Tf;
double y[10];
char* Method;



public:
	trapezoidal_sci(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
