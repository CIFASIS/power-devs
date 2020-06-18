#include "hystcomp.h"
void hystcomp::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.

va_list parameters;
va_start(parameters,t);

//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//      %Type% is the parameter type
swon = va_arg(parameters,double);
swoff = va_arg(parameters,double);
outon = va_arg(parameters,double);
outoff = va_arg(parameters,double);
inf=1e10;
u=0;
mu=0;
if (u<swon) {prevst=1;} else {prevst=0;};
sigma=0;
}
double hystcomp::ta(double t) {
//This function returns a double.
return sigma;
}
void hystcomp::dint(double t) {
prevst=1-prevst;
u=u+mu*sigma;
sigma=inf;
}
void hystcomp::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
double *xv;
xv=(double*)(x.value);
u=xv[0];
mu=xv[1];
if (prevst==0){
  if (u>swon) {
    sigma=0;
  } else {
    if (mu>0) {
       sigma=(swon-u)/mu;
    } else {
       sigma=inf;
    };
  };
} else {
  if (u<swoff) {
    sigma=0;
  } else {
    if (mu<0) {
       sigma=(swoff-u)/mu;
    } else {
       sigma=inf;
    };
  };
};
}
Event hystcomp::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)

if (prevst==0) {y[0]=outon;} else {y[0]=outoff;};
y[1]=0;
return Event(&y[0],0);
}
void hystcomp::exit() {

}
