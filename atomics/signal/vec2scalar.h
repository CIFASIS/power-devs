//CPP:signal/vec2scalar.cpp
#if !defined vec2scalar_h
#define vec2scalar_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"



class vec2scalar: public Simulator { 
// Declare the state,
// output variables
// and parameters

double sigma,u;

int k;

double y[10];

#define INF 1e20


public:
	vec2scalar(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
