#include "multiplier.h"
void multiplier::init(double t,...) {
va_list parameters;
va_start(parameters, t);
inf=1e10;
u[1]=0;
mu[1]=0;
u[0]=0;
mu[0]=0;
for(int i=0;i<10;i++){y[i]=0;};
sigma=inf;
nm=0;
}
double multiplier::ta(double t) {
return sigma;
}
void multiplier::dint(double t) {
sigma=inf;
}
void multiplier::dext(Event x, double t) {
double *xv;
xv=(double*)(x.value);
u[x.port]=xv[0];
mu[x.port]=xv[1];
pu[x.port]=xv[2];
u[1-x.port]=u[1-x.port]+e*mu[1-x.port]+e*e*pu[1-x.port];
mu[1-x.port]=mu[1-x.port]+2*pu[1-x.port]*e;
sigma=0;
if (xv[2]!=0) nm=1;
}
Event multiplier::lambda(double t) {
y[0]=u[0]*u[1];
y[1]=mu[0]*u[1]+mu[1]*u[0];
if (nm==1) y[2]=u[0]*pu[1]+mu[0]*mu[1]+pu[0]*u[1];
return Event(&y[0],0);
}
void multiplier::exit() {

}
