#include "sum.h"
void sum::init(double t,...) {
va_list parameters;
va_start(parameters, t);
n=va_arg(parameters, double);
int i;
for (i=0;i<20;i++) {
  S[i]=0;
  Ms[i]=0;
};
Output[0]=0;
Output[1]=0;
Sigma=4e10;
}
double sum::ta(double t) {
return Sigma;
}
void sum::dint(double t) {
Sigma=4e10;
}
void sum::dext(Event x, double t) {
double *Aux;
int i;
Aux=(double*)(x.value);
S[x.port]=Aux[0];
for (i=0;i<n;i++) {
  if (i!=x.port) {
    S[i]=S[i]+Ms[i]*e;
  } else {
    S[i]=Aux[0];
  };
};
Ms[x.port]=Aux[1];
Sigma=0;
}
Event sum::lambda(double t) {
int i;
Output[0]=0;
Output[1]=0;
for(i=0;i<n;i++) {
  Output[0]=Output[0]+S[i];
  Output[1]=Output[1]+Ms[i];
};
return Event(&Output[0],0);
}
void sum::exit() {

}
