//CPP:source/scuaren.cpp
#if !defined scuaren_h
#define scuaren_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"



class scuaren: public Simulator { 
// Declare the state, output
// variables and parameters
double y[10];
double sigma;
double A,F,D;
int lev;


public:
	scuaren(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
