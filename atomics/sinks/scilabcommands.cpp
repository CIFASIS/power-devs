#include "scilabcommands.h"
void scilabcommands::init(double t,...) {
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
	executeScilabJob(atInit,false);
}
double scilabcommands::ta(double t) {
//This function returns a double.
return sigma;
}
void scilabcommands::dint(double t) {

}
void scilabcommands::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
//     'e' is the time elapsed since last transition
if (strlen(atDExt)!=0)
	executeScilabJob(atDExt,true);
}
Event scilabcommands::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)
}
void scilabcommands::exit() {
//Code executed at the end of the simulation.
printLog("Hola\n");
if (strlen(atExit)!=0)
	executeScilabJob(atExit,true);
}
