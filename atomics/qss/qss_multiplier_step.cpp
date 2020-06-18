#include "qss_multiplier_step.h"
void qss_multiplier_step::init(double t,...) {
va_list parameters;
va_start(parameters, t);

char *advance_method_str=va_arg(parameters,char*);
purely_static=strcmp(advance_method_str,"Purely static") == 0;
char *dQrel_str=va_arg(parameters,char*);
char *dQmin_str=va_arg(parameters,char*);
dQrel = getScilabVar(dQrel_str);
dQmin = getScilabVar(dQmin_str);

for(int i=0;i<10;i++){
   y[i]=0;
	u[0][i]=0;
	u[1][i]=0;
};
sigma=INF;
order=1;
}
double qss_multiplier_step::ta(double t) {
return sigma;
}
void qss_multiplier_step::dint(double t) {
sigma=INF;
if (purely_static)
	return;
switch(order)
{
	case 1:
		sigma=INF;
		return;
	case 2:
		y_n1=u[0][0]*u[1][2]+u[0][1]*u[1][1]+u[0][2]*u[1][0];
		y_n2=u[0][0]*u[1][3]+u[0][1]*u[1][2]+u[0][2]*u[1][1]+u[0][3]*u[1][0];
		break;
	case 3:
		y_n1=u[0][0]*u[1][3]+u[0][1]*u[1][2]+u[0][2]*u[1][1]+u[0][3]*u[1][0];
		y_n2=u[0][0]*u[1][4]+u[1][1]*u[1][3]+u[0][2]*u[1][2]+u[0][3]*u[1][1]+u[0][4]*u[1][0];
		break;
	case 4:
		y_n1=u[0][0]*u[1][4]+u[1][1]*u[1][3]+u[0][2]*u[1][2]+u[0][3]*u[1][1]+u[0][4]*u[1][0];
		y_n2=u[0][0]*u[1][5]+u[0][1]*u[1][4]+u[0][2]*u[1][3]+u[0][3]*u[1][2]+u[0][4]*u[1][1]+u[0][5]*u[1][0];
		break;
}

tol=fabs(y[0])*dQrel;
if (tol<dQmin)
	tol=dQmin;
sigma=estimate_sigma_prod(y_n1, y_n2, order, tol, u, y); 


}
void qss_multiplier_step::dext(Event x, double t) {
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
Event qss_multiplier_step::lambda(double t) {
advance_time(u[0],sigma,order);
advance_time(u[1],sigma,order);
advance_time(y,sigma,order);
if (sigma && fabs(u[0][0]*u[1][0]-y[0])>tol)
	printLog("[t=%g] Diff u[0]={%g %g %g} u[1]={%g %g %g} g(t)-y(t)=%g\n",t,u[0][0],u[0][1],u[0][2],u[1][0],u[1][1],u[1][2],fabs(u[0][0]*u[1][0]-y[0])) ; 
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
void qss_multiplier_step::exit() {

}
