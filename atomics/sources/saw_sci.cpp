#include "saw_sci.h"
void saw_sci::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);

char *fvar=va_arg( parameters, char*);
A=getScilabVar(fvar );
fvar=va_arg( parameters, char*);
F=getScilabVar(fvar );
pend=va_arg(parameters,char*);
Method=va_arg(parameters,char*);
fvar=va_arg( parameters, char*);
dq=getScilabVar(fvar );

for(int i=0;i<10;i++){y[i]=0;};
S[0]=-A;
if(strcmp(Method,"QSS")==0){
	S[1]=0;
	//sigma = dq/(2*A*F);
}else{
	S[1]=2*A*F;
	//sigma=1/F;
};
sigma=0;
}
double saw_sci::ta(double t) {
//This function returns a double.
return sigma;
}
void saw_sci::dint(double t) {
if(strcmp(Method,"QSS")==0){
	if(S[0]+dq<=A){
		sigma = dq/(2*A*F);
		S[0]=S[0]+dq;
	}else{
		if(S[0]<A){
			S[0]=A;
			sigma=(A-S[0])/(2*A*F);
		}else{
			S[0]=-A;
			sigma=0;
		};
	};	
}else{
	sigma=1/F;
};
}
void saw_sci::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
}
Event saw_sci::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)
if(strcmp(pend,"Negative")==0){
	y[0]=-S[0];
	y[1]=-S[1];
}else{
	y[0]=S[0];
	y[1]=S[1];
};
return Event(&y[0],0);
}
void saw_sci::exit() {

}
