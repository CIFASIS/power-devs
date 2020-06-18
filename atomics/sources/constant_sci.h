//CPP:sources/constant_sci.cpp
#if !defined constant_sci_h
#define constant_sci_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"



class constant_sci: public Simulator { 
double S[10];
double Sigma;




public:
	constant_sci(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
