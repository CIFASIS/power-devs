#include "clock_wait.h"
void clock_wait::init(double t,...) {
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//      %Type% is the parameter typ
va_list parameters;
va_start(parameters,t);
waitMode = va_arg(parameters,char*);
//printf("Arg = '%s' y cmp = %d\n",waitMode,strcmp(waitMode,"Normal"));
//exit();
inf=1e10;
sigma=inf;
}
double clock_wait::ta(double t) {
//This function returns a double.
return sigma;
}
void clock_wait::dint(double t) {
sigma=inf;
}
void clock_wait::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
double actime;
xv=x.value;
sigma=0;
}
Event clock_wait::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)
y=xv;
Event out =  Event(y,0);
if (strcmp(waitMode,"Normal")==0) {
  out.setRealTimeMode(REALTIME);
} else {
  out.setRealTimeMode(PRECISEREALTIME);
}
return out;
}
void clock_wait::exit() {

}
