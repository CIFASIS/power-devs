#include "fromwav.h"
void fromwav::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//      %Type% is the parameter type
int i;
for (i=0;i<10;i++) y[i]=0;
fname=va_arg(parameters,char*);
fd=open(fname,O_RDONLY | O_BINARY);
read(fd,&h,sizeof(header));
//printLog("Reading a %s wave file sampled at %d with %d bits\n",h.channels==1?"mono":"stereo",h.samplerate,(int)h.bitdepth);
sigma=1.0/h.samplerate;
buf = (char*)malloc(h.datasize);
read(fd,buf,h.datasize);
val16=(short*)buf;
count=0;
}
double fromwav::ta(double t) {
//This function returns a double.
return sigma;
}
void fromwav::dint(double t) {
val16++;
count++;
if (count>=h.datasize/((int)h.bitdepth/8)) sigma=10e10;

/*
if ((int)val16-(int)buf>=len) {
len=fread(buf,1,128,finput);
val16=(short*)buf;
//printLog("Size of buf   %d\n",len);
if (len==0) sigma=10e10;
}
//printLog("Lei %d \n",*val16);
*/
}
void fromwav::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
}
Event fromwav::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)

y[0]=(1.0*(*val16))/32767.0;
//printLog("Lei %g %d\n",y[0],*val16);
return Event(y,0);
}
void fromwav::exit() {
//Code executed at the end of the simulation.
free(buf);
close(fd);
}
