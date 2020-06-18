#if !defined comandos_h
#define comandos_h 

#define	ComNOP		0	// ComNOP
#define	ResetPIC	1	// Resetea necesario para cambiar el modo del ADC
#define	SetADC10	2	// Adc en modo 10 bits
#define	SetADC12	3	// Adc en modo 12 bits
#define	SetCuanto1	4	// Setea valor de cuando 1-256, el valor se envia en el byte posterior
#define	SetCuanto2	5	// Setea valor de cuando 1-256, el valor se envia en el byte posterior
#define	SetPWMAlta	6       // Activa la escritura del Duty Cycle
#define	SetPWMBaja	7	// Escritura de la parte baja del Duty Cycle
#define	LeerCuanto1	8	// Lee el valor de cuanto 1, debe leerse un byte inmediatamente
#define	LeerCuanto2	9	// Lee el valor de cuanto 2, debe leerse un byte inmediatamente
#define	LeerAltaADC1	10	// Lee la parte alta del ADC1, este comando activa leer parte baja
#define	LeerBajaADC1	11	// Es un comando interno.. solo usar leer parte alta
#define	LeerAltaADC2	12	// Lee la parte alta del ADC2, este comando activa leer parte baja
#define	LeerBajaADC2	13	// Es un comando interno.. solo usar leer parte alta
#define	LeerMaxErr1H	14	// Lee parte alta de maximo error de desinc. del ADC 1		
#define	LeerMaxErr1L	15	// Lee parte baja de maximo error de desinc. del ADC 1
#define	LeerMaxErr2H	16	// Lee parte alta de maximo error de desinc. del ADC 2		
#define	LeerMaxErr2L	17	// Lee parte baja de maximo error de desinc. del ADC 2
#define	LeerMaxDelay1H	18	// Lee maximo delay en la respuesta de la PC. parte alta		
#define	LeerMaxDelay1L	19	// Lee maximo delay en la respuesta de la PC. parte baja
#define	LeerConf	20	// Lee BYTE de configuracion	
#define	ActivarINTS	21	// Activa las interrpuciones del EPP
#define	DesActivarINTS	22	// Enmascara las interrupciones del EPP
#define	ResetEstadisticas 23	// Limpia las estadisticas de maxerror
#define	SetCuanto3	24	// Setea valor de cuanto para el encoder 2. 1-256
#define	LeerAltaADC3	25	//
#define	LeerBajaADC3	26	//
#define	LeerMaxErr3H	27	//
#define	LeerMaxErr3L	28	//
#define	LeerCuanto3	29	//	
#define ContCorrOn	30	//
#define	ContCorrOff	31	//
#endif

