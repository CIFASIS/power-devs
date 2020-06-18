#include "vec2scalar.h"
void vec2scalar::init(double t,...) {
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
double vec2scalar::ta(double t) {
//This function returns a double.
return sigma;
}
void vec2scalar::dint(double t) {
sigma=INF;
}
void vec2scalar::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
//     'e' is the time elapsed since last transition
vector vec=*(vector*)x.value;
if (vec.index==index || vec.index<0){
   for (int i=0;i<10;i++) y[i]=x.getDouble(i);
	sigma=0;
};
}
Event vec2scalar::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)

return Event(&y,0);
}
void vec2scalar::exit() {
//Code executed at the end of the simulation.
}
