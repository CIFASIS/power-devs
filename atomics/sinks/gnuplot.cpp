#include "gnuplot.h"
void gnuplot::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
char *format[5],*gformat,buff[1024],ninput[3]="  ",Script[128];
n=(int) va_arg(parameters, double);
gformat = (char*) va_arg(parameters, char*);
format[0] = (char*) va_arg(parameters, char*);
format[1] = (char*) va_arg(parameters, char*);
format[2] = (char*) va_arg(parameters, char*);
format[3] = (char*) va_arg(parameters, char*);
format[4] = (char*) va_arg(parameters, char*);
sprintf(FName,"plots/%ld.csv",(long int)this);
sprintf(Script,"plots/%ld.plt",(long int)this);
long int fd=PDFileOpen(FName,'w');
PDFileWrite(fd,"0",1);
for (int i=0;i<n;i++)
{
  if (i==0)
     PDFileWrite(fd,",",1);
  PDFileWrite(fd,"0",1);
  if (i+1!=n)
     PDFileWrite(fd,",",1);
}
PDFileWrite(fd,"\n",1);
PDFileClose(fd);
hasOutput=0;
char buff2[1096];
strcpy(buff2,gformat);
char *p=strstr(buff2,"%tf");
if (p!=NULL)
{
	p[1]='g';
	p[2]=' ';
}
sprintf(buff,buff2,getFinalTime());
for (unsigned int i=0;i<strlen(buff);i++) 
   if (buff[i]=='@') buff[i]=';'; // Escaping characters
strcat(buff,"\nset datafile separator \",\"\nplot ");
for (int i=0;i<n;i++) {
	strcat(buff," \"");
	strcat(buff,FName);
	strcat(buff,"\" using 1:");
	ninput[0]='2'+i;
	strcat(buff, ninput);
	strcat(buff, format[i]);
	if (i<n-1) strcat( buff,", ");
	}
strcat( buff,"\n");
strcpy(printString,buff);
//strcat(buff,"pause 0.67\nload \"");
//strcat(buff,Script);
//strcat(buff,"\"\n");
fd=PDFileOpen(Script,'w');
PDFileWrite(fd,buff,strlen(buff));
PDFileClose(fd);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//      %Type% is the parameter type
sigma=1e20;
if (getOs()!=WINDOWS) {
//	spawnProcess("/usr/bin/gnuplot",Script);
   sprintf(buff,"find ./plots/%s.csv -ctime 1 -exec rm -rf {} \\;","*");
   //printLog(buff);
   int res=system(buff);//delete old .csv files
   sprintf(buff,"find ./plots/%s.plt -ctime 1 -exec rm -rf {} \\;","*");
   res=system(buff);//delete old .plt files
   //if (res<=0)
   // printLog("Failed system in gnuplot\n");
	}
/*else {
	char buff[1024];
	strcpy(buff,Script);
	strcat(buff," -");
	spawnProcess("../bin/gnuplot/bin/wgnuplot.exe",buff);
}*/
foutput=PDFileOpen(FName,'a');
}
double gnuplot::ta(double t) {
//This function returns a double.
return sigma;
}
void gnuplot::dint(double t) {
sigma=1e20;
}
void gnuplot::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
double *Aux;
char temp[64];
char buf[1024];
Aux=(double*)(x.value);
//printLog("Recibi un evento %g\n",Aux[0]);
sprintf(temp, "%g, ", t);
strcpy(buf,temp);
for (int i=0;i<n;i++) {
  if (i==(x.port)) {
   sprintf(temp,"%g " , ( Aux[0] ));
   strcat(buf, temp);
  }
  if (i<n-1) strcat(buf, ",");
}
strcat(buf, "\n");
//fwrite(buf,strlen(buf),1,foutput);
PDFileWrite(foutput,buf,strlen(buf));
#ifdef __WINDOWS__
 fclose((FILE*)foutput);
 foutput=(int)fopen(FName,"a"); 
#endif
}
Event gnuplot::lambda(double t) {
  return Event();
}
void gnuplot::exit() {
	PDFileClose(foutput);
	char Script[128];
	char buff[128];
	sprintf(Script,"plots/%ld.plt",(long int)this);
	long int fd=PDFileOpen(Script,'w');
//if (getOs()!=WINDOWS) 
        strcpy(buff,"\nset terminal wxt\n");
	PDFileWrite(fd,buff,strlen(buff));
//else
	PDFileWrite(fd,printString,strlen(printString));	
	strcpy(buff,"\npause 3600\n");

	PDFileWrite(fd,buff,strlen(buff));
	PDFileClose(fd);
if (getOs()!=WINDOWS) {
	spawnProcess("/usr/bin/gnuplot",Script);
	}
else {
	char buff[1024];
	strcpy(buff,Script);
	strcat(buff," -");
	spawnProcess("../bin/gnuplot/bin/wgnuplot.exe",buff);
}
}
