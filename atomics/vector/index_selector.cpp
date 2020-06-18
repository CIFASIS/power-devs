#include "index_selector.h"
void index_selector::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//	%Type% is the parameter type

char *fvar= va_arg(parameters,char*);
from=getScilabVar(fvar );
fvar= va_arg(parameters,char*);
to=getScilabVar(fvar );
sigma=INF;


}
double index_selector::ta(double t) {
//This function returns a double.
return sigma;
}
void index_selector::dint(double t) {
sigma=INF;
}
void index_selector::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
//     'e' is the time elapsed since last transition
vec=*(vector*)x.value;
if (vec.index>to || vec.index<from){
		sigma=INF;
} else {
		sigma=0;
};
if (vec.index<0)
	sigma=0;
}
Event index_selector::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)
return Event(&vec,0);
}
void index_selector::exit() {
//Code executed at the end of the simulation.
}
