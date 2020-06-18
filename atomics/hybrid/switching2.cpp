#include "switching2.h"
void switching2::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//      %Type% is the parameter type
inf=1e10;
u[0]=0;
u[1]=0;
mu[0]=0;
mu[1]=0;
k=0;
sigma=inf;
}
double switching2::ta(double t) {
//This function return a double.
return sigma;
}
void switching2::dint(double t) {
sigma=inf;
}
void switching2::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value
//     'x.port' is the port number
double *Input;
int j;
j=x.port;
u[0]=u[0]+mu[0]*e;
u[1]=u[1]+mu[1]*e;
if (j>=2){
 if (k%2==j%2){
   sigma=inf;
  }
  else {
    k=j%2;
    sigma=0;
  };  
}
else {
 Input=(double*)(x.value);
  u[j]=Input[0];
  mu[j]=Input[1];
  if (j==k){
    sigma=0;
  };
};
}
Event switching2::lambda(double t) {
//This function return an event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% is a direction to the variable that contain the value.
//     %NroPort% is the port number (from 0 to n-1)
y[0]=u[k];
y[1]=mu[k];
return Event(&y[0],0);
}
void switching2::exit() {

}
