#include "samphold_sci.h"
void samphold_sci::init(double t,...) {
va_list parameters;
va_start(parameters,t);
char *fvar= va_arg(parameters,char*);
T= getScilabVar(fvar );
fvar= va_arg(parameters,char*);
phi= getScilabVar(fvar );
for(int i=0;i<10;i++){
            S[i]=0;
            y[i]=0;
}
Sigma=phi;
}
double samphold_sci::ta(double t) {
return Sigma;
}
void samphold_sci::dint(double t) {
advance_time(S,Sigma,4);
Sigma=T;
}
void samphold_sci::dext(Event x, double t) {
double *Aux;
Aux=(double*)(x.value);
S[0]=Aux[0];
S[1]=Aux[1];
S[2]=Aux[2];
S[3]=Aux[3];
Sigma=Sigma-e;
}
Event samphold_sci::lambda(double t) {
y[0]=evaluate_poly(S,Sigma,4);
return Event(&y[0],0);
}
void samphold_sci::exit() {

}
