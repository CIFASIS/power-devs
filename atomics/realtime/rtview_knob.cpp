#include "rtview_knob.h"
void rtview_knob::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//	%Type% is the parameter type
char buff[1024];
char *min=va_arg(parameters,char*);
char *max=va_arg(parameters,char*);
char *init=va_arg(parameters,char*);

sprintf(filename,"plots/%ld.dat",(long int)this);

initval=getScilabVar(init);
unlink(filename);
sprintf(buff,"knob -min %d -max %d  -name %s -file plots/%ld.dat -value %d",(int)getScilabVar(min),(int)getScilabVar(max),name,(long int)this,initval);
spawnProcess("../bin/rtview", buff);
for (int i=0;i<10;i++) y[i]=0;
sigma=0;
y[0]=initval;
}
double rtview_knob::ta(double t) {
//This function returns a double.
return sigma;
}
void rtview_knob::dint(double t) {
sigma=0.2;
}
void rtview_knob::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
//     'e' is the time elapsed since last transition

}
Event rtview_knob::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)
FILE *fd=fopen(filename,"r");
if (fd!=NULL) {
	int v;
	if (fscanf(fd,"%d",&v)) 
		y[0]=v;
	fclose(fd);
}
return Event(y,0);

}
void rtview_knob::exit() {
//Code executed at the end of the simulation.
spawnProcess("../bin/rtview", "clean");
}
