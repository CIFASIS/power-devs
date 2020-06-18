#include "commsampler.h"
void commsampler::init(double t,...) {
va_list parameters;
va_start(parameters,t);
inf=1e20;
for(int i=0;i<10;i++){
            y[0]=0;
}
u=0;
mu=0;
pu=0;
sigma=inf;
}
double commsampler::ta(double t) {
return sigma;
}
void commsampler::dint(double t) {
sigma=inf;
}
void commsampler::dext(Event x, double t) {
double *xv;
xv=(double*)(x.value);
if (x.port==0){
  u=xv[0];
  mu=xv[1];
  pu=xv[2];
  sigma=sigma-e;
} else {
  u=u+mu*e+pu*e*e;
  mu=mu+2*pu*e;
  sigma=0;
};
}
Event commsampler::lambda(double t) {
y[0]=u;
  return Event(&y[0],0);
}
void commsampler::exit() {

}
