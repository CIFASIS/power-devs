//CPP:sink/ev_count.cpp
#if !defined ev_count_h
#define ev_count_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "stdio.h"
#include "string.h"


class ev_count: public Simulator { 
double Sigma;
FILE *FOutput;
double k[20];
int n;
int saved;



public:
	ev_count(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
