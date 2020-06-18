#include "qss_int_vec.h"
void qss_int_vec::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//	%Type% is the parameter type
char *fvar1= va_arg(parameters,char*);
char *fvar2= va_arg(parameters,char*);
char *fvar3= va_arg(parameters,char*);
char *fvar4= va_arg(parameters,char*);
char *fvar5= va_arg(parameters,char*);
char *fvar6= va_arg(parameters,char*);
char *fvar7= va_arg(parameters,char*);
char *fvar8= va_arg(parameters,char*);
double ni= va_arg(parameters,double);
Ni=(int)ni;

char *fvar= va_arg(parameters,char*);
N=getScilabVar(fvar );

       D0 = new Coupling("CoupledWSum");
        Simulator **D1 = new Simulator* [N];
        Connection **EIC1 = new Connection* [N*8];
        Connection **EOC1 = new Connection* [N];
        Connection **IC1 = new Connection* [0];
        for (int i=0;i<N;i++){ 
        	D1[i] = new qss_wsum("qss_sumi");
        	D1[i]->init(t,fvar1,fvar2,fvar3,fvar4,fvar5,fvar6,fvar7,fvar8,ni);
	        for (int j=0;j<Ni;j++){ 
				  EIC1[i*Ni+j] = new Connection();
        		  EIC1[i*Ni+j]->setup(0,j,i,j);
				}
        EOC1[i] = new Connection();
        EOC1[i]->setup(0,i,0,0);
    		}	
      D0->setup(D1,N,IC1,0,EIC1,N,EOC1,N);
      D0->init(t);
}
double qss_int_vec::ta(double t) {
//This function returns a double.
return D0->ta(t);
}
void qss_int_vec::dint(double t) {
D0->dint(t);
}
void qss_int_vec::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
//     'e' is the time elapsed since last transition
vector vec1=*(vector*)x.value;
int index=vec1.index;
if ((index>-1)&&(index<N)){
	D0->D[index]->dextmessage(x,t);
	D0->heap.update(index);
   
} else if (index==-1) {
   for (index=0;index<N;index++){
		D0->D[index]->dextmessage(x,t);
		D0->heap.update(index);
	}
};
}
Event qss_int_vec::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)
y= D0->D[D0->transitionChild]->lambdamessage(t);
vec=*(vector*)y.value;
vec.index=D0->transitionChild;
y.value=&vec;
return y;
}
void qss_int_vec::exit() {
//Code executed at the end of the simulation.
}
