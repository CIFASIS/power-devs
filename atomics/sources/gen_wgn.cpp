#include "gen_wgn.h"
void gen_wgn::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//      %Type% is the parameter type
mean = va_arg(parameters,double);
variance = va_arg(parameters,double);
sampleperiod = va_arg(parameters,double);
sigma=0; // sigma coincide con el tiempo de vida. Forzamos que salga un evento en t=0
srand(time(NULL)) ;
for(int i=1; i<10; i++) {
  y[i]=0;
};
double i,N; // variables auxiliares de iteracion
double U; // variable auxiliar para distribucion uniforme
u=0;
N=20; // N>=20 (Jeruchim et al.)
for(i=0; i<N; i++) {
U=((double)rand()/((double)(RAND_MAX)+(double)(1))) ; // U=[0,1]
u=u+U ; 
}
// for uniform randoms in [0,1], mu = 0.5 and var = 1/12 
// now we adjust y so mu = 0 and var = 1 
u = u - (N/2);  // set mean to 0 
u = u * sqrt(12 / N); // adjust variance to 1
// adjust variance to user's preference
u = mean + sqrt(variance) * u;
}
double gen_wgn::ta(double t) {
//This function returns a double.
return sigma; // sigma es el tiempo de vida
}
void gen_wgn::dint(double t) {
// el siguiente evento saldra en sampleperiod unidades de tiempo
sigma=sampleperiod;
// se calcula el valor de la amplitud del próximo sample
double i,N; // variables auxiliares de iteracion
double U; // variable auxiliar para distribucion uniforme
u=0;
N=20; // N>=20 (Jeruchim et al.)
for(i=0; i<N; i++) {
U=((double)rand()/((double)(RAND_MAX)+(double)(1))) ; // U=[0,1]
u=u+U ; 
}
// for uniform randoms in [0,1], mu = 0.5 and var = 1/12 
// now we adjust y so mu = 0 and var = 1 
u = u - (N/2);  // set mean to 0 
u = u * sqrt(12 / N); // adjust variance to 1
// adjust variance to user's preference
u = mean + sqrt(variance) * u;
}
void gen_wgn::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
// al no haber entradas, no hace falta poner nada aca.
}
Event gen_wgn::lambda(double t) {
// This function returns an Event:
//     Event(%&Value%, %NroPort%)
// where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)
y[0]=u;
return Event(&y,0);  //sacamos la salida por el puerto "0" (es el primer puerto).
}
void gen_wgn::exit() {

}
