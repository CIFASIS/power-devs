//CPP:qss/qsstools.cpp
//CPP:qss/xpower2.cpp
//CPP:vector/vector_pow2.cpp
#if !defined vector_pow2_h
#define vector_pow2_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "engine.h"
#include "qss/qsstools.h"
#include "vector.h"
#include "qss/xpower2.h"


class vector_pow2: public Simulator { 
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
int N,Ni;













public:
	vector_pow2(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
