//CPP:qss/qss_wsum.cpp
//CPP:qss/qsstools.cpp
//CPP:index_shift_d.cpp
#if !defined index_shift_d_h
#define index_shift_d_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "engine.h"
#include "qss/qsstools.h"
#include "vector.h"


class index_shift_d: public Simulator { 
// Declare the state,
// output variables
// and parameters

double sigma;
int shift;
vector vec;
Event y;
#define INF 1e20
int N;
























public:
	index_shift_d(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
