//CPP:continuous/wsum_sci.cpp
#if !defined wsum_sci_h
#define wsum_sci_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"



class wsum_sci: public Simulator { 
// Declare the state, output
// variables and parameters
double K[8];
double Sigma;
double Xs[8];
double Mxs[8];
double Pxs[8];
double S[10];
int n;
int order;


public:
	wsum_sci(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
