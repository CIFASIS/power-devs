#include "qss.h"
void qss::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//      %Type% is the parameter type
for (int i=0;i<10;i++) {
	y[i]=0;
 	X[i]=0;
	q[i]=0;
};

char *fvar= va_arg(parameters,char*);
dQmin=getScilabVar(fvar );

fvar= va_arg(parameters,char*);
dQrel=getScilabVar(fvar );

fvar= va_arg(parameters,char*);
X[0]=getScilabVar(fvar);

dQ=fabs(X[0])*dQrel;
if (dQ<dQmin){dQ=dQmin;};

q[0]=X[0]; 
sigma=0;
}
double qss::ta(double t) {
//This function return a double.
return sigma;
}
void qss::dint(double t) {
advance_time(X,sigma,1);
q[0]=X[0];
dQ=dQrel*fabs(X[0]);
if (dQ<dQmin)dQ=dQmin;
if (X[1]==0){
	sigma=INF;
} else {
	sigma=fabs(dQ/X[1]);
}

}
void qss::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value
//     'x.port' is the port number
double *derx;
double diffxq[10];
double dt1;

derx=(double*)x.value;
 

if (x.port==0) {
	X[0]=X[0]+X[1]*e;
	X[1]=derx[0];
	if (sigma>0){
	   	diffxq[1]=-X[1];
   		diffxq[0]=q[0]-X[0]-dQ;
   		sigma=minposroot(diffxq,1);
   		diffxq[0]=q[0]-X[0]+dQ;
   		dt1=minposroot(diffxq,1);
   		if (dt1<sigma) sigma=dt1;
   		if (fabs(X[0]-q[0])>dQ) sigma=0;
	}
} else {
	X[0]=derx[0];
	sigma=0;
}
}
Event qss::lambda(double t) {
//This function return an event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% is a direction to the variable that contain the value.
//     %NroPort% is the port number (from 0 to n-1)

y[0]=X[0];
y[1]=X[1];
advance_time(y,sigma,1);
y[1]=0;	 

return Event(&y,0);
}
void qss::exit() {

}
