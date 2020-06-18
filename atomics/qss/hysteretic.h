//CPP:qss/qsstools.cpp

//CPP:qss/hysteretic.cpp
#if !defined hysteretic_h
#define hysteretic_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "qss/qsstools.h"


class hysteretic: public Simulator { 
//states
double sigma,tcross;
double u[10];
int sw;

//output
double y[10];

//parameters
double ud[2],yd[2];


#define INF 1e20





















public:
	hysteretic(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
