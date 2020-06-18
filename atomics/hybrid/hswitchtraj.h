//CPP:hybrid/hswitchtraj.cpp
#if !defined hswitchtraj_h
#define hswitchtraj_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "math.h"


class hswitchtraj: public Simulator { 
// Declare the state, output
// variables and parameters
double sigma,u[4],mu[4],pu[4];
double tcross;
int sw;

double inf;
int ch;
double y[10];


public:
	hswitchtraj(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
