//CPP:qss/qss_gain.cpp
#if !defined qss_gain_h
#define qss_gain_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"



class qss_gain: public Simulator { 
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
	qss_gain(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
