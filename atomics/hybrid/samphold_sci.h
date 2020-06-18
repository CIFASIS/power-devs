//CPP:qss/qsstools.cpp
//CPP:hybrid/samphold_sci.cpp
#if !defined samphold_sci_h
#define samphold_sci_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "qss/qsstools.h"


class samphold_sci: public Simulator { 
 double T,phi;
double Sigma;
double S[10],y[10];





public:
	samphold_sci(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
