#include "conmlogica.h"
void conmlogica::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//	%Type% is the parameter type
char *fvar= va_arg(parameters,char*);

K[0]=getScilabVar(fvar );
fvar= va_arg(parameters,char*);
K[1]=getScilabVar(fvar );
for(int i=0;i<10;i++){
	S[i]=0;
}
inp[0]=0;
inp[1]=0;
inp[3]=0;
band0=false;
band1=false;
maxe=0;
outp=0;
Sigma=0;
}
double conmlogica::ta(double t) {
//This function returns a double.
return Sigma;
}
void conmlogica::dint(double t) {
if(outp==0){
	outp=K[0];
}else{
	if(outp==K[0]){
		outp=K[1];
	}else{
		outp=K[0];
	}	
}
band0=false;
band1=false;
lastmax=inp[1];
maxe=0;
Sigma=1e10;
}
void conmlogica::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
//     'e' is the time elapsed since last transition
double *Xv;
Xv=(double*)x.value;
inp[x.port]=Xv[0];
switch(x.port){
case 0:
	if(fabs(inp[0]-outp)<0.1*fabs(K[0])){band0=true;}
	break;
case 1:
	if(maxe<fabs(lastmax-inp[1])){
		maxe=fabs(lastmax-inp[1]);
	}
	if(band0&&maxe>1.1*(lastmax-inp[1])){
band1=true;
			if(fabs(inp[0]-inp[1])>0){
				band1=true;
			}
	}
break;
}
if(Sigma!=0){
	Sigma=1e10;
	if(band0&&band1){
		Sigma=0;
	}
}
}
Event conmlogica::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)
if(outp==0){
	S[0]=K[0];
}else{
	if(outp==K[0]){
		S[0]=K[1];
	}else{
		S[0]=K[0];
	}	
	
}
return Event(&S[0],0);
}
void conmlogica::exit() {
//Code executed at the end of the simulation.
}
