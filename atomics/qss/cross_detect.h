//CPP:qss/qsstools.cpp
//CPP:qss/cross_detect.cpp
#if !defined cross_detect_h
#define cross_detect_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "qss/qsstools.h"


class cross_detect: public Simulator { 
//states
double sigma;
double u[10];
int sw;

//output
double y[10];

//parameters
double level,outval;

#define INF 1e20





public:
	cross_detect(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
