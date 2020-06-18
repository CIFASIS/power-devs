#include "clock_wait.h"
void clock_wait::init(double t,...) {
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//      %Type% is the parameter typ
inf=1e10;
itime=1.0*clock()/CLOCKS_PER_SEC;
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
actime=1.0*clock()/CLOCKS_PER_SEC - itime;
while(actime<t){
        actime=1.0*clock()/CLOCKS_PER_SEC - itime;
}
sigma=0;
}
Event clock_wait::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)
y=xv;
return Event(y,0);
}
void clock_wait::exit() {

}
