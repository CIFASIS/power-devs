#include "qss_wsum.h"
void qss_wsum::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//      %Type% is the parameter type

char *fvar= va_arg(parameters,char*);

K[0]=getScilabVar(fvar );
fvar= va_arg(parameters,char*);
K[1]=getScilabVar(fvar );
fvar= va_arg(parameters,char*);
K[2]=getScilabVar(fvar );
fvar= va_arg(parameters,char*);
K[3]=getScilabVar(fvar );
fvar= va_arg(parameters,char*);
K[4]=getScilabVar(fvar );
fvar= va_arg(parameters,char*);
K[5]=getScilabVar(fvar );
fvar= va_arg(parameters,char*);
K[6]=getScilabVar(fvar );
fvar= va_arg(parameters,char*);
K[7]=getScilabVar(fvar );
n=(int)  va_arg(parameters,double);


order=1;

for (int i=0;i<10;i++) {
  for (int j=0;j<8;j++) {
  		X[j][i]=0;
  };
  y[i]=0;
};

Sigma=INF;
}
double qss_wsum::ta(double t) {
//This function returns a double.
return Sigma;
}
void qss_wsum::dint(double t) {
Sigma=INF;
}
void qss_wsum::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
double *Xv;
Xv=(double*)x.value;
switch(order) {
case 1: 
		X[x.port][0]=Xv[0];
     if (Xv[1]!=0){order=2;X[x.port][1]=Xv[1];}
     if (Xv[2]!=0){order=3;X[x.port][2]=Xv[2];}  
     if (Xv[3]!=0){order=4;X[x.port][3]=Xv[3];}  

break;
case 2: 
		X[x.port][0]=Xv[0];
		X[x.port][1]=Xv[1];

      for (int i=0;i<n;i++) {
      	 if (i!=x.port) {
				advance_time(X[i],e,1);
			};
		};
     if (Xv[2]!=0){order=3;X[x.port][2]=Xv[2];}  
     if (Xv[3]!=0){order=4;X[x.port][3]=Xv[3];}  
break;
case 3: 
		X[x.port][0]=Xv[0];
		X[x.port][1]=Xv[1];
		X[x.port][2]=Xv[2];
      for (int i=0;i<n;i++) {
      	 if (i!=x.port) {
				advance_time(X[i],e,2);
			};
		};
     if (Xv[3]!=0){order=4;X[x.port][3]=Xv[3];}  
break;
case 4: 
		X[x.port][0]=Xv[0];
		X[x.port][1]=Xv[1];
		X[x.port][2]=Xv[2];
		X[x.port][3]=Xv[3];
      for (int i=0;i<n;i++) {
      	 if (i!=x.port) {
				advance_time(X[i],e,3);
			};
		};
}
Sigma=0;
}
Event qss_wsum::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)

for (int j=0;j<order;j++) {
	y[j]=0;
	for (int i=0;i<n;i++) {
		y[j]=y[j]+K[i]*X[i][j];
	};
};
return Event(y,0);
}
void qss_wsum::exit() {

}
