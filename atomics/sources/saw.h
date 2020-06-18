//CPP:source/saw.cpp
#if !defined saw_h
#define saw_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "string.h"


class saw: public Simulator { 
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
	saw(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
