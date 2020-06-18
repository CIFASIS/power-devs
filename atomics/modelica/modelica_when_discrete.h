//CPP:Modelica/modelica_funcs.cpp

//CPP:modelica/modelica_when_discrete.cpp
#if !defined modelica_when_discrete_h
#define modelica_when_discrete_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "math.h"
#include "string.h"
#include "modelica_funcs.h"


class modelica_when_discrete: public Simulator { 
// Declare the state, output
// variables and parameters

//states
double u[20],mu[20],pu[20];
double yv;

double sigma;

//parameters
int Inputs,Outputs,Index,lastport;
double inf;

//outputs
double y[10];




public:
	modelica_when_discrete(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
