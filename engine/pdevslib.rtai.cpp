#include <sys/poll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/io.h>
#include <signal.h>

#include <rtai_sched.h>
#include <rtai_usi.h>
#include <rtai_lxrt.h>
#include <rtai_mbx.h>
#include <rtai_msg.h>
#include <rtai_fifos.h>

#include "types.h"
#include "event.h"
#include "pdevslib.h"
#include "root_simulator.h"

#define MIN_WAIT_NS (1e-6)
#define MAX_BUSY_SLEEP (20e-6)
#define NORMAL_WAIT_DIFF (MAX_BUSY_SLEEP/2)
#define FIFOSPACE (0x100000)
#define NIRQS (16)
#define NFILE 64
#define NBUFF 2097152
/*! \brief This structure is used to notify the simulation engine that an IRQ has happend */
struct IRQMessage {
    //! Which IRQ ocurred
    int irq;
    Time time;
};
/*! \brief Linked list to notify whan an IRQ has happend */
struct Node {
    void *atomic;
    struct Node *next;
};

struct Node *isr[NIRQS];

void initIRQMBX();
void clearISR();
void deleteIRQMBX();
double max_latency=0, initial_t;
double avg_latency=0, waits=1;
float overruns = 0;
float events = 0;
static volatile int end = 1;
static volatile int lastirq = -1;

typedef struct 
{
  int mode;
  char name[128];
  char *buff;
  long int p;
  long int size;
} PDFile;

PDFile fd[NFILE];

double getRealSimulationTime() {
 	return getTime()-realTi;
}
void alertOfIRQ(IRQMessage msg) {
    struct Node *n =  isr[msg.irq];
    class Simulator *s;
    msg.time = getTime()-realTi;
    for (;n != NULL; n = n->next) {
        s = (Simulator*)n->atomic;
		//rtai_print_to_screen("[t=%g] Notifying atomic %s of an IRQ\n",msg.time,s->name);
        s->externalinput(msg.time,msg.irq);
    };
}



OS getOs() 
{
	return RTAI;
}
double getTime()
{
	return count2nano(rt_get_time())*1e-9;
}


static MBX *mbxirq;
MBX *getIRQMBX() { return mbxirq; }
int waitFor(Time delay, RealTimeMode mode) 
{ 
	IRQMessage msg;
    MBX *mbx = getIRQMBX();
	int ret;
    if (delay<0) {
		overruns++;
		overRun=true;
		return 0;	
	}
	if (delay>MIN_WAIT_NS ) { // Do wait if the delay is less than MINWAIT_NS
		events++;
		Time beforeWait=getTime();
		if ( mode == REALTIME ) {
			RTIME sleep=nano2count(delay*1e9);
			//rt_sleep(sleep);
			ret = rt_mbx_receive_timed( mbx, &msg, sizeof(msg), sleep);
			if (ret == 0  && msg.irq > 0) { // An IRQ has happend. Notify this
/*
            	rtai_print_to_screen("Recibi un mail con la irq %d\n en t=%.12g\n",
				msg.irq,msg.time);
*/
                alertOfIRQ(msg);
                return -1;
            }

		} else {
			if ( delay < MAX_BUSY_SLEEP) { // The latency is lower than MAXBUSYSLEEP, then do a busy_sleep
				rt_busy_sleep(delay*1e9);
			} else {
				RTIME sleep=nano2count(delay*1e9 - NORMAL_WAIT_DIFF*1e9);
				ret = rt_mbx_receive_timed( mbx, &msg, sizeof(msg), sleep);
				if (ret == 0  && msg.irq > 0) { // An IRQ has happend. Notify this
/*
            	rtai_print_to_screen("Recibi un mail con la irq %d\n en t=%.12g\n",
				msg.irq,msg.time);
*/
               				 alertOfIRQ(msg);
			                return -1;
		        }

				Time newdelay = delay - (getTime()-beforeWait);
				//printLog("Waited %g in normal wait and going to wait %d in busy sleep\n",delay*1e9 - NORMAL_WAIT_DIFF,int(newdelay*1e9));
				if (newdelay > MIN_WAIT_NS) { // Compensate
					int n=newdelay*1e9;
					Time i=getTime();
					rt_busy_sleep(n);
					Time f=getTime();
					//printLog(" Lat busy(%d) sleep %d\n",n,int((f-i)*1e9));
				}
			}
		}
		Time afterWait=getTime();
		Time latency=(afterWait-beforeWait)-delay; 	
		if ((double)latency>0) {
			avg_latency+=latency;
			waits++;
		}
		max_latency   = (latency > max_latency ? latency : max_latency);
	}
	return 0;


}

