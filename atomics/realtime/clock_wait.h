//CPP:realtime/clock_wait.cpp
#if !defined clock_wait_h
#define clock_wait_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"



class clock_wait: public Simulator { 
// Declare the state, output
// variables and parameters
double sigma;
void *xv;//states
void *y; //output
double itime;
double inf;
char *waitMode;





public:
	clock_wait(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
