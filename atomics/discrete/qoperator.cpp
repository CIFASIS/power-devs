#include "qoperator.h"
void qoperator::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//      %Type% is the parameter type
state=va_arg(parameters,double);
next=state;
sigma=0;
ts=va_arg(parameters,double);
}
double qoperator::ta(double t) {
//This function returns a double.
return sigma;
}
void qoperator::dint(double t) {
sigma=ts;
state=next;
}
void qoperator::dext(Event x, double t) {
double *u=(double*)x.value;
sigma=sigma-e;
next=u[0];
if (sigma==ts ) {
    state=next;
}
}
Event qoperator::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)
y[0]=state;
return Event(&y,0);
}
void qoperator::exit() {
//Code executed at the end of the simulation.
}
