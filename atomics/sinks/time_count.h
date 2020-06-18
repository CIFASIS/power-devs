//CPP:sinks/time_count.cpp
#if !defined time_count_h
#define time_count_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "stdio.h"
#include "string.h"
#include "time.h"


class time_count: public Simulator { 
double Sigma;
double intim;
long int FOutput;
double tend;




public:
	time_count(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
