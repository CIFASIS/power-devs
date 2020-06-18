#include "qss_nlfunction_step.h"
void qss_nlfunction_step::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//      %Type% is the parameter type

expre = va_arg(parameters,char*);
n = (int)va_arg(parameters,double);

purely_static=strcmp(va_arg(parameters,char*),"Purely static")==0;
dQmin=getScilabVar(va_arg(parameters,char*));
dQrel=getScilabVar(va_arg(parameters,char*)); 

dt=1e-5;
sigma=INF;

for (int i=0;i<10;i++) {
	for (int j=0;j<n;j++) {
		u[j][i]=0;
		uaux[j][i]=0;
	};
   y[i]=0;
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
double qss_nlfunction_step::ta(double t) {
return sigma;
}
void qss_nlfunction_step::dint(double t) {

if (purely_static) {
	sigma=INF;
	return;
} 

// Estimate advance time
// First compute the next two coefficients y_n+1 and y_n+2
switch (order)
{
	case 1:
		sigma=INF;
		return;
	case 2:
  	{
		for (int i=0;i<n;i++) {
			advance_time(uaux[i],2*dt,1);
		};
	    f2dt=pop->Val(); // Evaluation of f(x+2dt)
		for (int i=0;i<n;i++) {
			advance_time(uaux[i],-4*dt,1);
		};
   		f_2dt=pop->Val(); // Evaluation of f(x-2dt)

		y_n1=(fdt-2*f0+f_dt)/(dt*dt*2);
		y_n2=(f2dt-2*fdt+2*f_dt-f_2dt)/(dt*dt*dt*12);

		for (int i=0;i<n;i++){ // Take back the changes
			uaux[i][0]=u[i][0];
		  	uaux[i][1]=u[i][1];
		};
  }
		break;
  case 3:
  {
		for (int i=0;i<n;i++) {
			advance_time(uaux[i],2*dt,2);
		};
    f2dt=pop->Val(); // Evaluation of f(x+2dt)
		for (int i=0;i<n;i++) {
			advance_time(uaux[i],-4*dt,2);
		};
    f_2dt=pop->Val(); // Evaluation of f(x-2dt)


		y_n1=(f2dt-2*fdt+2*f_dt-f_2dt)/(dt*dt*dt*12);
		y_n2=(f2dt-4*fdt+6*f0-4*f_dt+f_2dt)/(dt*dt*dt*dt*24);
		for (int i=0;i<n;i++){ // Take back the changes
			uaux[i][0]=u[i][0];
		  uaux[i][1]=u[i][1];
		};
  }
	break;
 
  case 4:
  {
		for (int i=0;i<n;i++) {
			advance_time(uaux[i],2*dt,3);
		};
    f2dt=pop->Val(); // Evaluation of f(x+2dt)
		for (int i=0;i<n;i++) {
			advance_time(uaux[i],dt,3);
		};
    f3dt=pop->Val(); // Evaluation of f(x+3dt)
		for (int i=0;i<n;i++) {
			advance_time(uaux[i],-5*dt,3);
		};
    f_2dt=pop->Val(); // Evaluation of f(x-2dt)
		for (int i=0;i<n;i++) {
			advance_time(uaux[i],-dt,3);
		};
    f_3dt=pop->Val(); // Evaluation of f(x-3dt)

  	y_n1=(f2dt-4*fdt+6*f0-4*f_dt+f_2dt)/(dt*dt*dt*dt*24);
		y_n2=(f3dt-4*f2dt+5*fdt-5*f_dt+4*f_2dt-f_3dt)/(dt*dt*dt*dt*dt*240);

		for (int i=0;i<n;i++){ // Take back the changes
			uaux[i][0]=u[i][0];
		  uaux[i][1]=u[i][1];
		};
  }
	break;
}

// Now estimate sigma
tol=fabs(y[0])*dQrel;
if (tol<dQmin)
	tol=dQmin;
printLog("[t=%g] -2dt=%g -dt=%g f0=%g dt=%g 2dt=%g\n",t,f_2dt,f_dt,f0,fdt,f2dt);
double eps=1e-20;
if (fabs(y_n1)>eps) {
	sigma=.8* pow(fabs(tol/y_n1),1.0/order);
	printLog("[t=%g] With first coefficient sigma=%g y_n1=%g\n",t,sigma,y_n1);
}   
if (fabs(y_n2)>eps) { 
   printLog("[t=%g] With second coefficient sigma=%g y_n2=%g tol=%g\n",t,.8*pow(fabs(tol/y_n2),1.0/(order+1)),y_n2,tol);
   if (.8*pow(fabs(tol/y_n2),1.0/(order+1))<sigma)
    sigma=.8*pow(fabs(tol/y_n2),1.0/(order+1));
}

for (int i=0;i<n;i++){ // Take back the changes
  uaux[i][0]=u[i][0];
  uaux[i][1]=u[i][1];
};
for (int i=0;i<n;i++) {
  advance_time(uaux[i],sigma,order);
}
double err=fabs(pop->Val()-evaluate_poly(y,sigma,order));
for (int i=0;i<n;i++){ // Take back the changes
  uaux[i][0]=u[i][0];
  uaux[i][1]=u[i][1];
};

while (fabs(sigma)>eps && err>tol)
{
  sigma*=.8*pow(fabs(tol/err),1.0/order);
  for (int i=0;i<n;i++) {
    advance_time(uaux[i],sigma,order);
  }
  err=fabs(pop->Val()-evaluate_poly(y,sigma,order));
  for (int i=0;i<n;i++){ // Take back the changes
    uaux[i][0]=u[i][0];
    uaux[i][1]=u[i][1];
  };
  printLog("[t=%g] With iteration sigma=%g\n",sigma);
}

if (fabs(sigma)<eps || err>tol) { 
	sigma = 1e-10;
	for (int i=0;i<n;i++){ // Take back the changes
  		uaux[i][0]=u[i][0];
  		uaux[i][1]=u[i][1];
	};
	for (int i=0;i<n;i++) {
	  advance_time(uaux[i],sigma,order);
	}
   unsigned iter=0,max_iter=100;
   while (fabs(pop->Val()-evaluate_poly(y,2*sigma,order))<tol && iter<max_iter) {
      sigma*=2;
	   iter++;
		for (int i=0;i<n;i++){ // Take back the changes
  			uaux[i][0]=u[i][0];
			uaux[i][1]=u[i][1];
		};
		for (int i=0;i<n;i++) {
		  advance_time(uaux[i],2*sigma,order);
		}
    }   
    printLog("Using a bisection step sigma=%g \n",sigma);    
  }


printLog("[t=%g] sigma=%g\n",t,sigma);
}
void qss_nlfunction_step::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value
//     'x.port' is the port number
double *xv;
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
}
sigma=0;
if (e>1e-15){dt=e/100;}

