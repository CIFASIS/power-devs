//CPP:realtime/rtdetect.cpp
#if !defined rtdetect_h
#define rtdetect_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"



class rtdetect: public Simulator { 
// Declare the state, output
// variables and parameters
double sigma;
double y[10];


public:
	rtdetect(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
