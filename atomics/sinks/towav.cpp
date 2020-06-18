#include "towav.h"
void towav::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//      %Type% is the parameter type
fname=va_arg(parameters,char*);
count=0;
sample=va_arg(parameters,char*);
fd=fopen(fname,"wb");
val16=0;
strcpy(h.riff,"RIFF");
strcpy(h.wave,"WAVE");
strcpy(h.fmt,"fmt ");
h.chunksize=16;
h.comp=1;
h.channels=1;
h.bitdepth=16;
h.samplerate=(! strcmp(sample,"44100") ? 44100 : 22050);
h.byterate=h.samplerate*h.bitdepth/8;
h.blockalign=h.bitdepth/8;
strcpy(h.data,"data");
fwrite(&h,sizeof(h),1,fd);
ts=1.0/(! strcmp(sample,"44100") ? 44100 : 22050);
sigma=ts;
//fclose(fd);
}
double towav::ta(double t) {
//This function returns a double.
return sigma;
}
void towav::dint(double t) {
int result  = fwrite(&val16,1,2,fd);
int c=0;
count++;
//printLog("[t=%g] Escribo %d en la posición %d con resultado %d\n",t,val16,pos,result);
sigma=ts;
}
void towav::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
sigma=sigma-e;
double *ev=(double*)x.value;
val16=(short)(ev[0]*32767.0);
//printLog("Sampleo %d\n",val16);
}
Event towav::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)
}
void towav::exit() {
//Code executed at the end of the simulation.
//fd=fopen(fname,"a");
rewind(fd);
h.filesize=sizeof(short)*count+44-8;
h.datasize=count*sizeof(short);
//printLog("Tome %d s22amples\n",count);
//printLog("File size should be %d\n",sizeof(h)+count*sizeof(short));
fwrite(&h,sizeof(h),1,fd);
fclose(fd);
}
