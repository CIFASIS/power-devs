//CPP:Qss/xpower2.cpp
#if !defined xpower2_h
#define xpower2_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "math.h"


class xpower2: public Simulator { 
//states
double u[10];
double sigma;
int order;

//output
double y[10];

#define INF 1e20




public:
	xpower2(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
