//CPP:source/step.cpp
#if !defined step_h
#define step_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"



class step: public Simulator { 
double Delay;
double S[2];
double Sigma;
double Vfin;
bool Aux;


public:
	step(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
