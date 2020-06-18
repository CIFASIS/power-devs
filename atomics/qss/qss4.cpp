#include "qss4.h"
void qss4::init(double t,...) {
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
double qss4::ta(double t) {
//This function return a double.
return sigma;
}
void qss4::dint(double t) {
advance_time(X,sigma,4);
q[0]=X[0];
q[1]=X[1];
q[2]=X[2];
q[3]=X[3];
dQ=dQrel*fabs(X[0]);
if (dQ<dQmin)dQ=dQmin;
if (X[4]==0) {
	sigma=INF;
} else {
	sigma=pow(fabs(dQ/X[4]),1.0/4);
}
};

void qss4::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value
//     'x.port' is the port number
double *derx;
double diffxq[10];
double dt1;

derx=(double*)x.value;
 
if (x.port==0) { 
	X[0]=evaluate_poly(X,e,4);
	X[1]=derx[0];
	X[2]=derx[1]/2;
	X[3]=derx[2]/3;
	X[4]=derx[3]/4;
	if (sigma>0){
		advance_time(q,e,3);
		diffxq[1]=q[1]-X[1];
		diffxq[2]=q[2]-X[2];
		diffxq[3]=q[3]-X[3];
		diffxq[4]=-X[4];
		diffxq[0]=q[0]-X[0]-dQ;
		sigma=minposroot(diffxq,4);
		diffxq[0]=q[0]-X[0]+dQ;
		dt1=minposroot(diffxq,4);
		if (dt1<sigma) sigma=dt1;
		if ((sigma>1e19)&&(X[4]!=0)) {
			//We suspect numerical error, and try with third order roots
			sigma=minposroot(diffxq,3);
			diffxq[0]=q[0]-X[0]-dQ;
			dt1=minposroot(diffxq,3);
			if (dt1<sigma) sigma=dt1;
		};      
		if (fabs(X[0]-q[0])>dQ) {
			sigma=1e-20;
		};       
	}
} else {
	advance_time(X,e,4);
	X[0]=derx[0];
	sigma=0;
}
}
Event qss4::lambda(double t) {
//This function return an event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% is a direction to the variable that contain the value.
//     %NroPort% is the port number (from 0 to n-1)


y[0]=X[0];
y[1]=X[1];
y[2]=X[2];     
y[3]=X[3];     
y[4]=X[4];     
advance_time(y,sigma,4);
y[4]=0;
return Event(&y,0);
}
void qss4::exit() {

}
