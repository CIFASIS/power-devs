#include "sinus2.h"
void sinus2::init(double t,...) {
va_list parameters;
va_start(parameters, t);
S=0;
Ms=0;
Ps=0;
c=0;
for(int i=0;i<10;i++){Y[i]=0;};
Sigma=4e10;
}
double sinus2::ta(double t) {
return Sigma;
}
void sinus2::dint(double t) {
Sigma=10e4;
}
void sinus2::dext(Event x, double t) {
double *Input;
Input=(double*)(x.value);
S=Input[0];
Ms=Input[1];
Ps=Input[2];
Sigma=0;
}
Event sinus2::lambda(double t) {
Y[0]=sin(S);
Y[1]=Ms*cos(S);
Y[2]=-0.5*Ms*sin(S)+Ps*cos(S);
return Event(&Y[0],0);
}
void sinus2::exit() {

}
