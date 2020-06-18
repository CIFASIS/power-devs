#include "inverse.h"
void inverse::init(double t,...) {
va_list parameters;
va_start(parameters, t);
for(int i=0;i<10;i++){y[i]=0;};
Sigma=4e10;
nm=0;
}
double inverse::ta(double t) {
return Sigma;
}
void inverse::dint(double t) {
Sigma=4e10;
}
void inverse::dext(Event x, double t) {
double *Aux;
Aux=(double*)(x.value);
u=Aux[0];
mu=Aux[1];
pu=Aux[2];
if (pu!=0) nm=1;
Sigma=0;
}
Event inverse::lambda(double t) {
if (u==0) {
  y[0]=1e20;
} else {
y[0]=1/u;
y[1]=-mu/(u*u);
if (nm==1) y[2]=-pu/(u*u)+mu/(u*u*u);
};
return Event(&y[0],0);
}
void inverse::exit() {

}
