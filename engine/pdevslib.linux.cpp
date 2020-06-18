#include <sys/poll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include "types.h"
#include <sys/time.h>
#include <math.h>
#include <sys/io.h>
#include <root_simulator.h>

OS getOs() 
{
	return LINUX;
}

double getTime()
{
	return 1.0*clock()/CLOCKS_PER_SEC;
}


double getRealSimulationTime()
{
  struct timeval tv;
  gettimeofday(&tv,NULL);
  return (tv.tv_sec + tv.tv_usec*1.0e-6)  - realTiSimulation;
}

int waitFor(Time t, RealTimeMode m) 
{
	double ti=getRealSimulationTime();
	if (t<=0) 
    return 0;
	while ((getRealSimulationTime()-ti)<t) ;
  return 0;
}

void initLib()
{
  struct timeval tv;
  gettimeofday(&tv,NULL);
  realTiSimulation = tv.tv_sec + tv.tv_usec*1.0e-6;
}

void cleanLib()
{

}

void parseCommandLine(char *program, char* cmdLineTxt, char*** argv, int* argc){
    int count = 1;

    char *cmdLineCopy = strdupa(cmdLineTxt);
    char* match = strtok(cmdLineCopy, " ");
 // First, count the number of arguments
    while(match != NULL){
        count++;
        match = strtok(NULL, " ");
    }

    *argv = (char**) malloc(sizeof(char*) * (count+1));
    (*argv)[count] = 0;
    **argv = strdup(program); // The program name would normally go in here

    if (count > 1){
        int i=1;
        cmdLineCopy = strdupa(cmdLineTxt);
        match = strtok(cmdLineCopy, " ");
        do{
            (*argv)[i++] = strdup(match);
            match = strtok(NULL, " ");
        } while(match != NULL);
     }

    *argc = count;
}
void spawnProcess(const char *path, char *arg) {
	char **argv;int argc;
  parseCommandLine((char*)path,arg,&argv,&argc);
	if (fork()==0) { // Child process
      /*
      printLog("Running program %s with args:", path);
      for (int i=0;i<argc;i++) 
        printLog("%d=%s, ",i,argv[i]); 
      printLog("\n");
      */
			execv(path,argv);
      exitStatus = -1;
			printLog("ERROR: %s not found\n",path);
      abort();
	}
}

void writeToPort(short v,int port) {
  if (ioperm(port,1,1)) {
    printLog("Couldnt get the permissions for port %x\n", port);
    exit(-1);
  }
	outb_p(v,port);
}

short readFromPort(int port) {
  if (ioperm(port,1,1)) {
    printLog("Couldnt get the permissions for port %x\n", port);
    exit(-1);
  }
	return inb_p(port);
}

long int PDFileOpen(char* name,char mode) {
	char strMode[2];
	strMode[0]=mode;
	strMode[1]='\0';
	return (long int)fopen(name,strMode);
};
long int PDFileWrite(long int file ,char* buf,int size) {
	long int r=fwrite(buf,size,1,(FILE*)file);	
  fflush((FILE*)file);
  return r;
};
long int PDFileRead(long int file ,char* buf ,int size){
  int r;
	if ((r=fread(buf,size,1,(FILE*)file))<size)
    printLog("Incomplete read from file\n");
  return r;
}
void PDFileClose(long int file){
	fclose((FILE*)file);
};

void printLog(const char *fmt,...) {
  static int init=0;
  if (!init) {
    init=1;
    fclose(fopen("pdevs.log","w"));
  }
	va_list va;
	va_start(va,fmt);
	FILE *fd=fopen("pdevs.log","a");
	vfprintf(fd,fmt,va);
	fclose(fd);
	va_end(va);
}

void RequestIRQ(unsigned irq, void  *a){

}

#include "pdevslib.common.cpp"
