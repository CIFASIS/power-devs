//CPP:sink/ev_count2.cpp
#if !defined ev_count2_h
#define ev_count2_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "stdio.h"
#include "string.h"


class ev_count2: public Simulator { 
double Sigma;
double k[20];

char* FName;
int n;






public:
	ev_count2(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
