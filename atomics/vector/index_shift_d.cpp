#include "index_shift_d.h"
void index_shift_d::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//	%Type% is the parameter type

char *fvar= va_arg(parameters,char*);
shift=getScilabVar(fvar );
fvar=va_arg(parameters,char*);
N=getScilabVar(fvar );

sigma=INF;

}
double index_shift_d::ta(double t) {
//This function returns a double.
return sigma;
}
void index_shift_d::dint(double t) {
sigma=INF;
}
void index_shift_d::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
//     'e' is the time elapsed since last transition
vec=*(vector*)x.value;
sigma=0;

}
Event index_shift_d::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)
vec.index=vec.index+shift;
if (vec.index>=0 && vec.index<N)
	return Event(&vec,0);
//printLog("Emiting events with index %d. Size=%d\n",vec.index,N);
if (vec.index>=N) {
	vec.index %=  N;
} else if (vec.index<0) {
	vec.index += N;
} 
return Event(&vec,1);
}
void index_shift_d::exit() {
//Code executed at the end of the simulation.
}
