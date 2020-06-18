//CPP:qss/qss_delay.cpp
#if !defined qss_delay_h
#define qss_delay_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"



class qss_delay: public Simulator { 
// Declare the state, output
// variables and parameters

//states
double Bt[100000];
double Bu[100000][4];
long PNi, PNo;
double sigma;

//output
double y[10];

//parameters
double Delay;

#define INF 1e20;




public:
	qss_delay(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
