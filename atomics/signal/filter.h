//CPP:signal/filter.cpp
#if !defined filter_h
#define filter_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "math.h"


class filter: public Simulator { 
// Declare the state,
// output variables
// and parameters
double u[10],y[10];
double sigma;
int c;
#define INF 1e20;
public:
	filter(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
