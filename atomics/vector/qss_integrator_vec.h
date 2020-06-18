//CPP:qss/qss_wsum.cpp
//CPP:qss/qsstools.cpp
//CPP:qss/qss_integrator.cpp
//CPP:qss/qss.cpp
//CPP:qss/qss2.cpp
//CPP:qss/qss3.cpp
//CPP:qss/qss4.cpp
//CPP:qss/liqss.cpp
//CPP:qss/liqss2.cpp
//CPP:qss/liqss3.cpp
//CPP:qss/bqss.cpp
//CPP:qss/cqss.cpp
//CPP:vector/qss_integrator_vec.cpp
#if !defined qss_integrator_vec_h
#define qss_integrator_vec_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "engine.h"
#include "qss/qsstools.h"
#include "vector.h"
#include "qss/qss_integrator.h"
#include "qss/qss.h"
#include "qss/qss2.h"
#include "qss/qss3.h"
#include "qss/qss4.h"
#include "qss/liqss.h"
#include "qss/liqss2.h"
#include "qss/liqss3.h"
#include "qss/cqss.h"
#include "qss/bqss.h"


class qss_integrator_vec: public Simulator { 
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
	qss_integrator_vec(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
