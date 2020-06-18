//CPP:sink/toscilab.cpp
#if !defined toscilab_h
#define toscilab_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "stdio.h"


class toscilab: public Simulator { 
// Declare the state, output
// variables and parameters
double sigma;
int count;
char *time;
char *signal;


public:
	toscilab(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
