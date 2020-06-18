#include "vector_pow2.h"
void vector_pow2::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//	%Type% is the parameter type


char *fvar= va_arg(parameters,char*);
N=getScilabVar(fvar );

       D0 = new Coupling("CoupledPow2");
        Simulator **D1 = new Simulator* [N];
        Connection **EIC1 = new Connection* [0];
        Connection **EOC1 = new Connection* [0];
        Connection **IC1 = new Connection* [0];
        for (int i=0;i<N;i++){ 
        	D1[i] = new xpower2("xpower2i");

        	D1[i]->init(t);
    		}	
      D0->setup(D1,N,IC1,0,EIC1,0,EOC1,0);
      D0->init(t);
}
double vector_pow2::ta(double t) {
//This function returns a double.
return D0->ta(t);

}
void vector_pow2::dint(double t) {
D0->dint(t);
}
void vector_pow2::dext(Event x, double t) {
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
  for (int ind=0;ind<N;ind++){
	D0->D[ind]->dextmessage(x,t);
  	D0->heap.update(ind);
 }
};
}
Event vector_pow2::lambda(double t) {
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
void vector_pow2::exit() {
//Code executed at the end of the simulation.
D0->exit();
}
