//CPP:continuous/gain.cpp
#if !defined gain_h
#define gain_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"



class gain: public Simulator { 
//states
double sigma;
double u[10];
int order;

//output
double y[10];

//parameters
double K;

#define INF 1e20



public:
	gain(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
