//CPP:qss/qsstools.cpp

//CPP:Qss/samplehold.cpp
#if !defined samplehold_h
#define samplehold_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "qss/qsstools.h"


class samplehold: public Simulator { 
//parameters
double T,phi;

//states
double sigma;
double u[10];
int order;

//output
double y[10];





public:
	samplehold(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
