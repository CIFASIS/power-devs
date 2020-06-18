#include "levelcheck.h"
void levelcheck::init(double t,...) {
va_list parameters;
va_start(parameters,t);
level=va_arg(parameters,double);
outvalue=va_arg(parameters,double);
inf=1e20;
for(int i=0;i<10;i++){
            y[0]=0;
}
u=0;
mu=0;
pu=0;
sigma=inf;
}
double levelcheck::ta(double t) {
return sigma;
}
void levelcheck::dint(double t) {
sigma=inf;
}
void levelcheck::dext(Event x, double t) {
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
  if (u>level) {
    sigma=0;
  } else {
    sigma=inf;
  };
};
}
Event levelcheck::lambda(double t) {
y[0]=outvalue;
  return Event(&y[0],0);
}
void levelcheck::exit() {

}
