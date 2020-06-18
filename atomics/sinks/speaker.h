//CPP:sink/speaker.cpp
#if !defined speaker_h
#define speaker_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"



class speaker: public Simulator { 
// Declare the state, output
// variables and parameters
double sigma;
double y[10];


public:
	speaker(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