#define STACKSIZE 0x1000
RT_TASK *task;
int fd_stdout;
void enterRealTime (){
	int period;
	initIRQMBX();
 	if (! (task = 
	rt_task_init(nam2num("PDEVS"), 0,STACKSIZE,0x0))) {
		printf("Error: Can't init the scheduler\n");
		exit(-1);
	}
	
	rt_set_oneshot_mode();
	period = start_rt_timer(nano2count(100000));
	mlockall(MCL_CURRENT | MCL_FUTURE);
	printLog("Going realtime...(PID = %d)\n",getpid());
	rt_make_hard_real_time();
	rt_task_make_periodic(task,rt_get_time()+10*period,period);
}
void leaveRealTime() {
	rt_make_soft_real_time();
	clearISR();
	printLog("Leaving realtime...\n");
	deleteIRQMBX();
	rt_task_delete(task);
	printLog("***********************************************\n");
	printLog("Max latency=%g\n",max_latency);
	printLog("Avg latency=%g\n", avg_latency/waits);
	printLog("Overruns %g over %g\n", overruns,events);
	printLog("***********************************************\n");
}


void spawnProcess(const char *path, char *arg) {
	char *argv[10],buff[1024];
	if (fork()==0) { // Child process
			int i=0;
			argv[i]=(char*)malloc(strlen(path));
			strcpy(argv[i++],path);
			while (sscanf(arg,"%s",buff)>0) {
				if (strlen(buff)==0) break;
				argv[i]=(char*)malloc(strlen(buff));
				strcpy(argv[i],buff);
				arg+=strlen(argv[i++]);
			}
			argv[i]=NULL;
			execv(path,argv);
			printLog("ERROR: %s not found\n",path);
	}
}

void writeToPort(short v,int port) {
	outb_p(v,port);
}

short readFromPort(int port) {
	return inb_p(port);
}

long int PDFileOpen(char* name,char mode) 
{
  int i;
  for (i=0;i<NFILE;i++)
    if (fd[i].buff==NULL) 
      break;
  if (i==NFILE)
    return -1;
  strcpy(fd[i].name,name);
  fd[i].p = 0;
  if (mode=='w')
  {
    fd[i].buff = new char[NBUFF];
    fd[i].mode = O_WRONLY;
  } else if (mode=='r') {
    struct stat s;
    int f=open(name,O_RDONLY,0);
    if (f<0)
      return -1;
    fstat(f,&s);
    fd[i].size = s.st_size;
    fd[i].buff = new char[fd[i].size];
    fd[i].mode = O_RDONLY;
    if (read(f,fd[i].buff,fd[i].size)!=fd[i].size)
      return -1;
    close(f);
  } else if (mode=='a') {
    struct stat s;
    int f=open(name,O_RDONLY,0);
    if (f<0)
      return -1;
    fstat(f,&s);
    fd[i].p = s.st_size;
    fd[i].size = s.st_size;
    fd[i].buff = new char[NBUFF];
    fd[i].mode = O_WRONLY;
    if (read(f,fd[i].buff,fd[i].size)!=fd[i].size)
      return -1;
    close(f);
  }
	return i;
};

long int PDFileWrite(long int file ,char* buf,int size) {
  if (fd[file].p+size>NBUFF && fd[file].size!=-1) 
  { 
    fd[file].size=-1;
    if (strcmp(fd[file].name,"pdevs.log")!=0)
      printLog("Buffer error writing %s. Try expanding NBUFF in %s\n",fd[file].name,__FILE__);
  }
  if (fd[file].p+size>NBUFF)
    return -1;
  memcpy(fd[file].buff+fd[file].p,buf,size);
  fd[file].p+=size;
  return size;
};

long int PDFileRead(long int file ,char* buf ,int size){
  if (fd[file].p+size > fd[file].size){
    size=fd[file].size-fd[file].p;
  }
  if (fd[file].p==fd[file].size) //EOF
    return 0;
  memcpy(buf,fd[file].buff+fd[file].p,size);
  fd[file].p+=size;
  return size;
}

void PDFileClose(long int file){
  if (fd[file].buff==NULL)
    return;
  if (fd[file].mode==O_WRONLY)
  {
    int f=open(fd[file].name,O_WRONLY | O_CREAT | O_TRUNC,0666);
    write(f,fd[file].buff,fd[file].p);
    close(f);
  }
  delete []fd[file].buff;
  fd[file].buff = NULL;
};

