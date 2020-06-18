//CPP:qss/qsstools.cpp

//CPP:modelica/modelica_qss_cross_detect.cpp
#if !defined modelica_qss_cross_detect_h
#define modelica_qss_cross_detect_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "qss/qsstools.h"
#include "modelica_funcs.h"


class modelica_qss_cross_detect: public Simulator { 

//states
double sigma;
double u[10];
int sw;

//output
double y[10];

//parameters
double level,outval;
int index;
bool from_discont;
#define INF 1e20


































public:
	modelica_qss_cross_detect(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