switch(order) {
case 1:
	uaux[x.port][0]=u[x.port][0];
	f[0]=pop->Val();
break;

case 2:
  {
	for (int i=0;i<n;i++){
		uaux[i][0]=u[i][0];
		uaux[i][1]=u[i][1];
	};
	f0=pop->Val();
	for (int i=0;i<n;i++) {
		advance_time(uaux[i],dt,1);
	};
  fdt=pop->Val();
	for (int i=0;i<n;i++) {
		advance_time(uaux[i],-2*dt,1);
	};
  f_dt=pop->Val();
  f[0]=f0;
	f[1]=(fdt-f_dt)/(2*dt);
	for (int i=0;i<n;i++){ // Take back the changes
		uaux[i][0]=u[i][0];
		uaux[i][1]=u[i][1];
	};
  }
break;

case 3:
  {
	for (int i=0;i<n;i++){
		uaux[i][0]=u[i][0];
		uaux[i][1]=u[i][1];
		uaux[i][2]=u[i][2];
	};
	f0=pop->Val();
	for (int i=0;i<n;i++) {
		advance_time(uaux[i],dt,2);
	};
	fdt=pop->Val();
	for (int i=0;i<n;i++) {
		advance_time(uaux[i],-2*dt,2);
	};
	f_dt=pop->Val();
  f[0]=f0;
	f[1]=(fdt-f_dt)/2*dt;
	f[2]=(fdt-2*f0+f_dt)/(dt*dt*2);
	for (int i=0;i<n;i++){ // Take back the changes
		uaux[i][0]=u[i][0];
		uaux[i][1]=u[i][1];
	};

  }
break;

case 4:
  {
	for (int i=0;i<n;i++){
		uaux[i][0]=u[i][0];
		uaux[i][1]=u[i][1];
		uaux[i][2]=u[i][2];
		uaux[i][3]=u[i][3];
	};
	f0=pop->Val(); // Evaluation of f(t)
	for (int i=0;i<n;i++) {
		advance_time(uaux[i],dt,3);
	};
	fdt=pop->Val(); // Evaluation of f(t+dt)
	for (int i=0;i<n;i++) {
		advance_time(uaux[i],dt,3);
	};
	f2dt=pop->Val(); // Evaluation of f(t+2dt)
	for (int i=0;i<n;i++) {
		advance_time(uaux[i],-3*dt,3);
	};
	f_dt=pop->Val(); // Evaluation of f(t-dt)
	for (int i=0;i<n;i++) {
		advance_time(uaux[i],-dt,3);
	};
	f_2dt=pop->Val(); // Evaluation of f(t-2dt)

  f[0]=f0;
	f[1]=(fdt - f_dt)/2*dt;
	f[2]=(fdt-2*f0+f_dt)/(dt*dt*2);
	f[3]=(f2dt-2*fdt+2*f_dt-f_2dt)/(12*dt*dt*dt);
	for (int i=0;i<n;i++){ // Take back the changes
		uaux[i][0]=u[i][0];
		uaux[i][1]=u[i][1];
	};
  }
};


