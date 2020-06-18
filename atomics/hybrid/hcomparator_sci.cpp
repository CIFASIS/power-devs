#include "hcomparator_sci.h"
void hcomparator_sci::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//      %Type% is the parameter type
char *fvar= va_arg(parameters,char*);
Vl= getScilabVar(fvar );
fvar= va_arg(parameters,char*);
Vu= getScilabVar(fvar );
inf=1e20;
for (int i=0;i<4;i++) {
  u[i]=0;
  mu[i]=0;
  pu[i]=0;
};
sigma=inf;
tcross=inf;
sw=0;
for(int i=0;i<10;i++){y[i]=0;};
}
double hcomparator_sci::ta(double t) {
//This function returns a double.
return sigma;
}
void hcomparator_sci::dint(double t) {
if (sigma==0) {
sigma=tcross;
} else {
	for (int i=0;i<2;i++) {
		u[i]=u[i]+mu[i]*sigma+pu[i]*sigma*sigma;
		mu[i]=mu[i]+2*pu[i]*sigma;
	};
	if ((mu[0]-mu[1])*(pu[0]-pu[1])<0) {
		sigma=(mu[1]-mu[0])/(pu[0]-pu[1]);
	} else {
		sigma=inf;
	};
	tcross=sigma;
	if (mu[0]<mu[1]){
		sw=1;
	} else {
		sw=0;
	};
};
}
void hcomparator_sci::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
double *xv;
xv=(double*)x.value;
u[1-x.port]=u[1-x.port]+mu[1-x.port]*e+pu[1-x.port]*e*e;
mu[1-x.port]=mu[1-x.port]+2*pu[1-x.port]*e;
u[x.port]=xv[0];
mu[x.port]=xv[1];
pu[x.port]=xv[2];
//calculate time to next crossing
double a,b,c,s1,s2;
a=pu[0]-pu[1];
b=mu[0]-mu[1];
c=u[0]-u[1];
if (a==0) {
	if (b==0){
		s1=inf;
		s2=inf;
	} else {
		s1=-c/b;
		s2=inf;
	};
} else {
	s1=(-b+sqrt(b*b-4*a*c))/2/a;
	s2=(-b-sqrt(b*b-4*a*c))/2/a;
};
if ((s1>0)&&((s1<s2)||(s2<0))) {
	tcross=s1;
} else {
	if (s2>0) {tcross=s2;} else {tcross=inf;};
};
if (sigma==e) {
sigma=0;
} else {
	if (((u[0]-u[1]>0)&&(sw==1))||((u[0]-u[1]<0)&&(sw==0))){
		sw=1-sw;
		sigma=0;
	} else {
		sigma=tcross; 
	};
};
}
Event hcomparator_sci::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)
if (sigma==0) {
	if(sw==1){y[0]=Vl;}else{y[0]=Vu;};
} else {
	if((1-sw)==1){y[0]=Vl;}else{y[0]=Vu;};
};
return Event(&y[0],0);
}
void hcomparator_sci::exit() {

}
