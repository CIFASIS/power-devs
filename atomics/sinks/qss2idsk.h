//CPP:sink/qss2idsk.cpp
#if !defined qss2idsk_h
#define qss2idsk_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "stdio.h"
#include "string.h"


class qss2idsk: public Simulator { 
double Sigma,X,u,mu,tlast;
FILE *FOutput;
double x0,ref,inf;


public:
	qss2idsk(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