sigma=0;
}
Event qss_nlfunction_step::lambda(double t) {
advance_time(y,sigma,order);
for (int i=0;i<n;i++) {
	advance_time(u[i],sigma,order);
};

if (e>1e-15){dt=e/100;}

switch(order) {
case 1:
	for (int i=0;i<n;i++)
	  uaux[i][0]=u[i][0];
	f0=pop->Val(); // Evaluation of f(t)
	f[0]=f0;
break;

case 2:
  {
	for (int i=0;i<n;i++){
		uaux[i][0]=u[i][0];
		uaux[i][1]=u[i][1];
	};
  f0=pop->Val(); // Evaluation of f(t)
	for (int i=0;i<n;i++) {
		advance_time(uaux[i],dt,1);
	};
   fdt=pop->Val(); // Evaluation of f(t+dt)
	for (int i=0;i<n;i++) {
		advance_time(uaux[i],-2*dt,1);
	};
   f_dt=pop->Val(); // Evaluation of f(t-dt)
	
   f[0]=f0;
	f[1]=(fdt-f_dt)/(2*dt);

	for (int i=0;i<n;i++){ // Take back the changes
		uaux[i][0]=u[i][0];
		uaux[i][1]=u[i][1];
	};
 } 
break;

case 3:
  {
	for (int i=0;i<n;i++){
		uaux[i][0]=u[i][0];
		uaux[i][1]=u[i][1];
		uaux[i][2]=u[i][2];
	};
	f0=pop->Val(); // Evaluation of f(t)
	for (int i=0;i<n;i++) {
		advance_time(uaux[i],dt,2);
	};
	fdt=pop->Val(); // Evaluation of f(t+dt)
	for (int i=0;i<n;i++) {
		advance_time(uaux[i],-2*dt,2);
	};
	f_dt=pop->Val();// Evaluation of f(t-dt)
	f[0]=f0;
	f[1]=(fdt - f_dt)/(2*dt);
	f[2]=(fdt-2*f0+f_dt)/(2*dt*dt);

	for (int i=0;i<n;i++){ // Take back the changes
		uaux[i][0]=u[i][0];
		uaux[i][1]=u[i][1];
		uaux[i][2]=u[i][2];
	};
  }

break;

case 4:
  {
	for (int i=0;i<n;i++){
		uaux[i][0]=u[i][0];
		uaux[i][1]=u[i][1];
		uaux[i][2]=u[i][2];
		uaux[i][3]=u[i][3];
	};
	f0=pop->Val(); // Evaluation of f(t)
	for (int i=0;i<n;i++) {
		advance_time(uaux[i],dt,3);
	};
	fdt=pop->Val();// Evaluation of f(t+dt)
	for (int i=0;i<n;i++) {
		advance_time(uaux[i],dt,3);
	};
	f2dt=pop->Val();// Evaluation of f(t+2dt)
	for (int i=0;i<n;i++) {
		advance_time(uaux[i],-3*dt,3);
	};
  f_dt=pop->Val();// Evaluation of f(t-dt)
	for (int i=0;i<n;i++) {
		advance_time(uaux[i],-dt,3);
	};
  f_2dt=pop->Val();// Evaluation of f(t-2dt)

	f[0]=f0;
	f[1]=(fdt-f_dt)/(2*dt);
	f[2]=(fdt-2*f0+f_dt)/(dt*dt*2);
	f[3]=(f2dt-2*fdt+2*f_dt-f_2dt)/(dt*dt*dt*12);
	for (int i=0;i<n;i++){ // Take back the changes
		uaux[i][0]=u[i][0];
		uaux[i][1]=u[i][1];
		uaux[i][2]=u[i][2];
		uaux[i][3]=u[i][3];
	};
  }
};

if (fabs(f[0]-y[0])>tol && sigma)
  printLog("[t=%g] Diff=%g (tol=%g)\n",t,f[0]-y[0],tol);

for (int i=0;i<order;i++) {
  y[i]=f[i];
};
return Event(y,0);
}
void qss_nlfunction_step::exit() {

}
