//CPP:Modelica/modelica_funcs.cpp
//CPP:qss/qsstools.cpp
//CPP:modelica/modelica_qss_static.cpp
#if !defined modelica_qss_static_h
#define modelica_qss_static_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "math.h"
#include "string.h"
#include "modelica_funcs.h"
#include "qss/qsstools.h"


class modelica_qss_static: public Simulator { 
// Declare the state, output
// variables and parameters

//states
//states 
double sigma;
double u[200][10],f[200][10];
double uaux[200][10],faux[200][10];
double inp[200],outp[200],out_dt[200],outdt[200],out2dt[200],out_2dt[200];
int order;
double dt,dtmax,tolerr;
int lastport;
bool fromext;
//parameters
int Inputs,Outputs,Index;
double abs_accuracy,rel_accuracy;
char* Method;
//output 
double y[10];

#define INF 1e20
























public:
	modelica_qss_static(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
