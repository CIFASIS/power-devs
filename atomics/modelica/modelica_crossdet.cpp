#include "modelica_crossdet.h"
void modelica_crossdet::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//      %Type% is the parameter type
level = 0;
inf=1e20;
u=0;
mu=0;
pu=0;
sigma=inf;
tcross=inf;

for (int i=0;i<10;i++) {
  y[i]=0;
 };
}
double modelica_crossdet::ta(double t) {
//This function returns a double.
return sigma;
}
void modelica_crossdet::dint(double t) {
u=u+mu*sigma+pu*sigma*sigma;
mu=mu+2*pu*sigma;
if (mu*pu<0) {
  sigma=-mu/pu;
} else {
  sigma=inf;
};
tcross=sigma;
}
void modelica_crossdet::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
double *xv;
xv=(double*)x.value;
u=xv[0];
mu=xv[1];
pu=xv[2];
//calculate time to next crossing (control event)
double a,b,c,s1,s2;
a=pu;
b=mu;
c=u-level;
if (a==0) {
   if (b==0){
     s1=inf;
     s2=inf;
   } else {
     s1=-c/b;
     s2=inf;
   };
} else {
    s1=(-b+sqrt(b*b-4*a*c))/2/a;
    s2=(-b-sqrt(b*b-4*a*c))/2/a;
};
if ((s1>0)&&((s1<s2)||(s2<0))) {
    tcross=s1;
} else {
  if (s2>0) {
     tcross=s2;
  } else {
     tcross=inf;
  };
};
sigma=tcross+1e-10; //we leave it cross
}
Event modelica_crossdet::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)
function_updateDepend();
y[0]=1;
return Event(&y[0],0);
}
void modelica_crossdet::exit() {

}
