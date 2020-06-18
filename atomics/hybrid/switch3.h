//CPP:hybrid/switch3.cpp
#if !defined switch3_h
#define switch3_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "math.h"


class switch3: public Simulator { 
// Declare the state, output
// variables and parameters
double sigma,u[3],mu[3],pu[3];
double tcross;
int sw,ch;

double level,inf;

double y[10];


public:
	switch3(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
