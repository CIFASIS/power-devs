#include "lcd.h"
void lcd::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//	%Type% is the parameter type
char buff[128];
sprintf(buff,"%d.out %s",(int)this,name);
sprintf(filename,"%d.out",(int)this);

if (getOs()!=WINDOWS) {
	spawnProcess("../atomics/sinks/lcd",buff);
}
}
double lcd::ta(double t) {
//This function returns a double.
return 1e20;

}
void lcd::dint(double t) {

}
void lcd::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
//     'e' is the time elapsed since last transition
FILE *fd=fopen(filename,"w");
fprintf(fd,"%d",((int)x.getDouble()) % 10000);
fclose(fd);

}
Event lcd::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)


return Event();
}
void lcd::exit() {
//Code executed at the end of the simulation.

}
