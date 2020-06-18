//CPP:continuous/delay_sci.cpp
#if !defined delay_sci_h
#define delay_sci_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"



class delay_sci: public Simulator { 
// Declare the state, output
// variables and parameters

//states
double Bt[100000];
double Bu[100000];
double Bmu[100000];
double Bpu[100000];
long PNi, PNo;
double sigma;

//output
double y[10];

//parameters
double Delay;

#define INF 1e20;


public:
	delay_sci(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
