//CPP:qss/qsstools.cpp
//CPP:qss/qss.cpp
//CPP:qss/qss2.cpp
//CPP:qss/qss3.cpp
//CPP:qss/qss4.cpp
//CPP:qss/bqss.cpp
//CPP:qss/cqss.cpp
//CPP:qss/liqss.cpp
//CPP:qss/liqss2.cpp
//CPP:qss/liqss3.cpp
//CPP:qss/qss_integrator.cpp
#if !defined qss_integrator_h
#define qss_integrator_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "math.h"
#include "string.h"
#include "qss/qsstools.h"
#include "qss/qss.h"
#include "qss/qss2.h"
#include "qss/qss3.h"
#include "qss/qss4.h"
#include "qss/bqss.h"
#include "qss/cqss.h"
#include "qss/liqss.h"
#include "qss/liqss2.h"
#include "qss/liqss3.h"


class qss_integrator: public Simulator { 
// Declare here the state, output
// variables and parameters

char* Method;
Simulator *solver;
enum { QSS, QSS2, QSS3, QSS4, 
	BQSS,CQSS,LIQSS,LIQSS2,
	LIQSS3 } met;


























public:
	qss_integrator(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
