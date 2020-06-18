//CPP:hybrid/hystcomp.cpp
#if !defined hystcomp_h
#define hystcomp_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"



class hystcomp: public Simulator { 
// Declare the state, output
// variables and parameters
double sigma,u,mu;
int prevst;
double y[2];
double swon,swoff,outon,outoff;
double inf;





public:
	hystcomp(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
