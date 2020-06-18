#include "pwmgen.h"
void pwmgen::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//      %Type% is the parameter type
   a=va_arg(parameters, double);
   tm=va_arg(parameters, double);
   int i;   
   i = 0;
   dt[0] = 0.52576249002189;
   i = i + 1;
   dt[i] = 0.42770710237896;
   i = i + 1;
   dt[i] = 0.62266988694768;
   i = i + 1;
   dt[i] = 0.33235474394723;
   i = i + 1;
   dt[i] = 0.71465110292802;
   i = i + 1;
   dt[i] = 0.24345384677928;
   i = i + 1;
   dt[i] = 0.79816713494423;
   i = i + 1;
   dt[i] = 0.16448202528866;
   i = i + 1;
   dt[i] = 0.87006137717856;
   i = i + 1;
   dt[i] = 0.09849927665395;
   i = i + 1;
   dt[i] = 0.92767110593532;
   i = i + 1;
   dt[i] = 0.04803154181473;
   i = i + 1;
   dt[i] = 0.96890546649863;
   i = i + 1;
   dt[i] = 0.01498703066906;
   i = i + 1;
   dt[i] = 0.99229400455701;
   i = i + 1;
   dt[i] = 0.00060227626444;
   i = i + 1;
   dt[i] = 0.99701303051085;
   i = i + 1;
   dt[i] = 0.00541209560964;
   i = i + 1;
   dt[i] = 0.98289729495148;
   i = i + 1;
   dt[i] = 0.02923790417889;
   i = i + 1;
   dt[i] = 0.95044204882374;
   i = i + 1;
   dt[i] = 0.07119152689703;
   i = i + 1;
   dt[i] = 0.90079658391823;
   i = i + 1;
   dt[i] = 0.12969549084197;
   i = i + 1;
   dt[i] = 0.83574604807234;
   i = i + 1;
   dt[i] = 0.20252423831807;
   i = i + 1;
   dt[i] = 0.75767499017353;
   i = i + 1;
   dt[i] = 0.2868723108525;
   i = i + 1;
   dt[i] = 0.66950488056329;
   i = i + 1;
   dt[i] = 0.37945431748385;
   i = i + 1;
   dt[i] = 0.57459956378267;
   i = i + 1;
   dt[i] = 0.47663726221426;
for (i=0;i<32;i++) {
  dt[32 + i] = dt[31 - i];
};
for (i=0;i<64;i++) {
    dt[i] = 1.0*dt[i]*tm/32.0;
}; 
sigma = 0,
sig=1;
j = -1;
for(int i=0;i<10;i++){y[i]=0;};
}
double pwmgen::ta(double t) {
//This function return a double.
return sigma;
}
void pwmgen::dint(double t) {
j=j+1;
if (j>63) {
j=0;
}
sigma=dt[j];
sig=-sig;
}
void pwmgen::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value
//     'x.port' is the port number
}
Event pwmgen::lambda(double t) {
//This function return an event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% is a direction to the variable that contain the value.
//     %NroPort% is the port number (from 0 to n-1)
y[0]=1.0*a*sig;
y[1]=0;
return Event(&y,0);
}
void pwmgen::exit() {

}
