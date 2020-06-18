#include "implicit.h"
void implicit::init(double t,...) {
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
iguess =getScilabVar(fvar );

n = (int)va_arg(parameters,double);
dt=1e-8;
sigma=INF;

for (int i=0;i<10;i++) {
	for (int j=0;j<n;j++) {
		u[j][i]=0;
		uaux[j][i]=0;
	};
   y[i]=0;
	f[i]=0;
};
order=1;

PRVar uvar0=new RVar ( "u0" , &uaux[0][0] );
PRVar uvar1=new RVar ( "u1" , &uaux[1][0] );
PRVar uvar2=new RVar ( "u2" , &uaux[2][0] );
PRVar uvar3=new RVar ( "u3" , &uaux[3][0] );
PRVar uvar4=new RVar ( "u4" , &uaux[4][0] );
PRVar uvar5=new RVar ( "u5" , &uaux[5][0] );
PRVar uvar6=new RVar ( "u6" , &uaux[6][0] );
PRVar uvar7=new RVar ( "u7" , &uaux[7][0] );
PRVar uvar8=new RVar ( "u8" , &uaux[8][0] );
PRVar uvar9=new RVar ( "u9" , &uaux[9][0] );
PRVar uvar10=new RVar ("y" , &f[0] );
PRVar vartab[11];
vartab[0]=uvar10;
vartab[1]=uvar0;
vartab[2]=uvar1;
vartab[3]=uvar2;
vartab[4]=uvar3;
vartab[5]=uvar4;
vartab[6]=uvar5;
vartab[7]=uvar6;
vartab[8]=uvar7;
vartab[9]=uvar8;
vartab[10]=uvar9;

pop=new ROperation ((char*)expre, n+1, (RVar**)vartab);

f[0]=iguess; //initial guess for the output
}
double implicit::ta(double t) {
return sigma;
}
void implicit::dint(double t) {
sigma=INF;
}
void implicit::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value
//     'x.port' is the port number
double *xv,fdt,f2dt,f0;
xv=(double*)(x.value);

switch(order) { 
case 1: 
	u[x.port][0]=xv[0];
	if (xv[1]!=0){order=2;u[x.port][1]=xv[1];}
	if (xv[2]!=0){order=3;u[x.port][2]=xv[2];} 
	if (xv[3]!=0){order=4;u[x.port][3]=xv[3];} 

break;

case 2: 
	u[x.port][0]=xv[0];
	u[x.port][1]=xv[1];
	for (int i=0;i<n;i++) {
		if (i!=x.port) {
			advance_time(u[i],e,1);
		};
	};
//	advance_time(f,e,1); //to improve initial guess
	if (xv[2]!=0){order=3;u[x.port][2]=xv[2];} 
	if (xv[3]!=0){order=4;u[x.port][3]=xv[3];} 

break;

case 3: 
	u[x.port][0]=xv[0];
	u[x.port][1]=xv[1];
	u[x.port][2]=xv[2];
	for (int i=0;i<n;i++) {
		if (i!=x.port) {
			advance_time(u[i],e,2);
		};
	};
//	advance_time(f,e,2); //to improve initial guess
	if (xv[3]!=0){order=4;u[x.port][3]=xv[3];} 

break;

case 4: 
	u[x.port][0]=xv[0];
	u[x.port][1]=xv[1];
	u[x.port][2]=xv[2];
	u[x.port][3]=xv[3];
	for (int i=0;i<n;i++) {
		if (i!=x.port) {
			advance_time(u[i],e,3);
		};
	
	};
//	advance_time(f,e,3); //to improve initial guess

}
sigma=0;
if (e>1e-15){dt=e/100;}

switch(order) {
case 1:
	uaux[x.port][0]=u[x.port][0];
	//printLog("We have uaux[0]= %g, f[0]=%g, tol= %g .",uaux[0][0],f[0],tol);
	f[0]=secant_solve(pop,uaux,&f[0],tol);
	//printLog("We compute f[0]=%g \n",f[0]);

break;

case 2:
	for (int i=0;i<n;i++){
		uaux[i][0]=u[i][0];
		uaux[i][1]=u[i][1];
	};
	f[0]=secant_solve(pop,uaux,&f[0],tol);
   f0=f[0];
	for (int i=0;i<n;i++) {
		advance_time(uaux[i],dt,1);
	};

	f[1]=(secant_solve(pop,uaux,&f[0],tol) - f0)/dt;
	f[0]=f0;
break;

case 3:
	for (int i=0;i<n;i++){
		uaux[i][0]=u[i][0];
		uaux[i][1]=u[i][1];
		uaux[i][2]=u[i][2];
	};
	f[0]=secant_solve(pop,uaux,&f[0],tol);
   f0=f[0];

	for (int i=0;i<n;i++) {
		advance_time(uaux[i],dt,2);
	};
	fdt=secant_solve(pop,uaux,&f[0],tol);
	f[1]=(fdt - f0)/dt;
	for (int i=0;i<n;i++) {
		advance_time(uaux[i],dt,2);
	};
	f[2]=(secant_solve(pop,uaux,&f[0],tol)-2*fdt+f0)/dt/dt/2;
//   printLog("We compute f =[%g %g %g] \n",f[0],f[1],f[2]);
	f[0]=f0;

break;

case 4:
	for (int i=0;i<n;i++){
		uaux[i][0]=u[i][0];
		uaux[i][1]=u[i][1];
		uaux[i][2]=u[i][2];
		uaux[i][3]=u[i][3];
	};
	f[0]=secant_solve(pop,uaux,&f[0],tol);
   f0=f[0];

	for (int i=0;i<n;i++) {
		advance_time(uaux[i],dt,3);
	};
	fdt=secant_solve(pop,uaux,&f[0],tol);
	f[1]=(fdt - f0)/dt;
	for (int i=0;i<n;i++) {
		advance_time(uaux[i],dt,3);
	};
	f2dt=secant_solve(pop,uaux,&f[0],tol);
	f[2]=(f2dt-2*fdt+f0)/dt/dt/2;
	for (int i=0;i<n;i++) {
		advance_time(uaux[i],dt,3);
	};
	f[3]=0*(secant_solve(pop,uaux,&f[0],tol)-3*f2dt+3*fdt-f0)/dt/dt/dt/6;
	f[0]=f0;
   //printLog("We compute f =[%g %g %g %g] \n",f[0],f[1],f[2],f[3]);
};

sigma=0;
}
Event implicit::lambda(double t) {
for (int i=0;i<order;i++) {
  y[i]=f[i];
};
return Event(y,0);
}
void implicit::exit() {

}
