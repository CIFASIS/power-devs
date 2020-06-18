#include "runscilabjob.h"
void runscilabjob::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//	%Type% is the parameter type
atInit=va_arg(parameters,char*);
atDExt=va_arg(parameters,char*);
atExit=va_arg(parameters,char*);

sigma=1e10;
if (strlen(atInit)!=0)
	executeVoidScilabJob(atInit,true);
}
double runscilabjob::ta(double t) {
//This function returns a double.
return sigma;
}
void runscilabjob::dint(double t) {

}
void runscilabjob::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
//     'e' is the time elapsed since last transition
if (strlen(atDExt)!=0)
	executeVoidScilabJob(atDExt,true);
}
Event runscilabjob::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)
return Event();
}
void runscilabjob::exit() {
//Code executed at the end of the simulation.
if (strlen(atExit)!=0)
	executeVoidScilabJob(atExit,true);
}
