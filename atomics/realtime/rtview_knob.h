//CPP:realtime/rtview_knob.cpp
#if !defined rtview_knob_h
#define rtview_knob_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "unistd.h"


class rtview_knob: public Simulator { 
// Declare the state,
// output variables
// and parameters

char filename[128];
double y[10];
int initval;
double sigma;











public:
	rtview_knob(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
