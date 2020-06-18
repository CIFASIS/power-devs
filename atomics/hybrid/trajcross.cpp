#include "trajcross.h"
void trajcross::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//      %Type% is the parameter type
X = va_arg(parameters,double);
y[0] = va_arg(parameters,double);
y[1]=0;
inf=1e10;
sigma=inf;
}
double trajcross::ta(double t) {
//This function return a double.
return sigma;
}
void trajcross::dint(double t) {
double t1;
double t2;
double disc;
X=X+sigma*u+sigma*sigma*mu/2;
u=u+mu*sigma;
v=v+mv*sigma;
sigma=inf;
}
void trajcross::dext(Event x, double t) {
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
if (x.port==0){
  u=Input[0];
  mu=Input[1];
  v=v+mv*e;
}
else {
  v=Input[0]; 
  mv=Input[1];
  u=u+mu*e;
};
if (sigma==e) {
  sigma=0;
}
else {
if (mu==0) {
  if (u-mv==0) {
    t1=inf;
    t2=inf;
  } 
  else {
    t1=(v-X)/(u-mv);
    if (t1<=0) {
      t1=inf;
    };
    t2=inf; 
  };
}
else {
  disc=(u-mv)*(u-mv)-2*(X-v)*mu;
  if (disc<0) {
    t1=inf;
    t2=inf;
  }
  else {
    t1=(mv-u-sqrt(disc))/mu;
    if (t1<=0) {
      t1=inf;
    };
    t2=(mv-u+sqrt(disc))/mu;
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
Event trajcross::lambda(double t) {
//This function return an event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% is a direction to the variable that contain the value.
//     %NroPort% is the port number (from 0 to n-1)
return Event(&y[0],0);
}
void trajcross::exit() {

}
