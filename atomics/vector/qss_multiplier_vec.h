//CPP:qss/qsstools.cpp
//CPP:qss/qss_multiplier.cpp
//CPP:vector/qss_multiplier_vec.cpp
#if !defined qss_multiplier_vec_h
#define qss_multiplier_vec_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "engine.h"
#include "qss/qsstools.h"
#include "vector.h"
#include "qss/qss_multiplier.h"


class qss_multiplier_vec: public Simulator { 
// Declare the state,
// output variables
// and parameters

Coupling *D0;
//Simulator **D1[10000];
//Connection **EIC1[1]; 
//Connection **EOC1[1];
//Connection **IC1[1];
vector vec;
Event y;
int N;














public:
	qss_multiplier_vec(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
