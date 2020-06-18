//CPP:qss/qss_wsum.cpp
//CPP:qss/qsstools.cpp
//CPP:vector/index_selector.cpp
#if !defined index_selector_h
#define index_selector_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "engine.h"
#include "qss/qsstools.h"
#include "vector.h"



class index_selector: public Simulator { 
// Declare the state,
// output variables
// and parameters

double sigma;
int from,to;
vector vec;
Event y;
#define INF 1e20









public:
	index_selector(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
