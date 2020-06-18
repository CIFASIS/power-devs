#include "xsquare.h"
void xsquare::init(double t,...) {
va_list parameters;
va_start(parameters, t);
S[0]=0;
S[1]=0;
S[2]=0;
Sigma=4e10;
for(int i=0;i<10;i++){y[i]=0;};
nm=0;
}
double xsquare::ta(double t) {
return Sigma;
}
void xsquare::dint(double t) {
Sigma=4e10;
}
void xsquare::dext(Event x, double t) {
double *Aux;
Aux=(double*)(x.value);
S[0]=Aux[0];
S[1]=Aux[1];
S[2]=Aux[2];
Sigma=0;
if (Aux[2]!=0) nm=1;
}
Event xsquare::lambda(double t) {
y[0]=S[0]*S[0];
y[1]=2*S[0]*S[1];
if (nm==1) y[2]=2*S[0]*S[2]+S[1]*S[1];
return Event(&y[0],0);
}
void xsquare::exit() {

}
