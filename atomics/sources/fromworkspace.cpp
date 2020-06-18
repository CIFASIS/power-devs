#include "fromworkspace.h"
void fromworkspace::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//	%Type% is the parameter type
time_var = va_arg(parameters,char*);
signal_var = va_arg(parameters,char*);
if (strcmp("QSS",va_arg(parameters,char*))==0) 
	method=0;
else
	method=1;
sprintf(buf,"length(%s)",time_var);
max=(int)getScilabVar(buf);
tu=(double*)calloc(max,sizeof(double));
getScilabVector(time_var,&max,tu);
u=(double*)calloc(max,sizeof(double));
getScilabVector(signal_var,&max,u);
count=0;
actualT=0;
nextT=0;
for (int i=0;i<10;i++) 
	y[i]=0;

Sigma=1e10;

if (count<max) {
   actualV=u[0];
   actualT=tu[0];
   nextT=tu[1];
	if (method && count+1<max) {
		nextV=u[1];
	}
	Sigma=actualT;
} else {
	Sigma=1e10;
}
}
double fromworkspace::ta(double t) {
//This function returns a double.
return Sigma;
}
void fromworkspace::dint(double t) {
count++;
if (count-1<max) {
	actualT=tu[count-1];
	actualV=u[count];
	nextT=tu[count];
	if (method && count+1<max) {
		nextV=u[count+1];
	}
	Sigma=-actualT+nextT;
} else {
	Sigma=1e10;
}
if (Sigma<0) Sigma=1e10;
}
void fromworkspace::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
//     'e' is the time elapsed since last transition
}
Event fromworkspace::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)
y[0]=actualV;
if (method) {
	y[1]=(nextV-actualV)/fabs(nextT-actualT);
} 
return Event(y,0);
}
void fromworkspace::exit() {
//Code executed at the end of the simulation.
free(u);
free(tu);
}
