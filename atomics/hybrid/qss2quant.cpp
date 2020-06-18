#include "qss2quant.h"
void qss2quant::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//      %Type% is the parameter type
dq = va_arg(parameters,double);
X = va_arg(parameters,double);
y[0]=0;
y[1]=0;
u=0;
mu=0;
inf=1e10;
sigma=0;
}
double qss2quant::ta(double t) {
//This function return a double.
return sigma;
}
void qss2quant::dint(double t) {
double t1;
  double t2;
  double t0;
  double level;
  double disc;
  X=X+sigma*u+sigma*sigma*mu/2;
  u=u+sigma*mu;
  t0=inf;
  if (u*mu<0) {
 //look for other solution at the actual level
    t0=-u/mu;
  };
  if (u>0) {
    level=int(X/dq+0.0001)*dq+dq;
  }
  else {
    level=int(X/dq+0.0001)*dq-dq;
  };
  disc=u*u-2*(X-level)*mu;
  if (disc<0) {
    t1=inf;
    t2=inf;
  }
  else {
    t1=(-u-sqrt(disc))/mu;
    if (t1<=0) {
      t1=inf;
    };
    t2=(-u+sqrt(disc))/mu;
    if (t2<=0) {
      t2=inf;
    };
  };
  if (t1<=t2) {
    if (t1<=t0) {
      sigma=t1; 
    }
    else {
      sigma=t0; 
    };
 }
  else {
     if (t2<=t0){
       sigma=t2;
     }
     else {
        sigma=t0;
    }; 
 };
}
void qss2quant::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value
//     'x.port' is the port number
double *Input;
double t1;
double t2;
double disc;
double level;
Input=(double*)(x.value);
X=X+u*e+mu*e*e/2;
u=Input[0];
mu=Input[1];
if (sigma==e) {
  sigma=0;
}
else {
level=int(X/dq)*dq;
if (mu==0) {
  if (u==0) {
    t1=inf;
    t2=inf;
  } 
  else {
    t1=(level-X)/u;
    if (t1<=0) {
      t1=inf;
    };
    t2=inf; 
  };
}
else {
  disc=u*u-2*(X-level)*mu;
  if (disc<0) {
    t1=inf;
    t2=inf;
  }
  else {
    t1=(-u-sqrt(disc))/mu;
    if (t1<=0) {
      t1=inf;
    };
    t2=(-u+sqrt(disc))/mu;
    if (t2<=0) {
      t2=inf;
    };
  };
};
if (t1<=t2) {
  sigma=t1;
}
else {
  sigma=t2;
};
level=level+dq;
if (mu==0) {
  if (u==0) {
    t1=inf;
    t2=inf;
  } 
  else {
    t1=(level-X)/u;
    if (t1<=0) {
      t1=inf;
    };
    t2=inf; 
  };
}
else {
  disc=u*u-2*(X-level)*mu;
  if (disc<0) {
    t1=inf;
    t2=inf;
  }
  else {
    t1=(-u-sqrt(disc))/mu;
    if (t1<=0) {
      t1=inf;
    };
    t2=(-u+sqrt(disc))/mu;
    if (t2<=0) {
      t2=inf;
    };
  };
};
if (t1<sigma) {
  sigma=t1;
};
if (t2<sigma) {
  sigma=t2;
};
};
}
Event qss2quant::lambda(double t) {
//This function return an event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% is a direction to the variable that contain the value.
//     %NroPort% is the port number (from 0 to n-1)
if (u+mu*sigma>0){
  y[0]=int((X+u*sigma+mu*sigma*sigma/2)/dq+0.00001)*dq;
}
else {
   y[0]=int((X+u*sigma+mu*sigma*sigma/2)/dq-0.00001)*dq;
};
return Event(&y[0],0);
}
void qss2quant::exit() {

}
