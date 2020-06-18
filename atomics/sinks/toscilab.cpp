#include "toscilab.h"
void toscilab::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//      %Type% is the parameter type
  time = va_arg(parameters,char*);
  signal = va_arg(parameters,char*);
  char buf[1024];
  sprintf(buf,"clear %s",time);
  executeScilabJob(buf);
  sprintf(buf,"clear %s",signal);
  executeScilabJob(buf);
  sigma=10e10;
  count = 1;
}
double toscilab::ta(double t) {
//This function returns a double.
return sigma;
}
void toscilab::dint(double t) {

}
void toscilab::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
char buff[1024];
double *y = (double*)x.value;
sprintf(buff,"%s(%d)",time,count);
putScilabVar(buff,t);
sprintf(buff,"%s(%d)",signal,count);
putScilabVar(buff,y[0]);
count++;
}
Event toscilab::lambda(double t) {

}
void toscilab::exit() {

}
