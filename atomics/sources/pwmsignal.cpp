#include "pwmsignal.h"
void pwmsignal::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//      %Type% is the parameter type
a = va_arg(parameters,double);
freq = va_arg(parameters,double);
j=-1;
int i=0;
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
 for ( i=0;i<32;i++){
   dt[32 + i] = dt[31 - i];
 };
 for ( i=0;i<64;i++){
   dt[i] = dt[i] / freq / 32.0;   
 };
sigma=0;
y[0]=0;
y[1]=0;
y[2]=0;
}
double pwmsignal::ta(double t) {
//This function return a double.
return sigma;
}
void pwmsignal::dint(double t) {
j=j+1;
if (j==64) {
  j=0;
};
sigma=dt[j];
}
void pwmsignal::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value
//     'x.port' is the port number
}
Event pwmsignal::lambda(double t) {
//This function return an event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% is a direction to the variable that contain the value.
//     %NroPort% is the port number (from 0 to n-1)
int mo;
mo=(j+1)%2;
if (mo==1) {
  y[0]=-a;
} else {
  y[0]=a;
};
return Event(&y[0],0);
}
void pwmsignal::exit() {

}
