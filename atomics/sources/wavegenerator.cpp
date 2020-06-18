#include "wavegenerator.h"
void wavegenerator::init(double t,...) {
va_list parameters;
va_start(parameters, t);
Type=va_arg(parameters, char*);
A=fabs(va_arg(parameters, double));
f=fabs(va_arg(parameters, double));
Method=va_arg(parameters, char*);
dQ=fabs(va_arg(parameters, double));
Sigma=0;
S[0]=0;
S[1]=0;
Dir=true;
}
double wavegenerator::ta(double t) {
return Sigma;
}
void wavegenerator::dint(double t) {
if (strcmp(Type,"Sine (only QSS)")==0) {
  int n;
  double to;
  if (strcmp(Method,"QSS2")==0) {
    Sigma=4e10;
  };
  if (strcmp(Method,"QSS")==0) {
    n=floor(t*f);
    to=t-n/f;
    if (to<1/(4*f)) {
      Dir=false;
    } else {
      if ((to>=1/(4*f)) && (to<3/(4*f))) {
        Dir=true;
        to=to-1/(2*f);
        S[0]=-S[0];
      } else {
        Dir=false;
        to=to-1/f;
      };
    };
    if ((S[0]+dQ)<=A) {
      Sigma=asin((S[0]+dQ)/A)/(6.2831853*f)-to;
      S[0]=S[0]+dQ;
      if (Dir) {
        S[0]=-S[0];
      };
    } else {
      Sigma=2*(1/(4*f)-to);
      S[0]=-S[0];
       to=-to;
      Sigma=Sigma+asin((S[0]+dQ)/A)/(6.2831853*f)-to;
      S[0]=S[0]+dQ;
      if (Dir==false) {
        S[0]=-S[0];
      };
    };
  };
};
if (strcmp(Type,"Square")==0) {
  if (strcmp(Method,"QSS2")==0) {
    if ((S[0]==A) && (S[1]==0)) {
      S[0]=A-dQ;
      S[1]=-400*f*A;
      Sigma=99/(200*f);
    } else {
      if ((S[0]<A) && (S[1]<0)) {
        S[0]=-A;
        S[1]=0;
        Sigma=1/(200*f);
      } else {
        if ((S[0]==-A) && (S[1]==0)) {
        S[0]=-A+dQ;
        S[1]=400*f*A;
        Sigma=99/(200*f);
        } else {
          if ((S[0]>-A) && (S[1]>0)) {
            S[0]=A;
            S[1]=0;
            Sigma=1/(200*f);
          } else {
            S[0]=A;
            S[1]=0;
            Sigma=1/(200*f);			
          };
        };
      };
    };
  };
  if (strcmp(Method,"QSS")==0) {
    if (((S[0]+dQ)<=A) && ((S[0]-dQ)>=-A)){
      if (Dir==true) { //sube
        S[0]=S[0]+dQ;
        Sigma=dQ/(400*A*f);
      } else { //baja
         S[0]=S[0]-dQ;
         Sigma=dQ/(400*A*f);
       };
    } else {
      if (((S[0]-dQ)<-A) && (Dir==false)) {
        S[0]=S[0]+dQ;
        Dir=true;
        Sigma=99/(200*f)+(A+S[0])/(400*A*f);
      };
      if (((S[0]+dQ)>A) && (Dir==true)) {
        S[0]=S[0]-dQ;
        Dir=false;
        Sigma=99/(200*f)+(A-S[0])/(400*A*f);
      };
    };
  };
};
if (strcmp(Type,"Sawtooth")==0) {
  if (strcmp(Method,"QSS2")==0) {
    if ((S[0]<0) && (S[1]>0)) {
      S[0]=A-dQ*199/200;
      S[1]=-400*A*f;
      Sigma=199/(200*f);
    } else {
      if ((S[0]>0) && (S[1]<0)) {
        S[0]=-A+dQ/200;
        S[1]=400*A*f/199;
        Sigma=1/(200*f);
      } else {
        S[0]=A-199*dQ/200;
        S[1]=-400*A*f;
        Sigma=199/(200*f);
      };
    };
  };
  if (strcmp(Method,"QSS")==0) {
    if (((S[0]+dQ)<=A) && ((S[0]-dQ)>=-A)){
      if (Dir==true) { //sube
        S[0]=S[0]+dQ;
        Sigma=dQ*199/(400*A*f);
      } else { //baja
        S[0]=S[0]-dQ;
        Sigma=dQ/(400*A*f);
      };
    } else {
      if (((S[0]-dQ)<-A) && (Dir==false)) {
        Dir=true;
        Sigma=(A+S[0])/(2*A*f)+dQ/(400*A*f);
        S[0]=S[0]+dQ;
      };
      if (((S[0]+dQ)>A) && (Dir==true)){
        Dir=false;
        Sigma=(A-S[0])/(2*A*f)+dQ/(400*A*f);
        S[0]=S[0]-dQ;
      };
    };
  };
};
if (strcmp(Type,"Triangle")==0) {
  if (strcmp(Method,"QSS2")==0) {
    if ((S[0]<0) && (S[1]>0)) {
      S[0]=A-dQ/2;
      S[1]=-4*A*f;
      Sigma=1/(2*f);
    } else {
      if ((S[0]>0) && (S[1]<0)) {
        S[0]=-A+dQ/2;
        S[1]=4*A*f;
        Sigma=1/(2*f);
      } else {
        S[0]=A-dQ/2;
        S[1]=-4*A*f;
        Sigma=1/(2*f);
      };
    };
  };
  if (strcmp(Method,"QSS")==0) {
    if (((S[0]+dQ)<=A) && ((S[0]-dQ)>=-A)){
      if (Dir==true) { //sube
        S[0]=S[0]+dQ;
        Sigma=dQ/(4*A*f);
      } else { //baja
        S[0]=S[0]-dQ;
        Sigma=dQ/(4*A*f);
      };
    } else {
      if (((S[0]-dQ)<-A) && (Dir==false)) {
        S[0]=S[0]+dQ;
        Dir=true;
        Sigma=dQ/(4*A*f)+((A+S[0])/(4*A*f));
      };
      if (((S[0]+dQ)>A) && (Dir==true)){
        S[0]=S[0]-dQ;
        Dir=false;
        Sigma=dQ/(4*A*f)+((A-S[0])/(4*A*f));
      };
    };
  };
};
}
void wavegenerator::dext(Event x, double t) {

}
Event wavegenerator::lambda(double t) {
return Event(&S[0],0);
}
void wavegenerator::exit() {

}
