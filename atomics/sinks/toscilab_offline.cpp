#include "toscilab_offline.h"
void toscilab_offline::init(double t,...) {
va_list parameters;
va_start(parameters, t);
time_var = va_arg(parameters,char*);
signal_var = va_arg(parameters,char*);
char FName[128];
sprintf(FName,"%d.dat",this);
Sigma=1e20;
count=0;
char name[24]={ 29, 14, 22, 25, 13, 14, 31, 28, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40 };
char buf[128];
if (getOs()==WINDOWS) {
	sprintf(buf,"del /F %s",FName);
} else{ 
	sprintf(buf,"rm -f %s",FName);

}
system(buf);
#ifdef O_BINARY
	FOutput = open(FName, O_WRONLY | O_CREAT | O_TRUNC | O_BINARY);
#endif
#ifndef O_BINARY
	FOutput = open(FName, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
#endif
write(FOutput,name,24);
int cols=1;
write(FOutput,&cols,sizeof(int));
cols=2;
write(FOutput,&cols,sizeof(int));
write(FOutput,&cols,sizeof(int));
short s=0;
write(FOutput,&s,sizeof(short));
write(FOutput,&s,sizeof(short));
close(FOutput);
FOutput=PDFileOpen(FName,'a');
}
double toscilab_offline::ta(double t) {
return Sigma;
}
void toscilab_offline::dint(double t) {
Sigma=4e10;
}
void toscilab_offline::dext(Event x, double t) {
double Aux=(*(double*)(x.value));
count++;
PDFileWrite(FOutput,(char*)&t,sizeof(double));
PDFileWrite(FOutput,(char*)&Aux,sizeof(double));
//printLog("Transicion %d\n",count);
}
Event toscilab_offline::lambda(double t) {

}
void toscilab_offline::exit() {
char FName[128];
sprintf(FName,"%d.dat",this);
char buf[1024];
int result;
long pos=32;
close(FOutput);
#ifdef O_BINARY
	FOutput = open(FName, O_WRONLY | O_CREAT | O_TRUNC | O_BINARY);
#endif
#ifndef O_BINARY
	FOutput = open(FName, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
#endif
result=lseek(FOutput,pos,SEEK_SET);
result=write(FOutput,&count,sizeof(int));
close(FOutput);

#ifdef __WINDOWS__
sprintf(buf,"load %s/%d.dat",getcwd(NULL,0),this);
#else
sprintf(buf,"load %s/../output/%d.dat",getenv("PWD"),this);
#endif
executeVoidScilabJob(buf,true);
sprintf(buf,"tempdevs1= [ 102 105 108 101 40 39 99 108 111 115 101 39 44 102 105 108 101 40 41 41 32]; execstr(char(tempdevs1));clear tempdevs1;");
executeVoidScilabJob(buf,true);
if (getOs()==WINDOWS) {
	sprintf(buf,"del /F %d.dat;",this);
} else {
	sprintf(buf,"rm -f %d.dat;",this);
}
system(buf);
sprintf(buf,"%s=tempdevs(1,:);%s=tempdevs(2,:);clear tempdevs;",time_var,signal_var);
executeVoidScilabJob(buf,true);
}
