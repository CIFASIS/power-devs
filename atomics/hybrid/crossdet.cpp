#include "crossdet.h"
void crossdet::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//      %Type% is the parameter type
level = va_arg(parameters,double);
X = va_arg(parameters,double);
y[0] = va_arg(parameters,double);
y[1]=0;
inf=1e10;
sigma=inf;
}
double crossdet::ta(double t) {
//This function return a double.
return sigma;
}
void crossdet::dint(double t) {
double t1;
double t2;
double disc;
double uv;
X=X+sigma*u+sigma*sigma*mu/2;
uv=u;
u=u+sigma*mu;
if (u*uv>=0) {
  sigma=inf;
}
else {
  disc=u*u-2*(X-level)*mu;
  if (disc<0) {
    t1=inf;
    t2=inf;
  }
  else {
    t1=(-u-sqrt(disc))/mu;
    if (t1<=0) {
      t1=inf;
    };
    t2=(-u+sqrt(disc))/mu;
    if (t2<=0) {
      t2=inf;
    };
  };
  if (t1<=t2) {
    sigma=t1; 
  }
  else {
    sigma=t2;
  };
};
}
void crossdet::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value
//     'x.port' is the port number
double *Input;
double t1;
double t2;
double disc;
Input=(double*)(x.value);
X=X+u*e+mu*e*e/2;
u=Input[0];
mu=Input[1];
if (sigma==e) {
  sigma=0;
}
else {
if (mu==0) {
  if (u==0) {
    t1=inf;
    t2=inf;
  } 
  else {
    t1=(level-X)/u;
    if (t1<=0) {
      t1=inf;
    };
    t2=inf; 
  };
}
else {
  disc=u*u-2*(X-level)*mu;
  if (disc<0) {
    t1=inf;
    t2=inf;
  }
  else {
    t1=(-u-sqrt(disc))/mu;
    if (t1<=0) {
      t1=inf;
    };
    t2=(-u+sqrt(disc))/mu;
    if (t2<=0) {
      t2=inf;
    };
  };
};
if (t1<=t2) {
  sigma=t1;
}
else {
  sigma=t2;
};
};
}
Event crossdet::lambda(double t) {
//This function return an event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% is a direction to the variable that contain the value.
//     %NroPort% is the port number (from 0 to n-1)
return Event(&y[0],0);
}
void crossdet::exit() {

}
