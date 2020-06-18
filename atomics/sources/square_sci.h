//CPP:source/square_sci.cpp
#if !defined square_sci_h
#define square_sci_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"



class square_sci: public Simulator { 
// Declare the state, output
// variables and parameters
double y[10];
double sigma;
double A,F,D;
int lev;


public:
	square_sci(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
