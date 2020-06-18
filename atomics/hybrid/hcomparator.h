//CPP:hybrid/hcomparator.cpp
#if !defined hcomparator_h
#define hcomparator_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "math.h"


class hcomparator: public Simulator { 
// Declare the state, output
// variables and parameters
double sigma,u[2],mu[2],pu[2];
double tcross;
int sw;
double Vu, Vl;
double inf;

double y[10];


public:
	hcomparator(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
