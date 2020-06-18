#include "rtview_lcd.h"
void rtview_lcd::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where: 
//      %Name% is the parameter name
//	%Type% is the parameter type

char *initial =va_arg(parameters,char*);
char buff[1024];
sprintf(filename,"plots/%ld.dat",(long int)this);
sprintf(buff,"lcd -name %s -value %d -file plots/%ld.dat",name,(int)getScilabVar(initial),(long int)this);
spawnProcess("../bin/rtview",buff);

}
double rtview_lcd::ta(double t) {
return 1e10;
//This function returns a double.

}
void rtview_lcd::dint(double t) {

}
void rtview_lcd::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
//     'e' is the time elapsed since last transition
FILE *fd=fopen(filename,"w");
if (fd!=NULL){
  fprintf(fd,"%d",(int)x.getDouble());
  fclose(fd); 
}
}
Event rtview_lcd::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)


return Event();
}
void rtview_lcd::exit() {
//Code executed at the end of the simulation.
spawnProcess("../bin/rtview", "clean");

}
