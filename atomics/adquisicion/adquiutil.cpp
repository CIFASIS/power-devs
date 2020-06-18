#include <string.h>
#include <stdlib.h>

int LeerVector(char * vectorbuf,double ** elementos){
	int i=1,len=strlen(vectorbuf),numel=1,e=0;
	char * vector=new char[len];
	memcpy(vector,vectorbuf,len);
	/*Parsing inicial, checkea formato y cuenta numero de elementos*/
	if(len==0){/*delete [] vector;*/return -1;}
	if(vector[0]!='['){/*delete [] vector;*/return 0;}
	if(vector[--len]!=']'){/*delete [] vector;*/return -1;}
	vector[len]=0; //termina el string en el ]
	while(i<len){
		if(vector[i]==',')numel++;
		else if((vector[i]<'0' || vector[i]>'9')&&vector[i]!='.' && vector[i]!='e' && vector[i]!='+' && vector[i]!='-' && vector[i]!=' ')return -1;
		i++;
	}
	/*Parsing final, asigna vector elementos*/

	(*elementos)=new double[numel];
	vector++; //se posiciona en primer elemento
	len--;
	while(e<numel){
		i=0;
		while(i<=len)
			if(vector[i++]==',')
				break;
		vector[i-1]=0;	//coloca terminador
		(*elementos)[e++]=atof(vector);
		vector+=i;		//reposiciona vector
		len-=i;			//ajusta tamaño final
	}
	//delete [] vector;
	return numel;
}