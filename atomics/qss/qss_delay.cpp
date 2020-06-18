#include "qss_delay.h"
void qss_delay::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
char *fvar=va_arg(parameters,char*);
Delay=getScilabVar(fvar);
for(int i=0;i<10;i++){y[i]=0;};
PNi=1;
PNo=0;
sigma=INF;
}
double qss_delay::ta(double t) {
return sigma;
}
void qss_delay::dint(double t) {
PNo=(PNo+1)%100000;
if ( ((PNo+1)%100000)==PNi ) {
	sigma=INF;
}else{
	sigma=Bt[PNo]+Delay-t;
	if(sigma<0){sigma=0;};
};
}
void qss_delay::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
double* xv;
xv= (double*) x.value;
if(PNi!=PNo){
	Bt[PNi-1]=t;
	Bu[PNi-1][0]=xv[0];
	Bu[PNi-1][1]=xv[1];
	Bu[PNi-1][2]=xv[2];
	Bu[PNi-1][3]=xv[3];

	if(PNi==((PNo+1)%100000)){sigma=Delay;}else{sigma=sigma-e;};
PNi++;
};
}
Event qss_delay::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)
y[0]=Bu[PNo][0];
y[1]=Bu[PNo][1];
y[2]=Bu[PNo][2];
y[3]=Bu[PNo][3];

return Event(y,0);
}
void qss_delay::exit() {

}
