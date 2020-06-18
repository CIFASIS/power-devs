//CPP:sink/to_disk_var.cpp
#if !defined to_disk_var_h
#define to_disk_var_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "stdio.h"
#include "string.h"


class to_disk_var: public Simulator { 
double Sigma;
FILE *FOutput;


public:
	to_disk_var(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
