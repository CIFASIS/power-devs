//CPP:qss/qss_wsum.cpp

//CPP:qss/qsstools.cpp

//CPP:qss/qss_integrator.cpp
//CPP:signal/scalar2vec.cpp
#if !defined scalar2vec_h
#define scalar2vec_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "engine.h"
#include "qss/qsstools.h"
#include "vector.h"
#include "qss/qss_integrator.h"


class scalar2vec: public Simulator { 
// Declare the state,
// output variables
// and parameters

double sigma;
int index;
vector vec;
double y[10];

#define INF 1e20



public:
	scalar2vec(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
