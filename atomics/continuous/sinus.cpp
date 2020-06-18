#include "sinus.h"
void sinus::init(double t,...) {
va_list parameters;
va_start(parameters, t);
S=0;
Ms=0;
Ps=0;
c=0;
for(int i=0;i<10;i++){Y[i]=0;};
Sigma=4e10;
nm=0;
}
double sinus::ta(double t) {
return Sigma;
}
void sinus::dint(double t) {
Sigma=10e4;
}
void sinus::dext(Event x, double t) {
double *Input;
Input=(double*)(x.value);
S=Input[0];
Ms=Input[1];
Ps=Input[2];
Sigma=0;
if (Ps!=0) nm=1; //To detect QSS3
}
Event sinus::lambda(double t) {
Y[0]=sin(S);
Y[1]=Ms*cos(S);
if (nm==1) Y[2]=-0.5*Ms*sin(S)+Ps*cos(S);
return Event(&Y[0],0);
}
void sinus::exit() {

}
