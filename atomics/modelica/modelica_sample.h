//CPP:Modelica/modelica_funcs.cpp

//CPP:modelica/modelica_sample.cpp
#if !defined modelica_sample_h
#define modelica_sample_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "modelica_funcs.h"


class modelica_sample: public Simulator { 
// Declare the state, output
// variables and parameters

double sigma;

double itime,period;
int Index;
int ConditionIndex;

double y[10];



public:
	modelica_sample(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
