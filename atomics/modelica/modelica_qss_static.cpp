#include "modelica_qss_static.h"
void modelica_qss_static::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//      %Type% is the parameter type
//We get the parameters here
Inputs = (int)va_arg(parameters,double);
Outputs = (int)va_arg(parameters,double);
Index = (int)va_arg(parameters,double);
Method=int_method();
abs_accuracy=function_abs_acc();
rel_accuracy=function_rel_acc();

//printLog("Setting up static function %i with %i Inputs and %i Outputs\n",Index,Inputs, Outputs);

//Initialization of states and outputs

dt=1e-6; 

for (int i=0;i<10;i++) {
	for (int j=0;j<Inputs;j++) {
		u[j][i]=0;
		uaux[j][i]=0;
	};
	y[i]=0;
	inp[i]=0;
};


if (strcmp(Method,"QSS")==0 || strcmp(Method,"LIQSS")==0) { 
	order=1;
};

if (strcmp(Method,"QSS2")==0 || strcmp(Method,"LIQSS2")==0) {
	order=2;
};
if (strcmp(Method,"QSS3")==0 || strcmp(Method,"LIQSS3")==0 || strcmp(Method,"MIXED_LIQSS3")==0 ){
	order=3;
};
if (strcmp(Method,"QSS4")==0){
	order=4;
};
if ((strcmp(Method,"BQSS")==0)||(strcmp(Method,"CQSS")==0)){
	order=1; 
};
fromext=false;

lastport=Outputs;

sigma=INF;
}
double modelica_qss_static::ta(double t) {
//This function returns a double.
return sigma;
}
void modelica_qss_static::dint(double t) {
double ddf;
fromext=false;
sigma=INF;

}
void modelica_qss_static::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number

for (int i=0;i<Inputs-1;i++) {
	advance_time(u[i],e,order);	 // This is not CORRECT
};
if (x.port!=Inputs-1) { //not a dummy event 
	for (int j=0;j<order;j++) {
		 u[x.port][j]=x.getDouble(j);
	}
  printLog("[t=%g] SF %d: Received {%g,%g} through port %d\n",t,Index,u[x.port][0],u[x.port][1],x.port);
//	if (e>1e-5){dt=e/100;}
} 


sigma=0; 
fromext=true;

lastport=Outputs-1;
}
Event modelica_qss_static::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)

if (fromext) { //We received events

	switch(order) { 

	case 1: 
	case 2: 
	case 3: 
	case 4: 
		for (int i=0;i<Inputs-1;i++) {
			inp[i]=u[i][0];
		}
		function_staticBlocks(Index,t,inp,outp); 
 	  //we evaluated the right hand side...
		for (int i=0;i<Inputs-1;i++) {
			inp[i]=evaluate_poly(u[i],dt,1);
		}
		function_staticBlocks(Index,t+dt,inp,outdt); 
		for (int i=0;i<Inputs-1;i++) {
			inp[i]=evaluate_poly(u[i],-dt,1);
		}
		function_staticBlocks(Index,t-dt,inp,out_dt); 
 		for (int i=0;i<Outputs;i++) {
			f[i][0]=outp[i];
			f[i][1]=(outdt[i]-out_dt[i])/(2*dt);
		} 	
		break;
	}
	lastport=Outputs-1;

} else {

}
printLog("[t=%g] SF: %d Emmiting {%g,%g} through port %d. Input {%g,%g}\n",t,Index,f[lastport][0],f[lastport][1],lastport,u[0][0],u[0][1]);

fromext=false;
for (int i=0;i<10;i++) 
  y[i] = f[lastport][i];
return Event(y,lastport);
}
void modelica_qss_static::exit() {

}