int printlog_fd=0;
void printLog(const char *fmt,...) {
  static int init=0;
  char buff[128];
  if (!init) {
    init=1;
    printlog_fd= PDFileOpen("pdevs.log",'w');
	if (printlog_fd>=0) {
		
		struct tm *ptr;
		time_t tm;
		tm = time(NULL);
		ptr = localtime(&tm);
		sprintf(buff,"Log of execution: %s\n",asctime(ptr));
		PDFileWrite(printlog_fd,buff,strlen(buff));
	}
	else rtai_print_to_screen("Error writing in log file\n");
  }
	va_list va;
	va_start(va,fmt);
	if (printlog_fd>=0) {
		vsprintf(buff,fmt,va);
		PDFileWrite(printlog_fd,buff,strlen(buff));
	} else {
		rtai_print_to_screen("Error writing in log file\n");
	}
	va_end(va);
}

int isrInitialized = 0;

void initIRQMBX() {
    mbxirq = (MBX*) rt_mbx_init(nam2num("IRQMBX"),20*sizeof(IRQMessage));
    printLog("Initializing MBX (at %x)\n",mbxirq);
}

static void endme (int dummy) {
    rtai_print_to_screen("Ending IRQS\n");
    unsigned int i;
    for (   i=0;i<NIRQS;i++) {
        if (isr[i]!=NULL) {
                rt_release_irq_task(i);
                rt_unmask_irq((unsigned)i);
        }
    }
    _exit(0);
}

void initISR() {
    int i;
    for (i=0;i<NIRQS;i++){
         isr[i]=NULL;
    }
}


void clearISR() {
    int i;
    for ( i=0;i<NIRQS;i++) {
        if (isr[i]!=NULL) {
            end = 0;
            rt_irq_signal(i);
            rt_sleep(2000000000); // Give time to IRQ handlers to exit
            rtai_print_to_screen("Ending IRQS from Clear\n");
        }
    }
}
    
void deleteIRQMBX() {
    rt_mbx_delete(mbxirq);
}

static void *irq_handler(void *args) {
        unsigned int irq = (int)args;
        char name[] ="HNDLR  ";
        name[5] = '0' + irq;
        MBX *mbx = NULL;
        struct IRQMessage msg;
        signal(SIGHUP,endme);
        signal(SIGINT,endme);
        signal(SIGKILL,endme);
        signal(SIGTERM,endme);
        char buf[] = "IRQ\n";
        RT_TASK *handler_task=rt_task_init_schmod(nam2num(name),0,0,0,SCHED_FIFO,0xF);
        rt_allow_nonroot_hrt();
        mlockall(MCL_CURRENT | MCL_FUTURE);
        rt_make_hard_real_time();
        while (mbx == NULL) {
            mbx = (MBX*)rt_get_adr(nam2num("IRQMBX"));
            rt_sleep(1000000);
        }
        rtai_print_to_screen("Waiting for IRQ %d\n",irq);
        rt_request_irq_task(irq, handler_task, RT_IRQ_TASK, 1);
        rt_startup_irq(irq);
        rt_enable_irq(irq);
        while (end) {
            int ret = rt_irq_wait(irq);
			if (ret == RT_IRQ_TASK_ERR) break;
            if (!end) break;
            if (!ret) {
                rt_ack_irq(irq);
                msg.irq = irq;
                //rtai_print_to_screen("Detected IRQ %d\n",irq);
                if (mbx!=NULL ) {
                    //rtai_print_to_screen("Sending mail to MBX at %x\n",mbx);
                    rt_mbx_send(mbx,&msg,sizeof(msg));
                }
				rt_pend_linux_irq(irq);

            }
        }
        rtai_print_to_screen("Releasing handler IRQ\n");
		rt_release_irq_task(irq);
        rt_make_soft_real_time();
        rt_task_delete(handler_task);
        return 0;
}

void RequestIRQ(unsigned int irq, void *atomic) {
    if (!isrInitialized) { initISR(); isrInitialized=1;}
    if (irq>NIRQS) return;
    struct Node *newnode = (Node*)malloc(sizeof(Node));
    newnode->atomic = atomic;
    newnode->next=isr[irq];
    void *handler = (void*)irq_handler;
    if (isr[irq]==NULL) { // Instalo la ISR
        rt_thread_create(handler,(void*)irq,0x10000);
    }
    isr[irq]=newnode;
    return;
}

void preinitLib()
{
  for (int i=0;i<NFILE;i++)
    fd[i].buff=NULL; // File closed
}

void cleanLib()
{
  if(printlog_fd>=0)	
    PDFileClose(printlog_fd);


}
void initLib()
{
  struct timeval tv;
  gettimeofday(&tv,NULL);
  realTiSimulation = tv.tv_sec + tv.tv_usec*1.0e-6;
}

#include "pdevslib.common.cpp"
