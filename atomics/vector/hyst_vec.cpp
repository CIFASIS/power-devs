#include "hyst_vec.h"
void hyst_vec::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//	%Type% is the parameter type

char *fvars[4];
double *parsd[4];
double par[4] = {0};
char childi[24];
int Ni[4];
int Nargs=4;
for (int i=0;i<Nargs;i++)fvars[i]= va_arg(parameters,char*);
char *fvar= va_arg(parameters,char*);
N=getScilabVar(fvar);
char pars[4][24];
for (int i=0;i<Nargs;i++){
 parsd[i]=(double*)calloc(N,sizeof(double));
 getScilabVector(fvars[i],&Ni[i],parsd[i]);
}
       D0 = new Coupling("CoupledHyst");
        Simulator **D1 = new Simulator* [N];
        Connection **EIC1 = new Connection* [0];
        Connection **EOC1 = new Connection* [0];
        Connection **IC1 = new Connection* [0];
        for (int i=0;i<N;i++){ 
          sprintf(childi,"hysteretic%i",i);
        	D1[i] = new hysteretic(childi);
				for (int j=0;j<Nargs;j++){
            if (i<Ni[j]) par[j]=parsd[j][i];
					sprintf(pars[j],"%1.16g",par[j]);
				}	
         	D1[i]->init(t,pars[0],pars[1],pars[2],pars[3]);
      	}	
      D0->setup(D1,N,IC1,0,EIC1,0,EOC1,0);
      D0->init(t);



}
double hyst_vec::ta(double t) {
//This function returns a double.
return D0->ta(t);
}
void hyst_vec::dint(double t) {
D0->dint(t);
}
void hyst_vec::dext(Event x, double t) {
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
Event hyst_vec::lambda(double t) {
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
void hyst_vec::exit() {
//Code executed at the end of the simulation.
D0->exit();
}
