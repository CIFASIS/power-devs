//CPP:control/pid_controler.cpp
#if !defined pid_controler_h
#define pid_controler_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "math.h"
#include "string.h"


class pid_controler: public Simulator { 
// Declare here the state, output
// variables and parameters
double u,mu,pu,X,q,mq,pq;
//double u_ant,mu_ant;
double qi,qs;
int band;
double uant;
//parameters
double pidiv3;
double eps,ep;
double sigma;
double dQ;
char* Method;
double y[10]; //output
double Kp,Ki,Kd;
#define INF 1e20;

double in_a[3];
double in_b[3];

double estder;
double gain,q_ant;
double count;
int met;










public:
	pid_controler(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
