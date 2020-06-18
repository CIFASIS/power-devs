//CPP:qss/qsstools.cpp
//CPP:qss/compare.cpp
#if !defined compare_h
#define compare_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "qsstools.h"


class compare: public Simulator { 
//states
double sigma,tcross;
double u[2][10],du[10];
int sw;

//output
double y[10];

//parameters
double low,up;

#define INF 1e20














public:
	compare(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
