#ifdef __linux__
#define SOCKET_ERROR        -1
int SendSocket;
#else
#define H5_SIZEOF_SSIZE_T 1
SOCKET SendSocket;
#endif
struct sockaddr_in service, serviceUDP;
bool activeScilab;

#include <hdf5/serial/hdf5.h>
double executeVoidScilabJob(char *cmd,bool blocking) {
  char buff[1024];
  sprintf(buff,"\\%s",cmd); // If the command starts with a slash the command is not written to the out var
  executeScilabJob(buff,blocking);
  return 0.0;
}

double executeScilabJob(char *cmd,bool blocking) {
  double ans=0;
  char buff[1024];
  int result = -1;

  initScilab();
  strcpy(buff,"");
  if (blocking)
    strcpy(buff,"!");
  strcat(buff,cmd);
  strcat(buff,"\n");
  if (!activeScilab) {
    exitStatus = -1;
    printLog("There's not an instance of Scilab running. Returing zero\n");
    return 0.0;
  }
#ifdef __linux__
  if (write(SendSocket,buff,strlen(buff))<(signed)strlen(buff))
#else
  if ((send(SendSocket,buff,strlen(buff),0))<(signed)strlen(buff)) 
#endif
    printLog("Incomplete TCP message\n");
  if (blocking) {
#ifdef __linux__
	  result = read(SendSocket, buff , 1024);
#else
	  result = recv(SendSocket, buff , 1024,0);
#endif
  }
  return ans;
}

void initScilab() {
  static int init=0;
  if (init++ == 0) {
    service.sin_family = AF_INET;
    service.sin_addr.s_addr = inet_addr("127.0.0.1");
#ifdef __linux__
    int actual_port = 27020+(getuid() % 10000);
#else
    int actual_port = 27020;
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
#endif
	  SendSocket = socket(AF_INET, SOCK_STREAM,IPPROTO_TCP);
	  service.sin_family = AF_INET;
	  service.sin_addr.s_addr = inet_addr("127.0.0.1");
	  service.sin_port = htons(actual_port);
	  printLog("Openening connection with scilab to TPC port %d\n",actual_port);
	  //ioctlsocket(SendSocket,FIONBIO,&bio);
    	  if (connect(SendSocket,(struct sockaddr*)&service,sizeof(service)) == SOCKET_ERROR)
          {
           activeScilab=false;
           exitStatus = -1;
	   printLog("Scilab not found\n");
           return;
         }
         activeScilab=true;
  }
}
void cleanScilab() {
 static int clean=0;
 if (clean++ == 0) {
#ifdef __linux__
    close(SendSocket);
#else
    closesocket(SendSocket);
#endif
 }
}


void putScilabVar(char *varname, double var) {
  initScilab();
   char buf[1024];
   sprintf(buf,"%s=%g",varname,var);
   executeVoidScilabJob(buf,false);
}

double getScilabVar(char *varname) {
  // Try to solve it locally
  char *s;
  double d=strtod(varname,&s);
  if (varname+strlen(varname)==s)
    return d;
  initScilab();
  char buf[1024];
  double f;
  sprintf(buf,"anss=%s",varname);
  executeVoidScilabJob(buf,true);
  executeScilabJob((char*)"exists('anss')",false);
  getAns(&f,1,1);
  if (!activeScilab) {
     exitStatus = -1;
     printLog("There's not an instance of Scilab running. Returning atof(%s)\n",varname); 
     return atof(varname);
  }
  if (f==0) {
     exitStatus = -1;
     printLog("Variable %s does not exists! Returning zero\n",varname); 
     return 0.0;
  }
  executeScilabJob((char*)"anss",true);
  getAns(&f,1,1);
  executeVoidScilabJob((char*)"clear anss",true);
  return f;
}


void getAns(double *ans, int rows, int cols) {
  initScilab();
  if (!activeScilab) {
    exitStatus = -1;
    printLog("There's not an instance of Scilab running. Returing zero\n");
	  ans[0]=0.0;
  }
  char cmd[124] = "@";
  int iResult;
  executeScilabJob(cmd,false);
  ans[0]=0;
  if (!activeScilab) 
    return;
#ifdef __linux__
  iResult = read(SendSocket, (char*)ans, sizeof(double)*rows*cols);
#else
  iResult = recv(SendSocket, (char*)ans, sizeof(double)*rows*cols,0);
#endif
  if (iResult<0) {
    exitStatus = -1;
    printLog("There's not an instance of Scilab running. Returing zero\n");
    activeScilab=false;
  }
}


void getScilabMatrix(char* varname, int *rows, int *cols, double **data) {
  char buf[1024];
  initScilab();
  hid_t       file_id, dataset_id;  
  sprintf(buf,"tempvar=%s",varname);
  executeVoidScilabJob(buf,true);
  sprintf(buf,"save('%s/../output/temp.dat','tempvar')",getenv("PWD"));
  executeVoidScilabJob(buf,true);
  file_id = H5Fopen("temp.dat",H5F_ACC_RDONLY , H5P_DEFAULT);
  dataset_id = H5Dopen2(file_id, "/tempvar", H5P_DEFAULT);
  int dimension =  H5Sget_simple_extent_ndims(H5Dget_space(dataset_id));
  if (dimension!=2) {
    printLog("Incomplete read in getScilabMatrix\n");
    return;
  }
  hsize_t size[2];
  H5Sget_simple_extent_dims(H5Dget_space(dataset_id), size, NULL);
  rows[0]=(int)size[1];
  cols[0]=(int)size[0];
  double *tmp_data = new double[rows[0]*cols[0]];
  H5Dread(dataset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, tmp_data);
  for (int i=0;i<rows[0];i++) {
    for (int j=0;j<cols[0];j++) {
        data[i][j]=tmp_data[i+j*cols[0]];
    }
  }
  delete tmp_data;
  H5Fclose(file_id);
  H5Dclose(dataset_id);
  unlink("temp.dat");
}

void getScilabVector(char* varname, int *length, double *data) {
  char *s;
  double d=strtod(varname,&s);
  hid_t       file_id, dataset_id;  
  hsize_t size[2];
  if (varname+strlen(varname)==s)
  {
    *length=1;
    data[0]=d;
    return;
  }
  char buf[1024];
  sprintf(buf,"tempvar=%s",varname);
  executeVoidScilabJob(buf,true);
  sprintf(buf,"save('temp.dat','tempvar')");
  executeVoidScilabJob(buf,true);
  sprintf(buf,"temp.dat");
  file_id = H5Fopen(buf,H5F_ACC_RDONLY , H5P_DEFAULT);
  dataset_id = H5Dopen2(file_id, "/tempvar", H5P_DEFAULT);
  int dimension =  H5Sget_simple_extent_ndims(H5Dget_space(dataset_id));
  H5Sget_simple_extent_dims(H5Dget_space(dataset_id), size, NULL);
  int rows=(int)size[1];
  int cols=(int)size[0];
  if (dimension!=2 || rows!=1) {
    printLog("Incomplete read in getScilabVector\n");
    return;
  }
  *length=cols;
  H5Dread(dataset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, data);
  H5Dclose(dataset_id);
  H5Fclose(file_id);
  unlink("temp.dat");

  /*sprintf(buf,"deletefile('C:\\powerdevs\\output\\temp.dat')");
  printLog(buf);
  executeVoidScilabJob(buf,true);
  */
}

extern double tf;
double getFinalTime()
{
	return tf;
}
