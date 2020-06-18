
#if !defined adquiutil_h
#define adquiutil_h 

#define	ZERO_COR 2010


#define	TINF	1e20

#define	eppbase		0x378
#define	eppstatus	eppbase+1
#define	eppcontrol	eppbase+2
#define	eppdata		eppbase+4

#define polrel		22.0/28.0
#define constconv	polrel*360.0/4000.0


//definicion de tipo de maquina utilizada
#define levitador 0 
#define pendubot 1
#define pendubot_control 2

typedef	unsigned char uchar;
typedef	unsigned short int ushort; 

#define EscribirEPP(datos)	writeToPort(datos,eppdata)
#define LeerEPP()	readFromPort(eppdata)

int LeerVector(char * vectorbuf,double ** elementos);

#endif