#include "impfunction_sci.h"
void impfunction_sci::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//      %Type% is the parameter type
expre = va_arg(parameters,char*);
char *fvar= va_arg(parameters,char*);
tol=getScilabVar(fvar );

fvar= va_arg(parameters,char*);
u[0]=getScilabVar(fvar );


n = (int)va_arg(parameters,double);
sigma=INF;
for(int i=0;i<10;i++){y[i]=0;};
for (int i=1;i<n+1;i++) {
	u[i]=0;
	mu[i]=0;
	pu[i]=0;
};
mu[0]=0;
pu[0]=0;
nm=0;
PRVar uvar1=new RVar ( "u0" , &u[1] );
PRVar uvar2=new RVar ( "u1" , &u[2] );
PRVar uvar3=new RVar ( "u2" , &u[3] );
PRVar uvar4=new RVar ( "u3" , &u[4] );
PRVar uvar5=new RVar ( "u4" , &u[5] );
PRVar uvar6=new RVar ( "u5" , &u[6] );
PRVar uvar7=new RVar ( "u6" , &u[7] );
PRVar uvar8=new RVar ( "u7" , &u[8] );
PRVar uvar9=new RVar ( "u8" , &u[9] );
PRVar uvar10=new RVar ( "u9" , &u[10] );
PRVar uvar0=new RVar ("y" , &u[0]);
PRVar vartab[11];
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
vartab[10]=uvar10;
pop=new ROperation ((char*)expre, n+1, (RVar**)vartab);
}
double impfunction_sci::ta(double t) {
//This function return a double.
return sigma;
}
void impfunction_sci::dint(double t) {
sigma=INF;
}
void impfunction_sci::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value
//     'x.port' is the port number
double *Aux;
int j;
double g1;
double g2;
double uv;
double zv;
double z1;
double z2;
double z3;
double dz;
double dx;
double gv1;
double zv1;
Aux=(double*)(x.value);
if (Aux[2]!=0) nm=1;
j=x.port;
for (int i=1;i<n+1;i++) {
    u[i]=u[i]+mu[i]*e;
};
uv=u[j+1];
zv=u[0]+mu[0]*e;
u[j+1]=Aux[0];
mu[j+1]=Aux[1];
pu[j+1]=Aux[2];
//secant method
u[0]=u[0]+tol;
z1=u[0];
g1=pop->Val();
gv1=g1;
zv1=z1;
if (g1=0) {
  z2=z1;
  g2=g1;
} 
else {
  z2=zv;
  u[0]=zv;
  g2=pop->Val();
};
for (int i=0;i<10;i++) {
  if (fabs(g2)<tol) {
     i=10;
  }
  else {
    z3 = 1 / (g1 - g2) * (g1 * z2 - g2 * z1);
    z1 = z2;
    z2 = z3;
    g1 = g2; 
    u[0]=z2;
    g2=pop->Val();
  };
};
//u[0] has the correct value.
//-------------------NEW---------------------------------
double y0;
y0=pop->Val();
double uAux1;
double uAux2;
double ddFun[n+1][n+1];
double dFun[n+1];
double deltaU[n+1];
for(int i=0;i<n+1;i++){
	if((e*mu[i]+e*e*pu[i])!=0){
		deltaU[i]=(e*mu[i]+e*e*pu[i])/10;
	}else{
		deltaU[i]=0.001;
	};
};
ini=0;
uAux1=u[0];
for(int n1=0;n1<n+1;n1++){
	uAux1=u[n1];
	u[n1]=u[n1]+deltaU[n1];
	dFun[n1]=pop->Val();
	for(int n2=ini;n2<n+1;n2++){
		uAux2=u[n2];
		u[n2]=u[n2]+deltaU[n2];
		ddFun[n1][n2]=pop->Val();
		ddFun[n2][n1]=ddFun[n1][n2];
		u[n2]=uAux2;
	};
	u[n1]=uAux1;
	ini++;
};
double crosder;
double crosder1;
double crosder2;
double borrar;
for(int i=0;i<n+1;i++){
	if (deltaU[i]!=0) {g[i]=(y0-dFun[i])/-deltaU[i];};
};
mu[0]=0;
double dercruz[n+1][n+1];
for(int i=1;i<n+1;i++){mu[0]=mu[0]+g[i]*mu[i];};
if(g[0]!=0){mu[0]=-mu[0]/g[0];};
for(int i=0;i<n+1;i++){
	h[i]=0;
	for(int k=0;k<n+1;k++){
		crosder1=(ddFun[i][k]-dFun[k])/deltaU[i];
		crosder2=(dFun[i]-y0)/deltaU[i];
		crosder=(crosder1-crosder2)/deltaU[k];
		dercruz[i][k]=(crosder1-crosder2)/deltaU[k];
		h[i]=h[i]+mu[k]*crosder;
	};
};
pu[0]=0;
for(int i=1;i<n+1;i++){pu[0]=pu[0]+h[i]*mu[i]+2*g[i]*pu[i];};
if (nm==1) pu[0]=pu[0]+h[0]*mu[0];
if(g[0]!=0){pu[0]=-pu[0]/(2*g[0]);};
sigma=0;
}
Event impfunction_sci::lambda(double t) {
//This function return an event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% is a direction to the variable that contain the value.
//     %NroPort% is the port number (from 0 to n-1)
y[0]=u[0];
y[1]=mu[0];
y[2]=pu[0];
return Event(&y[0],0);
}
void impfunction_sci::exit() {

}
