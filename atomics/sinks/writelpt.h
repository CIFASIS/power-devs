//CPP:sink/writelpt.cpp
#if !defined writelpt_h
#define writelpt_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"



class writelpt: public Simulator { 
// Declare the state, output
// variables and parameters
double sigma;
double y[10];


public:
	writelpt(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
