//CPP:Modelica/modelica_funcs.cpp

//CPP:modelica/modelica_crossdet.cpp
#if !defined modelica_crossdet_h
#define modelica_crossdet_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "math.h"


class modelica_crossdet: public Simulator { 
// Declare the state, output
// variables and parameters
double sigma,u,mu,pu;
double tcross;

double level,inf;

double y[10];


public:
	modelica_crossdet(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
