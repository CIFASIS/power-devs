#include "Trapezoidal.h"
void Trapezoidal::init(double t,...) {
va_list parameters;
va_start(parameters,t);
Vu=va_arg(parameters, double);
Vl=va_arg(parameters, double);
Th=fabs(va_arg(parameters, double));
Tl=fabs(va_arg(parameters, double));
Tup=fabs(va_arg(parameters, double));
Tlw=fabs(va_arg(parameters, double));
Method=va_arg(parameters, char *);
Q=fabs(va_arg(parameters, double));
Sigma=0;
Dir=true;  //Sube
S[0]=Vl;
S[1]=0;
}
double Trapezoidal::ta(double t) {
return Sigma;
}
void Trapezoidal::dint(double t) {
if (strcmp(Method,"QSS")==1) {
  if ((S[0]<Vu) && (S[0]>Vl)){
    if (Dir==true) { //sube
      S[0]=S[0]+Q;
      Sigma=Tup*Q/fabs(Vu-Vl);
    } else { //baja
      S[0]=S[0]-Q;
      Sigma=Tlw*Q/fabs(Vu-Vl);
    };
  } else {
    if (S[0]<=Vl) {
      S[0]=Vl;
      Dir=true;
      Sigma=Tl;
    };
    if (S[0]>=Vu){
      S[0]=Vu;
      Dir=false;
      Sigma=Th;
    };
  };
};
if (strcmp(Method,"QSS2")==1) {
  if ((S[0]==Vu) && (S[1]==0)) {
    S[0]=Vu-Q;
    S[1]=fabs(Vu-Vl)/Tlw;
    Sigma=Tlw;
  } else {
    if ((S[0]<Vu) && (S[1]<0)) {
      S[0]=Vl;
      S[1]=0;
      Sigma=Tl+(fabs(Vu-Vl)/Q)*(-1/Tlw+1/Tup);
    } else {
      if ((S[0]==Vl) && (S[1]==0)) {
        S[0]=Vl+Q;
        S[1]=fabs(Vu-Vl)/Tup;
        Sigma=Tup;
	} else {
        if ((S[0]>Vl) && (S[1]>0)) {
          S[0]=Vu;
          S[1]=0;
          Sigma=Th+(fabs(Vu-Vl)/Q)*(1/Tlw-1/Tup);
        } else {
          S[0]=Vu;
          S[1]=0;
          Sigma=Th+(fabs(Vu-Vl)/Q)*(1/Tlw-1/Tup);			
        };
      };
    };
  };
};
}
void Trapezoidal::dext(Event x, double t) {

}
Event Trapezoidal::lambda(double t) {
return Event(&S[0],0);
}
void Trapezoidal::exit() {

}
