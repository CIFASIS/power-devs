#include "hquantizer_sci.h"
void hquantizer_sci::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//      %Type% is the parameter type

char *fvar= va_arg(parameters,char*);
dq=getScilabVar(fvar );
inf=1e10;
sigma=inf;
xu=0;
xl=0;
for(int i=0;i<10;i++){y[i]=0;};
}
double hquantizer_sci::ta(double t) {
//This function returns a double.
return sigma;
}
void hquantizer_sci::dint(double t) {
if(sigma==0){
	if( (u==xu)&&( (mu>0)||((mu==0)&&(pu>0)) ) ){
		xl=xu;
		xu=xl+dq;
	};
}else{
	if((u+mu*sigma+pu*sigma*sigma)>((xu+xl)/2)){
		xu=xu+dq;
		xl=xl+dq;
	}else{
		xu=xu-dq;
		xl=xl-dq;
	};
	u=u+mu*sigma+pu*sigma*sigma;
	mu=mu+2*pu*sigma;
	if(u<((xu+xl)/2)){u=xl;}else{u=xu;};     //agregado
};
double a, b,c;
double s[2];
double sol[2];
s[0]=s[1]=inf;
sol[0]=sol[1]=inf;
a=pu;
b=mu;
for(int i=0;i<2;i++){
	if(i==0){c=u-xu;}else{c=u-xl;};
	if(a==0){
		if(b!=0){					//QSS2
			s[0]=-c/b;
		}else{					//QSS
			sigma=inf;
		};
	}else{    				//solo aplicable a QSS3
		if(b*b>4*a*c){
			s[0]=(-b+sqrt(b*b-4*a*c))/(2*a);
			s[1]=(-b-sqrt(b*b-4*a*c))/(2*a);
		};
	};
	if( (u==xl) && ( ((b==0)&&(a<0)) || ((sigma==0)&&(b<0)) ) ){
		xl=xl-dq;
		xu=xu-dq;
		s[0]=0;
		s[1]=inf;
	}else{
		if(s[0]<=0){s[0]=inf;};
		if(s[1]<=0){s[1]=inf;};
	};
	if(s[0]<s[1]){sol[i]=s[0];}else{sol[i]=s[1];};
};
if(sol[0]<sol[1]){sigma=sol[0];}else{sigma=sol[1];};
}
void hquantizer_sci::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
double *xv;
double c;
xv=(double*)(x.value);
u=xv[0];
mu=xv[1];
pu=xv[2];
double s[2];
s[0]=s[1]=inf;
if((u>=xu)||(u<xl)){               //CALCULO DE XL Y XU ANTE UN CAMBIO DE NIVEL
	xl=round(floor(u/dq));
	xl=xl*dq;
	xu=xl+dq;
	sigma=0;
}else {     //no hay cambio de nivel y se debe calcular el sigma que generara uno
	double sol[2]; // correspondientes a los cruce inmediatos con xl y xu resp 
	double a=pu;
	double b=mu;
	for(int i=0;i<2;i++){
		if(i==0){c=u-xl;}else{c=u-xu;};
		if(a==0){
			if(b!=0){s[0]=-c/b;};
		}else{
			if(b*b>=4*a*c){
				s[0]=(-b+sqrt(b*b-4*a*c))/(2*a);
				s[1]=(-b-sqrt(b*b-4*a*c))/(2*a);
				if(s[0]<=0){s[0]=inf;};
				if(s[1]<=0){s[1]=inf;};
			};
		};
		if( (u==xl) && ( ((b<=0)&&(a<0)) || (b<0) ) ){
			s[0]=0;
			s[1]=inf;
		}else{
			if(s[0]<=0){s[0]=inf;};
			if(s[1]<=0){s[1]=inf;};
		};
		if(s[0]<s[1]){sol[i]=s[0];}else{sol[i]=s[1];};
	};
	if(sol[0]<sol[1]){sigma=sol[0];}else{sigma=sol[1];};
};
}
Event hquantizer_sci::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)
if(sigma==0){
	y[0]=xl;
}else{
	if((u+mu*sigma+pu*sigma*sigma)>((xu+xl)/2)){
		y[0]=(xl+dq);
	}else{
		y[0]=(xl-dq);
	};
};
return Event(&y[0],0);
}
void hquantizer_sci::exit() {

}
