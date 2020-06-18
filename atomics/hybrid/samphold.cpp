#include "samphold.h"
void samphold::init(double t,...) {
va_list parameters;
va_start(parameters,t);
T=va_arg(parameters, double);
phi=va_arg(parameters, double);
for(int i=0;i<10;i++){
            S[i]=0;
            y[0]=0;
}
Sigma=phi;
}
double samphold::ta(double t) {
return Sigma;
}
void samphold::dint(double t) {
S[0]=S[0]+Sigma*S[1]+Sigma*Sigma*S[2];
S[1]=S[1]+2*S[2]*Sigma;
Sigma=T;
}
void samphold::dext(Event x, double t) {
double *Aux;
Aux=(double*)(x.value);
S[0]=Aux[0];
S[1]=Aux[1];
S[2]=Aux[2];
Sigma=Sigma-e;
}
Event samphold::lambda(double t) {
y[0]=S[0]+Sigma*S[1]+Sigma*Sigma*S[2];
return Event(&y[0],0);
}
void samphold::exit() {

}
