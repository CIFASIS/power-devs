#include "modulator.h"
void modulator::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//	%Type% is the parameter type
char *fvar=va_arg( parameters, char*);
dQ=getScilabVar(fvar );
n=(int)va_arg(parameters,double);
sigma=INF;
nextport=0;
for(int i=1;i<10;i++){y[i]=0;};
offset=0;
}
double modulator::ta(double t) {
//This function returns a double.
return sigma;
}
void modulator::dint(double t) {
nextport++;
if (nextport==n) {
	sigma=INF;
	nextport=0;
} else {
  sigma=0;
};
}
void modulator::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
//     'e' is the time elapsed since last transition
nextport=0;
u=x.getDouble();
long vmin=floor(u/dQ/n);
long vrest=floor(u/dQ)-vmin*n;
for (int i=0;i<n;i++) {
  if (i<vrest) {
    v[i]=vmin+1;
	} else {
	v[i]=vmin;
	}
}
sigma=0;
offset++;
if (offset>9)offset=0;
//printLog("We have n=%i, u=%g, dQ=%g, vmin=%i, vrest=%i\n",n,u,dQ,vmin,vrest);
//printLog("We calculated [v0..v4]=[%i,%i,%i,%i,%i]\n",v[0],v[1],v[2],v[3],v[4]);
}
Event modulator::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)
int port=(nextport+offset)%10;
y[0]=v[nextport]*dQ;
return Event(&y,nextport);
}
void modulator::exit() {
//Code executed at the end of the simulation.
}
