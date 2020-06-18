//CPP:qss/qsstools.cpp

//CPP:qss/command_sampler.cpp
#if !defined command_sampler_h
#define command_sampler_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "qss/qsstools.h"


class command_sampler: public Simulator { 

//states
double sigma;
double u[10];
int order;

//output
double y[10];

#define INF 1e20






public:
	command_sampler(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
