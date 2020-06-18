#include "vector_sum.h"
void vector_sum::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//      %Type% is the parameter type

char *fvar= va_arg(parameters,char*);

K=getScilabVar(fvar );
fvar= va_arg(parameters,char*);
n=getScilabVar(fvar );


order=1;

for (int i=0;i<10;i++) {
  for (int j=0;j<10000;j++) {
  		X[j][i]=0;
  };
  y[i]=0;
};

Sigma=INF;
}
double vector_sum::ta(double t) {
//This function returns a double.
return Sigma;
}
void vector_sum::dint(double t) {
Sigma=INF;
}
void vector_sum::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'vec.index' is the port number
vector vec;
double *Xv;
vec=*(vector*)x.value;
Xv=(double*)x.value;
switch(order) {
case 1: 
		X[vec.index][0]=Xv[0];
     if (Xv[1]!=0){order=2;X[vec.index][1]=Xv[1];}
     if (Xv[2]!=0){order=3;X[vec.index][2]=Xv[2];}  
     if (Xv[3]!=0){order=4;X[vec.index][3]=Xv[3];}  

break;
case 2: 
		X[vec.index][0]=Xv[0];
		X[vec.index][1]=Xv[1];

      for (int i=0;i<n;i++) {
      	 if (i!=vec.index) {
				advance_time(X[i],e,1);
			};
		};
     if (Xv[2]!=0){order=3;X[vec.index][2]=Xv[2];}  
     if (Xv[3]!=0){order=4;X[vec.index][3]=Xv[3];}  
break;
case 3: 
		X[vec.index][0]=Xv[0];
		X[vec.index][1]=Xv[1];
		X[vec.index][2]=Xv[2];
      for (int i=0;i<n;i++) {
      	 if (i!=vec.index) {
				advance_time(X[i],e,2);
			};
		};
     if (Xv[3]!=0){order=4;X[vec.index][3]=Xv[3];}  
break;
case 4: 
		X[vec.index][0]=Xv[0];
		X[vec.index][1]=Xv[1];
		X[vec.index][2]=Xv[2];
		X[vec.index][3]=Xv[3];
      for (int i=0;i<n;i++) {
      	 if (i!=vec.index) {
				advance_time(X[i],e,3);
			};
		};
}
Sigma=0;
}
Event vector_sum::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)

for (int j=0;j<order;j++) {
	y[j]=0;
	for (int i=0;i<n;i++) {
		y[j]=y[j]+K*X[i][j];
	};
};
return Event(y,0);
}
void vector_sum::exit() {

}
