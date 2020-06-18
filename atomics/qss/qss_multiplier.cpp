#include "qss_multiplier.h"
void qss_multiplier::init(double t,...) {
va_list parameters;
va_start(parameters, t);
for(int i=0;i<10;i++){
   y[i]=0;
	u[0][i]=0;
	u[1][i]=0;
};
sigma=INF;
order=1;
}
double qss_multiplier::ta(double t) {
return sigma;
}
void qss_multiplier::dint(double t) {
sigma=INF;
}
void qss_multiplier::dext(Event x, double t) {
double *xv;
xv=(double*)(x.value);

switch(order){ 
case 1: 
	u[x.port][0]=xv[0];
	if (xv[1]!=0){order=2;u[x.port][1]=xv[1];}
	if (xv[2]!=0){order=3;u[x.port][2]=xv[2];} 
	if (xv[3]!=0){order=4;u[x.port][3]=xv[3];} 

break;
case 2: 
	u[x.port][0]=xv[0];
	u[x.port][1]=xv[1];
	advance_time(u[1-x.port],e,1);
	if (xv[2]!=0){order=3;u[x.port][2]=xv[2];} 
	if (xv[3]!=0){order=4;u[x.port][3]=xv[3];} 
break;
case 3:
	u[x.port][0]=xv[0];
	u[x.port][1]=xv[1];
	u[x.port][2]=xv[2];
	advance_time(u[1-x.port],e,2);

	if (xv[3]!=0){order=4;u[x.port][3]=xv[3];} 
break;
case 4:
	u[x.port][0]=xv[0];
	u[x.port][1]=xv[1];
	u[x.port][2]=xv[2];
	u[x.port][3]=xv[3];
	advance_time(u[1-x.port],e,3);

}

sigma=0;
}
Event qss_multiplier::lambda(double t) {
switch(order){ 
case 1:
	y[0]=u[0][0]*u[1][0];
break;

case 2:
	y[0]=u[0][0]*u[1][0];
	y[1]=u[0][0]*u[1][1]+u[0][1]*u[1][0];

break;

case 3:
	y[0]=u[0][0]*u[1][0];
	y[1]=u[0][0]*u[1][1]+u[0][1]*u[1][0];
	y[2]=u[0][0]*u[1][2]+u[0][1]*u[1][1]+u[0][2]*u[1][0];
break;

case 4:
	y[0]=u[0][0]*u[1][0];
	y[1]=u[0][0]*u[1][1]+u[0][1]*u[1][0];
	y[2]=u[0][0]*u[1][2]+u[0][1]*u[1][1]+u[0][2]*u[1][0];
	y[3]=u[0][0]*u[1][3]+u[0][1]*u[1][2]+u[0][2]*u[1][1]+u[0][3]*u[1][0];
}
return Event(y,0);
}
void qss_multiplier::exit() {

}
