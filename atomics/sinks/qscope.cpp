#include "qscope.h"
void qscope::init(double t,...) {
char* FName;
	va_list parameters;
	va_start(parameters, t);
	FName=va_arg(parameters, char*);
	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES); 
	saAttr.bInheritHandle = TRUE; 
	saAttr.lpSecurityDescriptor = NULL; 
	CreatePipe(&hChildStdinRd, &hChildStdinWr, &saAttr, 0);
	bFuncRetn = FALSE;
	char b[100];
	strcpy(b,"qscope ");
	printf("%s\n",FName);
	strcat(b,FName);
ZeroMemory( &piProcInfo, sizeof(PROCESS_INFORMATION) );
// Set up members of the STARTUPINFO structure. 
   ZeroMemory( &siStartInfo, sizeof(STARTUPINFO) );
   siStartInfo.cb = sizeof(STARTUPINFO); 
   siStartInfo.hStdInput = hChildStdinRd;
   siStartInfo.dwFlags |= STARTF_USESTDHANDLES;
bFuncRetn = CreateProcess(NULL, 
      b,       // command line 
      &saAttr,          // process security attributes 
      &saAttr,          // primary thread security attributes 
      TRUE,          // handles are inherited 
      0,             // creation flags 
      NULL,          // use parent's environment 
      NULL,          // use parent's current directory 
      &siStartInfo,  // STARTUPINFO pointer 
      &piProcInfo);
sigma=10e10;
}
double qscope::ta(double t) {
//This function returns a double.
return sigma;
}
void qscope::dint(double t) {
sigma=10e10;
}
void qscope::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
char buff[1000];
double *v=(double*)x.value;
sprintf(buff,"(%d,%.12g,%.12g)\n",x.port,t,*v);
WriteFile(hChildStdinWr,buff, strlen(buff), &dwWritten, NULL);
}
Event qscope::lambda(double t) {

}
void qscope::exit() {
WriteFile(hChildStdinWr,"q\n", 2, &dwWritten, NULL);
CloseHandle(hChildStdinWr);
}
