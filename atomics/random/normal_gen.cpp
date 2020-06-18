#include "normal_gen.h"
void normal_gen::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//	%Type% is the parameter type
char *fvar= va_arg(parameters,char*);
m=getScilabVar(fvar );
fvar= va_arg(parameters,char*);
std=getScilabVar(fvar );
fvar= va_arg(parameters,char*);
T=getScilabVar(fvar );
fvar= va_arg(parameters,char*);
int seed = (strlen(fvar)==0 ? (int)time(0)+rand() : getScilabVar(fvar));            // random seed
sigma=0;
for (int i=0;i<10;i++)y[i]=0;

stor=new StochasticLib1(seed);
v = stor->Normal(m, std);
}
double normal_gen::ta(double t) {
//This function returns a double.
return sigma;
}
void normal_gen::dint(double t) {
sigma=T;
v = stor->Normal(m, std);
}
void normal_gen::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
//     'e' is the time elapsed since last transition
}
Event normal_gen::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)
y[0]=v;
return Event(&y,0);
}
void normal_gen::exit() {
//Code executed at the end of the simulation.
}
