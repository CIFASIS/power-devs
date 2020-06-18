#include "ev_count.h"
void ev_count::init(double t,...) {
char* FName;
va_list parameters;
va_start(parameters, t);
FName=va_arg(parameters, char*);
Sigma=va_arg(parameters, double);
n=va_arg(parameters, double);
FOutput = fopen(FName, "w");
fclose(FOutput);
FOutput = fopen(FName, "a");
for (int i=0;i<n;i++){
  k[i]=0;
};
saved=0;
}
double ev_count::ta(double t) {
return Sigma;
}
void ev_count::dint(double t) {
for (int i=0;i<n;i++){
  fprintf(FOutput, "%g\n",k[i]);
};
saved=1;
Sigma=4e10;
}
void ev_count::dext(Event x, double t) {
Sigma=Sigma-e;
k[x.port]=k[x.port]+1;
}
Event ev_count::lambda(double t) {

}
void ev_count::exit() {
if (saved==0){
  for (int i=0;i<n;i++){
    fprintf(FOutput, "%g\n",k[i]);
  };
saved=1;
};
}
