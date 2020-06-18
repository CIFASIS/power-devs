#include "xpowern.h"
void xpowern::init(double t,...) {
va_list parameters;
va_start(parameters, t);
char *fvar= va_arg(parameters,char*);
n=(int)getScilabVar(fvar );

sigma=INF;
for (int i=0;i<10;i++){
	y[i]=0;
	u[i]=0;
};
order=1;
}
double xpowern::ta(double t) {
return sigma;
}
void xpowern::dint(double t) {
sigma=INF;
}
void xpowern::dext(Event x, double t) {
double *xv; 
xv=(double*)(x.value);

switch(order){ 
case 1: 
u[0]=xv[0];
if (xv[1]!=0){order=2;u[1]=xv[1];}
if (xv[2]!=0){order=3;u[2]=xv[2];} 
if (xv[3]!=0){order=4;u[3]=xv[3];} 

break;
case 2: 
u[0]=xv[0];
u[1]=xv[1];
if (xv[2]!=0){order=3;u[2]=xv[2];} 
if (xv[3]!=0){order=4;u[3]=xv[3];} 
break;
case 3:
u[0]=xv[0];
u[1]=xv[1];
u[2]=xv[2];
if (xv[3]!=0){order=4;u[3]=xv[3];} 
break;
case 4:
u[0]=xv[0];
u[1]=xv[1];
u[2]=xv[2];
u[3]=xv[3];
}
sigma=0;
}
Event xpowern::lambda(double t) {
switch(order){ 
case 1: 
	y[0]=pow(u[0],n);
break;
case 2: 
	
	y[0]=pow(u[0],n);
	y[1]=n*pow(u[0],n-1)*u[1];
break;
case 3:
	y[0]=pow(u[0],n);
	y[1]=n*pow(u[0],n-1)*u[1];
	y[2]=n*(n-1)*pow(u[0],n-2)*u[1]*u[1]/2+n*pow(u[0],n-1)*u[2];
break;
case 4:
	y[0]=pow(u[0],n);
	y[1]=n*pow(u[0],n-1)*u[1];
	y[2]=n*(n-1)*pow(u[0],n-2)*u[1]*u[1]/2+n*pow(u[0],n-1)*u[2];
	y[3]=n*(n-1)*(n-2)*pow(u[0],n-3)*u[1]*u[1]*u[1]/6+n*(n-1)*pow(u[0],n-2)*u[1]*u[2]+n*pow(u[0],n-1)*u[3];//arreglar
} 
return Event(y,0);

}
void xpowern::exit() {

}
