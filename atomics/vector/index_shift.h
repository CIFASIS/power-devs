//CPP:qss/qss_wsum.cpp

//CPP:qss/qsstools.cpp

//CPP:qss/qss_integrator.cpp

//CPP:vector/index_shift.cpp
#if !defined index_shift_h
#define index_shift_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "engine.h"
#include "qss/qsstools.h"
#include "vector.h"
#include "qss/qss_integrator.h"


class index_shift: public Simulator { 
// Declare the state,
// output variables
// and parameters

double sigma;
int shift;
vector vec;
Event y;
#define INF 1e20





public:
	index_shift(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
