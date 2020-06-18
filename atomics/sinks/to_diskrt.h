//CPP:sink/to_diskrt.cpp
#if !defined to_diskrt_h
#define to_diskrt_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "stdio.h"
#include "string.h"
#include "time.h"


class to_diskrt: public Simulator { 
double Sigma;
double itime;
FILE *FOutput;


public:
	to_diskrt(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
