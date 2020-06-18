#include "scalar2vec.h"
void scalar2vec::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//	%Type% is the parameter type

char *fvar= va_arg(parameters,char*);
index=getScilabVar(fvar );
sigma=INF;

}
double scalar2vec::ta(double t) {
//This function returns a double.
return sigma;
}
void scalar2vec::dint(double t) {
sigma=INF;
}
void scalar2vec::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
//     'e' is the time elapsed since last transition
y=*(double*)x.value;
vec.value=y;
vec.index=index;
sigma=0;
}
Event scalar2vec::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)

return Event(&vec,0);
}
void scalar2vec::exit() {
//Code executed at the end of the simulation.
}
