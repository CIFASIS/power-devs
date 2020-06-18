#include "adquider.h"


void adquider::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//	%Type% is the parameter type
	int i,a;
	double *v;
	double d=va_arg(parameters,double);		//Maquina utilizada
	char buffer[256];
	if(d<0.5) { 
		tipomaq=levitador;	//0 MagLev
	}
	else{
		tipomaq=pendubot;	//1 PenduBot
	}

	d=va_arg(parameters,double);		//modo ADC
	
	if(d<0.5) { 
		ModoADC=0;	//10 bits, Tresp=1us
	}
	else{
		ModoADC=1;		//12 bits, Tresp=5us
	}

	d=va_arg(parameters,double);		//Interrupciones
	
	if(d<0.5) { 
		Interrupts=0;	//Sin interrupciones
	}
	else{
		Interrupts=1;	//Con interrupciones
	}


	char *e=va_arg(parameters,char*);
	i=LeerVector(e,&v);
	if(i==0){
		sprintf(buffer,"%s(1)",e);
	    Cuanto1=getScilabVar(buffer);
	    sprintf(buffer,"%s(2)",e);
	    Cuanto2=getScilabVar(buffer);	    
	    sprintf(buffer,"%s(3)",e);
	    Cuanto3=getScilabVar(buffer);
	}
	else if(i!=3){
		ERROR=1;
		printLog("\nAdquisidor: Sintaxis incorecta, o incorrecta cantidad de elementos, parametro 2\n");
		return;
	}else{
		Cuanto1=v[0];
	    Cuanto2=v[1];
	    Cuanto3=v[2];
		//delete [] v;
	}
	
	e =va_arg(parameters,char*);		//ganancia PWM
	gainPWM=getScilabVar(e);

	e=va_arg(parameters,char*);
	i=LeerVector(e,&v);
	if(i==0){
		sprintf(buffer,"%s(1)",e);
	    gainADC1=getScilabVar(buffer);
	    sprintf(buffer,"%s(2)",e);
	    gainADC2=getScilabVar(buffer);	    
	    sprintf(buffer,"%s(3)",e);
	    gainADC3=getScilabVar(buffer);
	}
	else if(i!=3){
		ERROR=1;
		printLog("\nAdquisidor: Sintaxis incorecta, o incorrecta cantidad de elementos, parametro 4\n");
		return;
	}else{
		gainADC1=v[0];
	    gainADC2=v[1];
	    gainADC3=v[2];
		//delete [] v;
	}	    
	
	
	e =va_arg(parameters,char*);		//Tiempo máximo de espera para derivada 0
	TMAX=getScilabVar(e);

	e=va_arg(parameters,char*);
	i=LeerVector(e,&v);
	if(i==0){
	    sprintf(buffer,"%s(1)",e);
	    d2sup=getScilabVar(buffer);
	    sprintf(buffer,"%s(2)",e);
	    d2inf=getScilabVar(buffer);	 
	}
	else if(i!=2){
		printLog("\nAdquisidor: Sintaxis incorecta, o incorrecta cantidad de elementos, parametro 6\n");
		ERROR=1;
		return;
	}else{
		d2sup=v[0];
	    d2inf=v[1];
		//delete [] v;
	}

	e=va_arg(parameters,char*);
	i=LeerVector(e,&v);
	if(i==0){
	    sprintf(buffer,"%s(1)",e);
	    d3sup=getScilabVar(buffer);
	    sprintf(buffer,"%s(2)",e);
	    d3inf=getScilabVar(buffer);	     
	}
	else if(i!=2){
		printLog("\nAdquisidor: Sintaxis incorecta, o incorrecta cantidad de elementos, parametro 7\n");
		ERROR=1;
		return;
	}else{
		d3sup=v[0];
	    d3inf=v[1];
		//delete [] v;
	}

	e=va_arg(parameters,char*);
	refpos=getScilabVar(e);
	printLog("\nCuantos:[%d,%d,%d], gainADC's:[%d,%d,%d], SatDer2:[%d,%d], SatDer3:[%d,%d]\n",(int)Cuanto1,(int)Cuanto2,(int)Cuanto3,(int)gainADC1,(int)gainADC2,(int)gainADC3,(int)d2sup,(int)d2inf,(int)d3sup,(int)d3inf);

	/*Inicializa vectores*/
	
	int ind=0;
	while(ind<10){dADC1[ind]=0; dADC2[ind]=0; dADC3[ind]=0; dDADC2[ind]=0; dDADC3[ind++]=0;}
	



    if(tipomaq == levitador) { //si es el Levitador Magnetico
	//Inicializacion del Puerto EPP
	printLog("******** Inicializando Placa Levitador Magnetico ******");
	ERROR=0;			// Limpia el error

	iopl(3); 			// consige permisos de IO en memoria
	writeToPort(0x80,eppbase+0x402);
	if(Interrupts==1){
	      writeToPort(0xd4,eppcontrol); 	// configura Control Register para tener ints y setear senales de control
	      RequestIRQ(7,this);		// pide interrupcion al RTAI 
	}else{
	      writeToPort(0x04,eppcontrol); 	// configura Control Register para no tener ints y setear senales de control
	}
	//Inicalizacion del Equipo
	EscribirEPP(ComNOP);	//limpia por las dudas
	if(ModoADC==1)EscribirEPP(SetADC12);
	else EscribirEPP(SetADC10);
	
	EscribirEPP(SetCuanto1);	//solicita cambio de cuanto
	EscribirEPP(Cuanto1-1);		//envia el cambio de cuanto
	EscribirEPP(SetCuanto2);	//solicita cambio de cuanto
	EscribirEPP(Cuanto2-1);		//envia el cambio de cuanto
	
	EscribirEPP(ActivarINTS);	//activa interrupciones
	
	EscribirEPP(ResetPIC);	//Genera un reset para aplicar configuracion y empezar de 0
	
	sleep(1);

	EscribirEPP(LeerConf);
	
	uchar r=LeerEPP();
	
	if((r&0x01)!=ModoADC){	//Error!!! debe logearlo
		printLog("\nError en la inicializacion del modo del ADC\n******** No se pudo inicializar ***********\n");
		ERROR=1;
		return;
	}
	
	EscribirEPP(LeerCuanto1);
	r=LeerEPP();
	
	if(r!=(Cuanto1-1)){	//Error!!! debe logearlo
		printLog("\nError en la inicializacion del cuanto 1. Cuanto recibido: %d\n******** No se pudo inicializar ***********\n",(int)r+1);
		ERROR=1;
		return;
	}
	
	EscribirEPP(LeerCuanto2);
	r=LeerEPP();
	
	if(r!=(Cuanto2-1)){	//Error!!! debe logearlo
		printLog("\nError en la inicializacion del cuanto 2. Cuanto recibido: %d\n******** No se pudo inicializar ***********\n",(int)r+1);
		ERROR=1;
		return;
	}

	desinc=1;		//indica que se esta inicializando y el adc esta desincronizado
	ADC1=0;			//por las dudas...
	ADC2=0;
	inti = 0;
	EscribirEPP(LeerAltaADC1);	//comando para leer el ADC1 entero
	ADC1=LeerEPP()<<8;	//Lee parte alta
	ADC1+=LeerEPP();		//Lee parte baja
	EscribirEPP(LeerAltaADC2);	//comando para leer el ADC2 entero
	ADC2=LeerEPP()<<8;	//Lee parte alta
	ADC2+=LeerEPP();		//Lee parte baja
	EscribirEPP(ResetEstadisticas);	//Resetea estadisticas
	sigma=TINF;
	printLog("\n******** Placa levitador magnetico iniciada OK **********\n");
} //fin if levitador
else  //si es un pendubot
{

	//Inicializacion del Puerto EPP
	printLog("******** Inicializando Placa Pendulo Invertido ******");
	ERROR=0;			// Limpia el error

	iopl(3); 			// consige permisos de IO en memoria
	writeToPort(0x80,eppbase+0x402);
	if(Interrupts==1){
	      writeToPort(0xd4,eppcontrol); 	// configura Control Register para tener ints y setear senales de control
	      RequestIRQ(7,this);		// pide interrupcion al RTAI 
	}else{
	      writeToPort(0x04,eppcontrol); 	// configura Control Register para no tener ints y setear senales de control
	}
	//Inicalizacion del Equipo
	EscribirEPP(ComNOP);	//limpia por las dudas
	
	EscribirEPP(SetCuanto1);	//solicita cambio de cuanto
	EscribirEPP(Cuanto1-1);		//envia el cambio de cuanto
	EscribirEPP(SetCuanto2);	//solicita cambio de cuanto
	EscribirEPP(Cuanto2-1);		//envia el cambio de cuanto
	EscribirEPP(SetCuanto3);	//solicita cambio de cuanto
	EscribirEPP(Cuanto3-1);		//envia el cambio de cuanto

	EscribirEPP(ActivarINTS);	//activa interrupciones
	
	
	EscribirEPP(LeerCuanto1);
	uchar r=LeerEPP();
	
	if(r!=(Cuanto1-1)){	//Error!!! debe logearlo
		printLog("\nError en la inicializacion del cuanto 1. Cuanto recibido: %d\n******** No se pudo inicializar ***********\n",(int)r+1);
		ERROR=1;
		return;
	}
	
	EscribirEPP(LeerCuanto2);
	r=LeerEPP();
	
	if(r!=(Cuanto2-1)){	//Error!!! debe logearlo
		printLog("\nError en la inicializacion del cuanto 2. Cuanto recibido: %d\n******** No se pudo inicializar ***********\n",(int)r+1);
		ERROR=1;
		return;
	}

	EscribirEPP(LeerCuanto3);
	r=LeerEPP();
	
	if(r!=(Cuanto3-1)){	//Error!!! debe logearlo
		printLog("\nError en la inicializacion del cuanto 3. Cuanto recibido: %d\n******** No se pudo inicializar ***********\n",(int)r+1);
		ERROR=1;
		return;
	}

	desinc=1;		//indica que se esta inicializando y el adc esta desincronizado
	ADC1=0;			//por las dudas...
	ADC2=0;
	ADC3=0;
	inti = 0;
	EscribirEPP(LeerAltaADC1);	//comando para leer el ADC1 entero
	ADC1=LeerEPP()<<8;	//Lee parte alta
	ADC1+=LeerEPP();		//Lee parte baja
	EscribirEPP(LeerAltaADC2);	//comando para leer el ADC2 entero
	ADC2=LeerEPP()<<8;	//Lee parte alta
	ADC2+=LeerEPP();		//Lee parte baja
	EscribirEPP(LeerAltaADC3);	//comando para leer el ADC2 entero
	ADC3=LeerEPP()<<8;	//Lee parte alta
	ADC3+=LeerEPP();		//Lee parte baja
	dADC3[0]=(double)ADC3*360.0/4000.0-refpos*360.0/4000.0;
	if(dADC3[0]<0)dADC3[0]+=360.0;
	dADC2[0]=360.0-dADC3[0];
	
	evN2=10;evN3=10; //debe ser configurable
	
	evI2=0;evI3=0;

	evI2ant=evN2-1;evI3ant=evN3-1;	
		
	evT2=new double[evN2];
	evT3=new double[evN3];
	evA2=new double[evN2];
	evA3=new double[evN3];
	
	ind=0;
	while(ind<evN2){evT2[ind]=0;evA2[ind++]=dADC2[0];}
	while(ind<evN3){evT3[ind]=0;evA3[ind++]=ADC3;}
	
	cambio1=1;
	cambio2=1;
	cambio3=1;
	EscribirEPP(ResetEstadisticas);	//Resetea estadisticas
	sigma=0;
	aux=0;
	printLog("\n******** Placa Pendulo Invertido iniciada OK **********\n");
	//printLog("\nADC2:%d, dADC2:%f\n",ADC2,dADC2[0]);
  }
}
double adquider::ta(double t) {
//This function returns a double.
	return sigma;
}
void adquider::dint(double t) {
int cambio=cambio1+cambio2+cambio3+cambio2d+cambio3d;
    if((t-evT3[evI3ant]>TMAX) && (dDADC3[0]>1e-10))cambio++;
    if((t-evT2[evI2ant]>TMAX) && (dDADC2[0]>1e-10))cambio++;
    if(cambio>1)sigma=0;
    else sigma=TMAX;
}
void adquider::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
//     'e' is the time elapsed since last transition
	double *d;
	double  delta;
	if(ERROR==1)return;
	switch(x.port){ 
		case -1:	//Interrupcion del EPP
			if(desinc>0){	//debe resincronizar
				EscribirEPP(LeerAltaADC1);	//comando para leer el ADC1 entero
				ADC1=LeerEPP()<<8;	//Lee parte alta
				ADC1+=LeerEPP();		//Lee parte baja

				if(tipomaq == pendubot) {
						EscribirEPP(LeerAltaADC2);	//comando para leer el ADC2 entero
						short	ADC2temp;
						ADC2temp=LeerEPP()<<8;	//Lee parte alta
						ADC2temp+=LeerEPP();	//Lee parte baja
						delta=((double)(ADC2temp-ADC2))*360.0/4000.0;
						ADC2=ADC2temp;
						if(delta>180.0)delta-=360.0;
						else if(delta<-180.0)delta+=360.0;
						dADC2[0]+=delta*polrel;

						if (dADC2[0]>=360.0) dADC2[0]-=360.0;
						else if(dADC2[0]<0.0) dADC2[0]+=360.0;

						EscribirEPP(LeerAltaADC3);	//comando para leer el ADC2 entero
						ADC3=LeerEPP()<<8;	//Lee parte alta
						ADC3+=LeerEPP();	//Lee parte baja
						
						cambio3=1;					
						
						evA2[evI2]=dADC2[0];
						evT2[evI2]=t;
						evI2ant=evI2;
						if(evI2==(evN2-1)){
							delta=evA2[evI2]-evA2[0];
							if(delta>180)delta-=360;
							else if(delta<-180)delta+=360;
							dDADC2[0]=delta/(evT2[evI2]-evT2[0]);
							evI2=0;
						}else{
							delta=evA2[evI2]-evA2[evI2+1];
							if(delta>180)delta-=360;
							else if(delta<-180)delta+=360;
							dDADC2[0]=delta/(evT2[evI2]-evT2[evI2+1]);
							evI2++;
						}
						
						cambio2d=1;
						
						evA3[evI3]=ADC3;
						evT3[evI3]=t;
						evI3ant=evI3;
						if(evI3==(evN3-1)){
							delta=evA3[evI3]-evA3[0];
							if(delta>2000)delta-=4000;
							else if(delta<-2000)delta+=4000;
							dDADC3[0]=delta/(evT3[evI3]-evT3[0]);
							evI3=0;
						}else{
							delta=evA3[evI3]-evA3[evI3+1];
							if(delta>2000)delta-=4000;
							else if(delta<-2000)delta+=4000;
							dDADC3[0]=delta/(evT3[evI3]-evT3[evI3+1]);
							evI3++;
						}
						cambio3d=1;
						
				}else{
					EscribirEPP(LeerAltaADC2);	//comando para leer el ADC2 entero
					ADC2=LeerEPP()<<8;	//Lee parte alta
					ADC2+=LeerEPP();		//Lee parte baja
				}
				desinc=0;						//ya se sincronizo
				sigma=0;						//para emitir un evento de salida
				cambio1=1;						//activa bandera de cambio en
				cambio2=1;						//adc 1 y 2
				EscribirEPP(ResetEstadisticas);	//Resetea estadisticas
			 }else{
				inti++;
				uchar inc;
				inc=LeerEPP();				//lee el byte de incremento
				if(tipomaq == pendubot){
					if((inc&0x07)>0){								//solo actua si magnitud>0
						if((inc&0x8)==0)
							ADC1+=(inc&0x7)*Cuanto1;	//si es negativo el incremento invierte
						else
							ADC1-=(inc&0x7)*Cuanto1;					//suma al valor del ADC
						cambio1=1;							//indica cambio
						sigma=0;							//guarda para solicitar una salida
						
					 }
					if((inc&0x10)>0) {
						if((inc&0x20)==0){
							ADC2+=Cuanto2;	//si es negativo el incremento invierte
							dADC2[0]+=((double)Cuanto2)*constconv;
						}else{
							ADC2-=Cuanto2;
							dADC2[0]-=((double)Cuanto2)*constconv;
						}
						if (ADC2>=4000) ADC2-=4000;
						else if(ADC2<0) ADC2+=4000;
						if (dADC2[0]>=360.0) dADC2[0]-=360.0;
						else if(dADC2[0]<0.0) dADC2[0]+=360.0;


						evA2[evI2]=dADC2[0];
						evT2[evI2]=t;
						evI2ant=evI2;
						if(evI2==(evN2-1)){
							delta=evA2[evI2]-evA2[0];
							if(delta>180)delta-=360;
							else if(delta<-180)delta+=360;
							dDADC2[0]=delta/(evT2[evI2]-evT2[0]);
							evI2=0;
						}else{
							delta=evA2[evI2]-evA2[evI2+1];
							if(delta>180)delta-=360;
							else if(delta<-180)delta+=360;
							dDADC2[0]=delta/(evT2[evI2]-evT2[evI2+1]);
							evI2++;
						}
						
						sigma=0;//guarda para solicitar una salida	
						cambio2=1;							//indica cambio				
						cambio2d=1;
					}
					
					if((inc&0x40)>0) {
						if((inc&0x80)==0)
							ADC3+=Cuanto3;	//si es negativo el incremento invierte
						else
							ADC3-=Cuanto3;
						if (ADC3>=4000) ADC3-=4000;
						else if(ADC3<0) ADC3+=4000;
						

						evA3[evI3]=ADC3;
						evT3[evI3]=t;
						evI3ant=evI3;
						if(evI3==(evN3-1)){
							delta=evA3[evI3]-evA3[0];
							if(delta>2000)delta-=4000;
							else if(delta<-2000)delta+=4000;
							dDADC3[0]=delta/(evT3[evI3]-evT3[0]);
							evI3=0;
						}else{
							delta=evA3[evI3]-evA3[evI3+1];
							if(delta>2000)delta-=4000;
							else if(delta<-2000)delta+=4000;
							dDADC3[0]=delta/(evT3[evI3]-evT3[evI3+1]);
							evI3++;
						}
						
						cambio3d=1;
						cambio3=1;							//indica cambio
						sigma=0;							//guarda para solicitar una salida
					}
					
				}else{
				if((inc&0x7)>0){								//solo actua si magnitud>0
					if((inc&0x8)==0)
						ADC1+=(inc&0x7)*Cuanto1;	//si es negativo el incremento invierte
					else
						ADC1-=(inc&0x7)*Cuanto1;					//suma al valor del ADC
					cambio1=1;							//indica cambio
					sigma=0;							//guarda para solicitar una salida
				}
				if((inc&0x70)>0){								//solo actua si magnitud>0
					if((inc&0x80)==0)
						ADC2+=((inc&0x70)>>4)*Cuanto2;	//si es negativo el incremento invierte
					else
						ADC2-=((inc&0x70)>>4)*Cuanto2;					//suma al valor del ADC
					cambio2=1;							//indica cambio
					sigma=0;							//guarda para solicitar una salida
				}
				}

			}
			break;
		case 0:		//Cambio en cuanto 1
			d=(double*)x.value;
			if(*d>256)Cuanto1=256;
			else if(*d<1)Cuanto1=1;
			else Cuanto1=(ushort)*d;	//Lee valor de cuanto entregado al bloque
			
			EscribirEPP(SetCuanto1);	//solicita cambio de cuanto 1
			EscribirEPP(Cuanto1);		//envia el cambio de cuanto
			break;
		case 1:		//Cambio en cuanto 2
			d=(double*)x.value;
			if(*d>256)Cuanto2=256;
			else if(*d<1)Cuanto2=1;
			else Cuanto2=(ushort)*d;	//Lee valor de cuanto entregado al bloque
		
			EscribirEPP(SetCuanto2);	//solicita cambio de cuanto 2
			EscribirEPP(Cuanto2);		//envia el cambio de cuanto
			break;
		case 2:		//Cambio en cuanto 3
			if(tipomaq == pendubot) {
				d=(double*)x.value;
				if(*d>256)Cuanto3=256;
				else if(*d<1)Cuanto3=1;
				else Cuanto3=(ushort)*d;	//Lee valor de cuanto entregado al bloque
			
				EscribirEPP(SetCuanto3);	//solicita cambio de cuanto 3
				EscribirEPP(Cuanto3);		//envia el cambio de cuanto
				break;
			}
			else break;
		case 3:		//Cambio en la salida PWM
			short s;
			d=(double*)x.value;
			if(tipomaq == pendubot)
			{
				s=(short)((*d)*512.0/30.0)+512;
				if(s>1023)s=1023;
				else if(s<0) s=0;

				outb_p((unsigned char)((((s>>3)&0x60)|0x80)|SetPWMAlta),eppdata);	//genera comando y envia
				outb_p((unsigned char)(s&0xFF),eppdata);			//envia parte baja PWM
			}else{
				*d=(*d)*gainPWM;
				if(*d>1300){
				    s=1300;}
				else if(*d<0){ 
				    s=0;} 
				else{
				    s=*d;}
				EscribirEPP((uchar)(((s>>3)&0xE0)|SetPWMAlta));	//genera comando y envia
				EscribirEPP((uchar)(s&0xFF));			//envia parte baja PWM
			}	
			break;
		case 4:		//Evento en RESINC1
			EscribirEPP(LeerAltaADC1);	//comando para leer el ADC1 entero
			ADC1=LeerEPP()<<8;	//Lee parte alta
			ADC1+=LeerEPP();	//Lee parte baja
			cambio1=1;						//Bandera de cambio en ADC1
			sigma=0;						//para emitir un evento de salida
			break;
		case 5:		//Evento en RESINC2
			EscribirEPP(LeerAltaADC2);	//comando para leer el ADC2 entero
			if(tipomaq==pendubot){
				short	ADC2temp;
				ADC2temp=LeerEPP()<<8;	//Lee parte alta
				ADC2temp+=LeerEPP();	//Lee parte baja
				delta=((double)(ADC2temp-ADC2))*360.0/4000.0;
				ADC2=ADC2temp;
				if(delta>180.0)delta-=360.0;
				else if(delta<-180.0)delta+=360.0;
				dADC2[0]+=delta*polrel;
				if (dADC2[0]>=360.0) dADC2[0]-=360.0;
				else if(dADC2[0]<0.0) dADC2[0]+=360.0;
				
				evA2[evI2]=dADC2[0];
				evT2[evI2]=t;
				evI2ant=evI2;
				if(evI2==(evN2-1)){
					delta=evA2[evI2]-evA2[0];
					if(delta>180)delta-=360;
					else if(delta<-180)delta+=360;
					dDADC2[0]=delta/(evT2[evI2]-evT2[0]);
					evI2=0;
				}else{
					delta=evA2[evI2]-evA2[evI2+1];
					if(delta>180)delta-=360;
					else if(delta<-180)delta+=360;
					dDADC2[0]=delta/(evT2[evI2]-evT2[evI2+1]);
					evI2++;
				}
				
				cambio2d=1;
						
			}else{
				ADC2=LeerEPP()<<8;	//Lee parte alta
				ADC2+=LeerEPP();	//Lee parte baja
			}
			cambio2=1;						//Bandera de cambio en ADC2
			sigma=0;						//para emitir un evento de salida
			break;
		case 6:		//Evento en RESINC3
			if(tipomaq == pendubot) {
			EscribirEPP(LeerAltaADC3);	//comando para leer el ADC3 entero
			ADC3=LeerEPP()<<8;	//Lee parte alta
			ADC3+=LeerEPP();	//Lee parte baja
			
			evA3[evI3]=ADC3;
			evT3[evI3]=t;
			evI3ant=evI3;
			if(evI3==(evN3-1)){
				delta=evA3[evI3]-evA3[0];
				if(delta>2000)delta-=4000;
				else if(delta<-2000)delta+=4000;
				dDADC3[0]=delta/(evT3[evI3]-evT3[0]);
				evI3=0;
			}else{
				delta=evA3[evI3]-evA3[evI3+1];
				if(delta>2000)delta-=4000;
				else if(delta<-2000)delta+=4000;
				dDADC3[0]=delta/(evT3[evI3]-evT3[evI3+1]);
				evI3++;
			}
			
			cambio3d=1;
			cambio3=1;						//Bandera de cambio en ADC3
			sigma=0;						//para emitir un evento de salida
			
			}
			break;
	}
}
Event adquider::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)
	if(cambio2==1){
		cambio2=0;
		//if(cambio1==1 || cambio3==1 || cambio2d==1 || cambio3d==1)sigma=0;
		if(tipomaq == levitador){ //adc2 entrega en milimetros
			if(ADC2>1672)dADC2[0]= (double)ADC2*(-0.001749) + 13.32;
			else dADC2[0]=-3.8183*logf((double)ADC2)+38.7481;
			dADC2[0]-=6;
		}
		dADC2[0]*=gainADC2;
		return Event(dADC2,1);
	}
	if(cambio3==1){
		cambio3=0;
		//if(cambio1==1 || cambio2==1 || cambio2d==1 || cambio3d==1)sigma=0;
		dADC3[0]=(double)ADC3*360.0/4000.0-refpos*360.0/4000.0;
		if(dADC3[0]<0)dADC3[0]+=360.0;
		dADC3[0]*=gainADC3;
		return Event(dADC3,2);
	}
	if((t-evT2[evI2ant])>TMAX && dDADC2[0]>1e-10){
	      //if(cambio2==1 || cambio3==1 || cambio1==1 || cambio3d==1 || ((t-evT3[evI3ant])>TMAX && dDADC3[0]>1e-10))sigma=0;
	      dDADC2[0]=0;
	      return Event(dDADC2,3);
	}
	if((t-evT3[evI3ant] && dDADC3[0]>1e-10)>TMAX){
	      //if(cambio2==1 || cambio3==1 || cambio1==1 || cambio2d==1 || ((t-evT2[evI2ant])>TMAX && dDADC2[0]>1e-10))sigma=0;
	      dDADC3[0]=0;
	      return Event(dDADC3,4);
	}
	if(cambio1==1){
		cambio1=0;
		//if(cambio2==1 || cambio3==1 || cambio2d==1 || cambio3d==1)sigma=0;
		
		if(tipomaq == levitador)
			dADC1[0]=ADC1/781.5;
		else
			dADC1[0]=((double)ADC1-2022.5)*5.0/(4096.0*1.1);
		dADC1[0]*=gainADC1;
		return Event(dADC1,0);
	}	
	if(cambio2d==1){
		cambio2d=0;
		//if(cambio1==1 || cambio2==1 || cambio3==1 || cambio3d==1)sigma=0;
		if(dDADC2[0]>d2sup)dDADC2[0]=d2sup;
		else if(dDADC2[0]<d2inf)dDADC2[0]=d2inf;
		return Event(dDADC2,3);
	}
	if(cambio3d==1){
		cambio3d=0;
		//if(cambio1==1 || cambio2==1 || cambio3==1 || cambio2d==1)sigma=0;
		dDADC3[0]*=360.0/4000.0;
		if(dDADC3[0]>d3sup)dDADC3[0]=d3sup;
		else if(dDADC3[0]<d3inf)dDADC3[0]=d3inf;
		return Event(dDADC3,4);
	}


	return Event();
}
void adquider::exit() {
	if(ERROR==1)return;
	ushort maxerr1,maxerr2, maxerr3;
	EscribirEPP(SetPWMAlta);		//apaga la salida
	EscribirEPP(0x00);
	
	//Lectura de estadisticas de la performance del control
	
	EscribirEPP(LeerMaxErr1H);		//Lee maximo error de desincronizacion del ADC 1
	maxerr1=LeerEPP()<<8;
	EscribirEPP(LeerMaxErr1L);
	maxerr1+=LeerEPP();
	
	printLog("\nMaximo error de sincronizacion del canal 1: %d\n",(int)maxerr1);
	
	EscribirEPP(LeerMaxErr2H);		//Lee maximo error de desincronizacion del ADC 2
	maxerr2=LeerEPP()<<8;
	EscribirEPP(LeerMaxErr2L);
	maxerr2+=LeerEPP();
	
	printLog("\nMaximo error de sincronizacion del canal 2: %d\n",(int)maxerr2);
	
	if(tipomaq == pendubot) {
	EscribirEPP(LeerMaxErr3H);		//Lee maximo error de desincronizacion del ADC 3
	maxerr3=LeerEPP()<<8;
	EscribirEPP(LeerMaxErr3L);
	maxerr3+=LeerEPP();
	
	printLog("\nMaximo error de sincronizacion del canal 3: %d\n",(int)maxerr3);
	}
  
	EscribirEPP(LeerConf);
	uchar r=LeerEPP();
	//Log de las estadisticas
	if((r&0x4)!=0)
	{
	  printLog("\nSe ha activado la proteccion de sobrecorriente!\n");
	}
	printLog("Inti: %d", inti);
}
