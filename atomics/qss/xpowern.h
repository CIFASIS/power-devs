//CPP:qss/xpowern.cpp
#if !defined xpowern_h
#define xpowern_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "math.h"


class xpowern: public Simulator { 
//states 
double u[10];
double sigma;
int order;

//parameters
int n;


//output
double y[10];

#define INF 1e20


public:
	xpowern(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
