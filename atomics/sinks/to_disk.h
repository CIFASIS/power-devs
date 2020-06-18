//CPP:sinks/to_disk.cpp
#if !defined to_disk_h
#define to_disk_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "string.h"


class to_disk: public Simulator { 
double Sigma;
long int FOutput;



public:
	to_disk(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
