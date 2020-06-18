//CPP:qss/qsstools.cpp

//CPP:qss/qss_saturation.cpp
#if !defined qss_saturation_h
#define qss_saturation_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "qss/qsstools.h"


class qss_saturation: public Simulator { 
//states
double sigma,tcross;
double u[10];
int sw;

//output
double y[10];

//parameters
double xl,xu;

#define INF 1e20













public:
	qss_saturation(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
