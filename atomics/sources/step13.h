//CPP:source/step13.cpp
#if !defined step13_h
#define step13_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"



class step13: public Simulator { 
 double Delay;
double S[10];
double Sigma;
double Vfin;
bool Aux;


public:
	step13(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
