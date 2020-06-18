//CPP:source/saw_sci.cpp
#if !defined saw_sci_h
#define saw_sci_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "string.h"


class saw_sci: public Simulator { 
// Declare the state, output
// variables and parameters
double A,F,dq;
double y[10];
double S[2];
char* pend;
char* Method;
double sigma;
double pte;



public:
	saw_sci(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
