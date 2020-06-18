//CPP:qss/qss_wsum.cpp

//CPP:qss/qsstools.cpp

//CPP:vector/qss_int_vec.cpp
#if !defined qss_int_vec_h
#define qss_int_vec_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "qss/qss_wsum.h"
#include "engine.h"
#include "qss/qsstools.h"
#include "vector.h"


class qss_int_vec: public Simulator { 
// Declare the state,
// output variables
// and parameters

Coupling *D0;
Simulator **D1[1000];
Connection **EIC1[10000]; 
Connection **EOC1[1000];
Connection **IC1[1];
vector vec;
Event y;
int N,Ni;




public:
	qss_int_vec(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
