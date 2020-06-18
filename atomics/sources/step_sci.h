//CPP:source/step_sci.cpp
#if !defined step_sci_h
#define step_sci_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"



class step_sci: public Simulator { 
double Delay;
double S[10];
double Sigma;
double Vfin;
bool Aux;


public:
	step_sci(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
