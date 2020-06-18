//CPP:/random/lib/stocc.cpp
//CPP:/random/lib/randomc.cpp
//CPP:random/normal_gen.cpp
#if !defined normal_gen_h
#define normal_gen_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "time.h"
#include "random/lib/randomc.h"
#include "random/lib/stocc.h"


class normal_gen: public Simulator { 
// Declare the state,
// output variables
// and parameters
double sigma,v;

double m,std,T;

double y[10];
StochasticLib1 *stor;






public:
	normal_gen(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
