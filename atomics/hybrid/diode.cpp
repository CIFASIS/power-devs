#include "diode.h"
void diode::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//      %Type% is the parameter type
inf=1e10;
u=0;
mu=0;
sigma=inf;
}
double diode::ta(double t) {
//This function returns a double.
return sigma;
}
void diode::dint(double t) {
if (sigma>0) {
  u=0;
  sigma=inf;
} else {
  if (u*mu<0) {
    sigma=-u/mu;
  } else {
    sigma=inf;
  };
};
}
void diode::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
double *xv;
xv=(double*)(x.value);
if (xv[0]>=0) {
  sigma=0;
} else {
  if (y[0]>=0) {
    sigma=0;
  } else {
    if (xv[1]>0) {
       sigma=-xv[0]/xv[1];
    } else {
       sigma=inf;
    };
  };
};
u=xv[0];
mu=xv[1];
}
Event diode::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)
if (sigma==0){
  if (u<0){
    y[0]=0;
    y[1]=0;
  } else {
    y[0]=u;
    y[1]=mu;
  };
} else {
  if (mu<0){
    y[0]=0;
    y[1]=0;
  } else {
    y[0]=0;
    y[1]=mu;
  };
};  
return Event(&y[0],0);
}
void diode::exit() {

}
