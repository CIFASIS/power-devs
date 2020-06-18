#include "normal_gen_vec.h"
void normal_gen_vec::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//	%Type% is the parameter type
char *fvars[4];
double *parsd[4];
double par[4];
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
       D0 = new Coupling("CoupledNRGEN");
        Simulator **D1 = new Simulator* [N];
        Connection **EIC1 = new Connection* [0];
        Connection **EOC1 = new Connection* [0];
        Connection **IC1 = new Connection* [0];
        for (int i=0;i<N;i++){ 
          sprintf(childi,"NRGEN%i",i);
        	D1[i] = new normal_gen(childi);
				for (int j=0;j<Nargs;j++){
            if (i<Ni[j]) par[j]=parsd[j][i];
				  if (j==3 && strlen(fvars[j])==0)
						sprintf(pars[j],"");
					else
						sprintf(pars[j],"%1.16g",par[j]);
				}	
				
         	D1[i]->init(t,pars[0],pars[1],pars[2],pars[3]);
      	}	
      D0->setup(D1,N,IC1,0,EIC1,0,EOC1,0);
      D0->init(t);
}
double normal_gen_vec::ta(double t) {
//This function returns a double.
return D0->ta(t);
}
void normal_gen_vec::dint(double t) {
D0->dint(t);
}
void normal_gen_vec::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
//     'e' is the time elapsed since last transition

}
Event normal_gen_vec::lambda(double t) {
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
void normal_gen_vec::exit() {
//Code executed at the end of the simulation.
D0->exit();
}
