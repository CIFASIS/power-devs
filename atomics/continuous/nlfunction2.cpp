#include "nlfunction2.h"
void nlfunction2::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//      %Type% is the parameter type
expre = va_arg(parameters,char*);
n = va_arg(parameters,double);
inf=1e20;
dt=1e-8;
sigma=inf;
for (int i=0;i<n;i++) {
  u[i]=0;
  mu[i]=0;
  pu[i]=0;
  uaux[i]=0;
 };
for (int i=0;i<10;i++) {
  y[i]=0;
};
maxord=0;
 f=0;
 mf=0;
 pf=0;
PRVar uvar0=new RVar ( "u0" , &uaux[0] );
PRVar uvar1=new RVar ( "u1" , &uaux[1] );
PRVar uvar2=new RVar ( "u2" , &uaux[2] );
PRVar uvar3=new RVar ( "u3" , &uaux[3] );
PRVar uvar4=new RVar ( "u4" , &uaux[4] );
PRVar uvar5=new RVar ( "u5" , &uaux[5] );
PRVar uvar6=new RVar ( "u6" , &uaux[6] );
PRVar uvar7=new RVar ( "u7" , &uaux[7] );
PRVar uvar8=new RVar ( "u8" , &uaux[8] );
PRVar uvar9=new RVar ( "u9" , &uaux[9] );
PRVar vartab[10];
vartab[0]=uvar0;
vartab[1]=uvar1;
vartab[2]=uvar2;
vartab[3]=uvar3;
vartab[4]=uvar4;
vartab[5]=uvar5;
vartab[6]=uvar6;
vartab[7]=uvar7;
vartab[8]=uvar8;
vartab[9]=uvar9;
pop=new ROperation ((char*)expre, n, (RVar**)vartab);
}
double nlfunction2::ta(double t) {
return sigma;
}
void nlfunction2::dint(double t) {
sigma=inf;
}
void nlfunction2::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value
//     'x.port' is the port number
double *Aux;
int j=x.port;
Aux=(double*)(x.value);

//QSS
if (maxord==0) {
  u[j]=Aux[0];
  uaux[j]=u[j];
  f=pop->Val();
  if (Aux[1]!=0){maxord=1;}  
  if (Aux[2]!=0){maxord=2;}  
};  

//QSS2
if (maxord==1) {
  if (e>0){dt=e/100;}
  for (int i=0;i<n;i++) {
       if (i!=j){
	u[i]=u[i]+mu[i]*e;
              uaux[i]=u[i];
       };
  };
  u[j]=Aux[0];
  uaux[j]=u[j];
  mu[j]=Aux[1];
  f=pop->Val();
  for (int i=0;i<n;i++) {
              uaux[i]=uaux[i]+mu[i]*dt;
  };
  mf=(pop->Val()-f)/dt;
  if (Aux[2]!=0){maxord=2;}
};

//QSS3
if (maxord==2) {
  if (e>0){dt=e/100;}
  for (int i=0;i<n;i++) {
       if (i!=j){
	u[i]=u[i]+mu[i]*e+pu[i]*e*e;
              uaux[i]=u[i];
	mu[i]=mu[i]+2*pu[i]*e;
       };
  };
  u[j]=Aux[0];
  uaux[j]=u[j];
  mu[j]=Aux[1];
  pu[j]=Aux[2];
  f=pop->Val();
  for (int i=0;i<n;i++) {
              uaux[i]=u[i]+mu[i]*dt+pu[i]*dt*dt;
  };
  double fdt=pop->Val();
  mf=(fdt-f)/dt;
  for (int i=0;i<n;i++) {
              uaux[i]=u[i]+mu[i]*2*dt+pu[i]*4*dt*dt;
  };
  double f2dt=pop->Val();
  pf=(f2dt-2*fdt+f)/dt/dt/2;
};
sigma=0;
}
Event nlfunction2::lambda(double t) {
y[0]=f;
y[1]=mf;
y[2]=pf;
return Event(&y[0],0);
}
void nlfunction2::exit() {

}
