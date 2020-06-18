//CPP:source/pwmsignal.cpp
#if !defined pwmsignal_h
#define pwmsignal_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"



class pwmsignal: public Simulator { 
// Declare here the state, output
// variables and parameters
double sigma;
int j;
double a;
double freq;
double dt[64];
double y[10];



public:
	pwmsignal(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
