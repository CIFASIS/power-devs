#include "cqss.h"
void cqss::init(double t,...) {
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

ep=dQmin/100; 
qs=X[0]+dQ; 
qi=X[0]-dQ; 
q[0]=qi;
eps=1e-20;
band=1;
sigma=0;
}
double cqss::ta(double t) {
//This function return a double.
return sigma;
}
void cqss::dint(double t) {

advance_time(X,sigma,1);
if (sigma>eps) { // X arrives to a new level (q)
	dQ=fabs(X[0])*dQrel; 
	if (dQ<dQmin){dQ=dQmin;};
	ep=dQ/100; 

	if (X[1]>0) {
		qs=qs+dQ;
		q[0]=qs;
		qi=q[0]-2*dQ;
	}else{
		qi=qi-dQ;
		q[0]=qi;
		qs=q[0]+2*dQ;
	}; 
} else { // slope change
	if(X[1]>0) {
		q[0]=qs;
	}else{
	q[0]=qi;
	};
};
if (X[1]!=0) {
	sigma=(q[0]-X[0])/X[1]+2*eps;
} else {
	sigma=INF;
};
band=0;
}

void cqss::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value
//     'x.port' is the port number
double *derx;
//double diffxq[10];
//double dt1;

derx=(double*)x.value;

if (x.port==0) {
	if (e*X[1]!=0) {
		X[0]=X[0]+e*X[1];
		if (X[1]>0) { // we check the correct value of qi;
			if (X[0]-qi>=dQ+ep) qi=qi+dQ;
		} else { //we check the correct value of qs; 
			if (qs-X[0]>=dQ+ep) qs=qs-dQ;
		};
	};
	X[1]=derx[0];
	if (t==0) {// initialization
		if (band==1) { //we need to send the output
			sigma=0;
		} else { //the output was already sent and there was a change in u
			if (X[1]*(q[0]-X[0])>=0) { //q is still ok
				if (X[1]!=0) {
					sigma=(q[0]-X[0])/X[1]+eps;
				} else {
					sigma=INF;
				};
			} else { // q is wrong, but we wait an infinitesimal until changing it 
				sigma=eps;
			};
		};
	} else {
		if ((sigma-e)<eps){
			sigma=2*eps;
		}else{ 
			if (e>0) { //derivative change 
				if (X[1]*(q[0]-X[0])>=0) { // q was ok
					if (X[1]!=0) {
						sigma=(q[0]-X[0])/X[1]+2*eps;
					}else{
						sigma=INF;
					};
				} else { //we need to change q
					/*
					if (met==5){
						sigma=0;
					}else{
					*/
						if (X[1]>0){
							sigma=(qs-X[0])/X[1]+2*eps;
							q[0]=qs;
						}else{
							sigma=(qi-X[0])/X[1]+2*eps;
							q[0]=qi;
						};
					/* }; */
				};
			}else { //we had already sent the value q 
				if (X[1]*(q[0]-X[0])>=0) { // q is still ok 
					if (X[1]!=0) {
						sigma=(q[0]-X[0])/X[1]+eps;
					} else {
						sigma=INF;
					}; 
				} else { // q is wrong, but we are close to u=0, so we set u=0.
					X[1]=0;
					sigma=INF;
				};
			};
		};
	};
} else {
	X[0]=derx[0];
	sigma=0;
	ep=dQ/100;
	//ep=0;
	qs=floor(X[0]/dQ)*dQ+dQ; 
	qi=floor(X[0]/dQ-ep)*dQ; 
	q[0]=qi;
	eps=1e-20;
	band=1;
}
}
Event cqss::lambda(double t) {
//This function return an event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% is a direction to the variable that contain the value.
//     %NroPort% is the port number (from 0 to n-1)

if (sigma<=eps) { //derivative change
	if (X[1]>0){
		y[0]=q[0];
	} else {
		y[0]=qi;
	};
} else { // X arrives to a new level (q)
	if (X[1]>0){
		y[0]=q[0]+dQ;
	}else{
		y[0]=q[0]-dQ;
	};
};
y[0]=(y[0]+X[0]+sigma*X[1])/2;
return Event(&y,0);
}
void cqss::exit() {

}
