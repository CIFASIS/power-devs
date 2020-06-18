//CPP:qss/qsstools.cpp

//CPP:qss/qss_switch.cpp
#if !defined qss_switch_h
#define qss_switch_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "qss/qsstools.h"


class qss_switch: public Simulator { 
//states
double sigma,tcross;
double u[3][10];
int sw,change;

//output
double y[10];

//parameters
double level;

#define INF 1e20













public:
	qss_switch(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
