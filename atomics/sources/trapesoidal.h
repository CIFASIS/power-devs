//CPP:source/trapesoidal.cpp
#if !defined trapesoidal_h
#define trapesoidal_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"



class trapesoidal: public Simulator { 
// Declare the state, output
// variables and parameters
double sigma;
int k;
double dq;
double v[4],mv[4];
double tv[4];
double Vu,Vl,Tu,Tl,Tr,Tf;
double y[10];
char* Method;

public:
	trapesoidal(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
