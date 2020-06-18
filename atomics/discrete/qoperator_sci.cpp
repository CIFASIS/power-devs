#include "qoperator_sci.h"
void qoperator_sci::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//      %Type% is the parameter type
char *fvar= va_arg(parameters,char*);
state= getScilabVar(fvar );
fvar = va_arg(parameters,char*);
ts=getScilabVar(fvar);
next=state;
sigma=0;
}
double qoperator_sci::ta(double t) {
//This function returns a double.
return sigma;
}
void qoperator_sci::dint(double t) {
sigma=ts;
state=next;
}
void qoperator_sci::dext(Event x, double t) {
double *u=(double*)x.value;
sigma=sigma-e;
next=u[0]; 

if (sigma==ts )
    state=next;


}
Event qoperator_sci::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)
y[0]=state;
return Event(&y,0);
}
void qoperator_sci::exit() {
//Code executed at the end of the simulation.
}
