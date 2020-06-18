//CPP:qss/qsstools.cpp

//CPP:Qss/pure_integrator.cpp
#if !defined pure_integrator_h
#define pure_integrator_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "qss/qsstools.h"


class pure_integrator: public Simulator { 
// Declare here the state, output
// variables and parameters
double X[10];
double sigma;
double y[10]; //output

#define INF 1e20





public:
	pure_integrator(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
