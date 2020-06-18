//CPP:sink/qss3idsk.cpp
#if !defined qss3idsk_h
#define qss3idsk_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "stdio.h"
#include "string.h"


class qss3idsk: public Simulator { 
double Sigma,X,u,mu,pu;
double tlast;
FILE *FOutput;
double x0,ref,inf;


public:
	qss3idsk(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
