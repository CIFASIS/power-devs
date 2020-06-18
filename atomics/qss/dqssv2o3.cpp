#include "dqssv2o3.h"
void dqssv2o3::init(double t,...) {
//printLog("%g ENTER init \n",t);     // DEBUG 
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);

char *fvar=va_arg(parameters,char*);
double InitialDelay=getScilabVar(fvar);		// Initial Delay parameter
	   										// Assumed: Positive values mean positive "Delays", i.e., retarded times. (Minus sign is implicit)

char *HistoryVar=va_arg(parameters,char*);	// History parameter
               // ---------- printLog("HistoryVar=%s\n",HistoryVar);	    // DEBUG

char buf[1024]; 

/////////////////////////////////////////////
// Loads History
//
sprintf(buf,"H_temp=%s",HistoryVar);
executeVoidScilabJob(buf,true);

sprintf(buf,"size(H_temp,1)");
int rows=(int)getScilabVar(buf);  // # rows of H (1st. row is the time vector)

sprintf(buf,"size(H_temp,2)"); // # columns of H (each column maps to a poly segment)
int columns=(int)getScilabVar(buf);


  //             printLog("rows=%d,columns=%d \n",rows,columns);	// DEBUG

if ((rows==1)&&(columns==1)) {
  // Constant History
       		  // ---------- printLog("Constant History \n");	// DEBUG
	

	PNi=1;
	PNo=0;  
	
	Bu[PNo][0]=getScilabVar("H_temp"); 
	Bu[PNo][1]=0;
	Bu[PNo][2]=0;
	Bu[PNo][3]=0;
  	Bt[PNo]=-InitialDelay;		// Make the segment as old as the Initial Delay.
   								// ???? CHECK. WHY NOT USING -INF... ?

	//			for (int j=rows-1;j<4;j++) {
    // 					######## // ---------- printLog("Bu[%d,%d]=%g \n",PNo,j,Bu[PNo][j]); // DEBUG 
	//			};
	
} else {

  // History as a Function of Time 
	       	 	// ---------- printLog("History is a function of time \n");	// DEBUG
  
    PNo=0;

	for (int i=0;i<columns;i++) { 
		// iterate columns (Assumed: timestamps is ascending with column's order)
		sprintf(buf,"H_temp(1,%d+1)",i);
		Bt[i]=getScilabVar(buf); // Timestamp of the beggining of the segment of history Bt[i] <= t < Bt[i+1]
		if (Bt[i]<=-InitialDelay){  // A history segment has been specified with information older that the maximum delay needed (Delay).
			PNo=i; 			 // Keeps track of it, remember the index for further use (to calculate a differential delay (ddelay)). 
							 // Segments < i will be ignored.
			     // ---------- printLog("Delay exceeded with t=%g \n",Bt[i]);	// DEBUG
		}
		         // ---------- printLog("Bt[%d]=%g \n",i,Bt[i]); // DEBUG

		for (int j=0;j<rows-1;j++) {
		// iterate rows
			sprintf(buf,"H_temp(%d+2,%d+1)",j,i); // skips the first row (i.e., the time vector)
			Bu[i][j]=getScilabVar(buf);
   			     // ---------- printLog("Bu[%d,%d]=%g \n",i,j,Bu[i][j]); // DEBUG
		};
		
		for (int j=rows-1;j<4;j++) {
			Bu[i][j]=0;	// Initializes the not declared elements of the Arrival Queue
		};
	};

  	PNi=columns;
};
     			// ---------- printLog("OutputPointer=%d , InputPointer=%d \n",PNo,PNi);	// DEBUG
//
/////////////////////////////////////////////

for (int i=0;i<10;i++){y[i]=0;}; // Output Initialization

for (int i=0;i<10;i++){Tau[i]=0;}; // Tau Initialization
Tau[0]=InitialDelay ;
tTau=0;

nxt='i';  // Next Expiration Initialization
		  // 'i'=Infinite (Never Expires)

/////////////////////////////////////////////
// Flow Control
//
phase=1;	// Pretend that a new Tau segment just arrived, to get everything properly intitialized
sigma=0;  	// Forces the internal transition
     		   // ---------- printLog("INIT completed \n");	// DEBUG
/////////////////////////////////////////////
}
double dqssv2o3::ta(double t) {
// ---------- printLog("%g \\ ENTER ta -- SIGMA: %g Phase: %i PNi: %d PNo: %d(%g) \n",t,sigma,phase,PNi,PNo,Bt[PNo]);     // DEBUG 
				// ---------- printLog("%g with Tau: [%g,%g,%g,%g] \n",t,Tau[0],Tau[1],Tau[2],Tau[3]);     // DEBUG 
				// ---------- printLog("\n");     // DEBUG 

	   return sigma;
}
void dqssv2o3::dint(double t) {
// dint(s)=s_aux (s_aux already calculated at lambda(s))
     
	 				  // ---------- printLog("%g ENTER dint \n",t);     // DEBUG 
	 				  
     sigma=tnext;    // Wait according the tnext calculated
}
void dqssv2o3::dext(Event x, double t) {
// ---------- printLog("%g / ENTER dext \n",t);     // DEBUG 
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number

	double* xv;
	xv=(double*)(x.value);

if (x.port==1) { /////////// New Y Arrived
     
				// ---------- printLog("%g New Y Arrived: [%g,%g,%g,%g] \n",t,xv[0],xv[1],xv[2],xv[3]);     // DEBUG 
				// ---------- printLog("%g begin with Tau: [%g,%g,%g,%g] \n",t,Tau[0],Tau[1],Tau[2],Tau[3]);     // DEBUG 

		Bt[PNi]=t;				// t is the global simulation time
		Bu[PNi][0]=xv[0]; 		// stores the event (polinomial coeff.) just arrived
		Bu[PNi][1]=xv[1];
		Bu[PNi][2]=xv[2];
		Bu[PNi][3]=xv[3];
	
		// Handling of special case: // The new segment comes inmediately after the 
		if(PNi==(PNo+1)%MaxQ) {      // segment currently at the output (last emitted,
									 // at the head of the queue).
                double Tau_now=evaluate_poly(Tau,e,3) ; // How much is Tau right now, which tells the delay for the incoming event
                
                // ---------- printLog("%g (New Y is the next output, %g seconds from now) \n",t,Tau_now); 
			    
				ty_Fwd=e+Tau_now;      // Updates ty_Fwd, because a new segment apeared at the right of the current PNo

				// ---------- printLog("%g now  Tau: [%g,%g,%g,%g] \n",t,Tau[0],Tau[1],Tau[2],Tau[3]);     // DEBUG 
		  
               if (zone_Tau=='f')  {	 // The delay was moving FORWARDS in time.
			 			if (nxt=='t')	{      // Next event is a planned zone change for Tau.
						   					   // (sigma-e is the time remaining to complete ttau_Inv)
								  			   if (Tau_now<sigma-e) {sigma=Tau_now;    // The new segment y must be treated next, before the Tau changes
										                             phase=4;          // Now y will expire first
						  	                                 } else {sigma=sigma-e;};  // The planned change in Tau must be treated next 
						          };
						if (nxt=='y')	{      // There is a planned change in y
					  		     // ---------- printLog("%g ERROR: Conflict at the head of the queue \n",t); // ... this shouldn't happend in this special case !
					  		     sigma=sigma-e;
					  		     };							 
						if (nxt=='i')	{       // There is no planned change  							 
								 sigma=Tau_now; // The new segment Y must be treated next
								 phase=4;       // Now Y will expire first
						   }; }; 
		       if (zone_Tau=='b') {		  // The delay was moving BACKWARDS in time.
					sigma=sigma-e;        // Resume previous wait, whatever it was (for Tau or for Y)
  					       };
		       if (zone_Tau=='s') {		  // The delay was remaining fixed at a stationary point.
					sigma=INF; 
					ttau_Inv=INF; 				  
			               };
		} else { 
		// No special case
			sigma=sigma-e;		  // Resume previous wait.
		};
		  // ---------- printLog("%g now  Tau: [%g,%g,%g,%g] \n",t,Tau[0],Tau[1],Tau[2],Tau[3]);     // DEBUG 		
		PNi=(PNi+1)%MaxQ;
};

if (x.port==0) { ///////// New Tau Arrived

   			   // ---------- printLog("%g New Tau Arrived: [%g,%g,%g,%g] \n",t,xv[0],xv[1],xv[2],xv[3]);     // DEBUG 

        //#######// Load new Tau

		Tau[0]=xv[0]; // stores the event (polinomial coeff.) just arrived
		Tau[1]=xv[1];
		Tau[2]=xv[2];
		Tau[3]=xv[3];
		tTau=t;			// stores the timestamp of the new Tau segment

		phase=1 ;	// "New Tau Arrived"
		sigma=0	;	// Forces internal transition
   };
}
Event dqssv2o3::lambda(double t) {
// ---------- printLog("%g / ENTER lambda   \n",t);     // DEBUG 
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)

double 	Delay_target ;   // Target Delay to be used as the new absolute time where Tau aims at. (Delay_target=t-Tau[0])
long 	PNt=-1 ;  		 // Target Pointer to be used as the new value for PNo
	 		     		 // Starts up with an invalid pointer value (-1), for further consistency check
	 		  
   switch (phase) {
	  case 1:			// New Tau segment arrived
	  
	  	        // ---------- printLog("%g 		Recalculating... (new Tau arrived) \n",t);     // DEBUG 
	  
		  //#######// TAU: Determine order: zone_Tau
		  order_Tau=det_order(Tau);	

		  //#######// TAU: Determine zone: order_Tau
		  zone_Tau=det_zone(Tau);

   		  //#######// TAU: Determine invertion time of direction (Backwards<->Forward): ttau_Inv
		  if (Tau[2]==0 && Tau[3]==0) { 	// At most linear evolution of Tau, won´t change its direction
 			      ttau_Inv=INF;
		  } else {double Tau_linDetrend_deriv[4]; // Tau can change direction wrt f(t)=t, have to calculate when it will be.
		  		 		 						  // Takes Tau, removes 1 to the linear component, and derivates it 
		  		 		 						  // to find the inversion time (a max. or a min.)
				  Tau_linDetrend_deriv[0]=Tau[1]-1 ; 
				  Tau_linDetrend_deriv[1]=2*Tau[2] ;  
				  Tau_linDetrend_deriv[2]=0 ; // 
				  Tau_linDetrend_deriv[3]=0 ; // 
				  ttau_Inv=minposroot(Tau_linDetrend_deriv,1);	// Find when the next direction change will happend
				  
				  // ---------- printLog("Tau_linDetrend_deriv: [%g,%g,%g,%g] \n",Tau_linDetrend_deriv[0],Tau_linDetrend_deriv[1],Tau_linDetrend_deriv[2],Tau_linDetrend_deriv[3]);   // DEBUG 					 
		  };
		  
  		  		  // ---------- printLog("Next Tau Inversion Time: %g \n",ttau_Inv);   // DEBUG 	 

		  	 	  // ---------- printLog("%g 		Tau: order: %d zone: %c \n",t,order_Tau,zone_Tau); // DEBUG
	  	
		  //#######// Find new target segment pointer (PNt), departing from the current segment (PNo)
		  
		  Delay_target=t-Tau[0]; // use the constant coeff. of Tau. (t is "now")
			 
			      // ---------- printLog("%g 		Tau[0]: %g  Delay_target: %g \n",t,Tau[0],Delay_target);
		  		  
		  if (Delay_target<Bt[PNo]) { // Lookup BACKWARDS, from PNo-1
		  	      // ---------- printLog("%g 		Lookup Backwards \n",t);
		  	 if (Delay_target<Bt[0]){PNt=0;}   	// Special Case: Delay is older than the oldest input. Avoids the search.
	   		 	else { 							// No Special Case
					   for (int i=PNo-1;i>=0;i--){ // Delay is somewhere between the oldest input and the present output segment.
			 			 	  if (Bt[i]<=Delay_target){PNt=i;	// Found it
					 						 break; }; }; }; };  

		  if (Delay_target>=Bt[PNo]) { // Lookup FORWARDS, from PNo
		  	      // ---------- printLog("%g 		Lookup Forwards \n",t);
				      for (int i=PNo;i<PNi;i++) { 
		          // ---------- printLog("%g 		i: %d  Bt[i]: %g Bt[i+1]: %g \n",t,i,Bt[i],Bt[i+1]);
		  	 		   	      if (i==PNi-1) {PNt=i;}   // Special Case: This is the last segment in the queue. Avoids the search.
		  				   	      else { if (Bt[i+1]>Delay_target){ 
				   						      // ---------- printLog("%g ENTER HERE Subs: %g \n",t,Bt[i+1]-Delay_target); 
										 PNt=i;	// Found it (it is actually overpassed)
						                    break; }; }; }; };

		  if (PNt<0) {     // CONSISTENCY CHECK
		   	 		  printLog("%g ERROR: FAILED to find new PNt or PNt less than 0 \n",t);
				      PNt=PNo;}; // ERROR, Make PNt=PNo (segment unchanged)
		  
		  //#######// Calc. new ty_Bck, ty_Fwd
		  // New ty_Bck
		  if (PNt==0 && Delay_target<Bt[0]) // Special Case: Delay older than oldest segment.   
		  	 	      {ty_Bck=INF;} 
		    else 							// No special case
				      {ty_Bck=Delay_target-Bt[PNt];}; // How much older is Bt[PNt] than Delay_target

          if (ty_Bck<0) {      printLog("%g ERROR: ty_Bck<0 -> Force ty_Bck=0 \n",t);
			 		  ty_Bck=0;}; // Consistency Check
					   
		  // New ty_Fwd
		  if (PNt==PNi-1)				  			   // Special Case: Delay falls in the newest segment.   
		  	          {ty_Fwd=INF;} 
		    else { if (PNt==0 && Delay_target<Bt[0])   // Special Case: Delay older than oldest segment.   
				      {ty_Fwd=-Bt[PNt]-Delay_target;}  // How much newer is Bt[0] than Delay_target
				 else 								   // No special case
				 	  {ty_Fwd=Bt[PNt+1]-Delay_target;  // How much newer is Bt[PNt+1] than Delay_target
					   }; }; 

	      if (ty_Fwd<0) {      printLog("%g ERROR: ty_Fwd<0 -> Force ty_Fwd=0 \n",t);
		  	 		  ty_Fwd=0;}; // CONSISTENCY CHECK
		  break;
		  
	  case 3:			// Tau segment expired
	  
	  	        // ---------- printLog("%g 		Recalculating... (Tau expired) \n",t);     // DEBUG 
	
		  // THIS ONLY HAPPENS in QSS3 or HIGHER. 

          // Tau: SWITCH Tau Zone
		  if (zone_Tau=='f') {	zone_Tau='b';
                      } else {  zone_Tau='f';}; 
   		  
		  			  // ---------- printLog("%g Original Tau: [%g,%g,%g,%g] \n",t,Tau[0],Tau[1],Tau[2],Tau[3]);     // DEBUG 
		  
    	  // Advances Tau until the instant of its slope change (now), generating a new Tau from there on
    	  // t-tTau = how much time elapsed since the Tau arrival until this slope change
		  advance_time(Tau,t-tTau,-1); 
		  
		  			  // ---------- printLog("%g Advanced Tau: [%g,%g,%g,%g] %g secs. \n",t,Tau[0],Tau[1],Tau[2],Tau[3],t-tTau);     // DEBUG 
		  tTau=t;
          
		  //#######// TAU: Determine the time of invertion of direction (Backwards<->Forward): ttau_Inv
		  			  // For QSS3 there should exist only one inversion in Tau. So a recalc. should yield always ttau_I=INF.
	      ttau_Inv=INF; // FORCE !CBH!
  		  
		  			  // ---------- printLog("Next Tau Inversion Time: %g \n",ttau_Inv);   // DEBUG 	 
		  			  // ---------- printLog("%g 		Tau: order: %d zone: %c \n",t,order_Tau,zone_Tau); // DEBUG
	  	
		  //#######// The target pointer PNt remains in the current PNo 
	      PNt=PNo; 
	      
		  			  // ---------- printLog("PNt: %d \n",PNt);   // DEBUG 	 

		  //#######// Repositions Delay_target with the new (current) Tau[0]
		  Delay_target=t-Tau[0];
		  			  // ---------- printLog("New Delay_target: %g \n",Delay_target);   // DEBUG 	 
		  
		  //#######// Calc. new ty_Bck, ty_Fwd
		  // New ty_Bck
		  if (PNt==0 && Delay_target<Bt[0]) // Special Case: Delay older than oldest segment.  ???? VALIDATE USAGE of INF
		  	 	      {ty_Bck=INF;} 
		    else 							// No special case
				      {ty_Bck=Delay_target-Bt[PNt];}; // How much older is Bt[PNt] than Delay_target

          if (ty_Bck<0) {       printLog("%g ERROR: ty_Bck<0 -> Force ty_Bck=0 \n",t);
			 		  ty_Bck=0;}; // CONSISTENCY CHECK
					   
		  // New ty_Fwd
		  if (PNt==PNi-1)				  			 // Special Case: Delay falls in the newest segment.  ???? VALIDATE USAGE of INF
		  	          {ty_Fwd=INF;} 
		    else { if (PNt==0 && Delay_target<Bt[0]) // Special Case: Delay older than oldest segment.  ???? VALIDATE USAGE of INF
				      {// ---------- printLog("Enter special case: DOOS \n");     // DEBUG 
					   ty_Fwd=-Bt[PNt]-Delay_target;}  // How much newer is Bt[0] than Delay_target
				 else 									// No special case
				 	  {// ---------- printLog("No special case \n");     // DEBUG 
					   ty_Fwd=Bt[PNt+1]-Delay_target;
					      // ---------- printLog("Bt[PNt]: %g \n",Bt[PNt]);   // DEBUG 	 
    		    		  // ---------- printLog("Bt[PNt+1]: %g \n",Bt[PNt+1]);   // DEBUG 	 
					   }; }; // How much newer is Bt[PNt+1] than Delay_target

	      if (ty_Fwd<0) {       printLog("%g ERROR: ty_Fwd<0 (%g) -> Force ty_Fwd=0 \n",t,ty_Fwd);
		  	 		  ty_Fwd=0;}; // CONSISTENCY CHECK

		  break;
		    
	  case 4:			// Y segment expired
	  
	  	        // ---------- printLog("%g 		Recalculating... (Y expired) \n",t);     // DEBUG 

	  //#######// Recalc. invertion time of Tau direction (Backwards<->Forward): ttau_Inv
		  if (Tau[2]==0 && Tau[3]==0) {ttau_Inv=INF;	            // At most linear evolution of Tau, won´t change its direction	      
		  } else { if (ttau_Inv!=INF) {ttau_Inv=ttau_Inv-sigma;};}; // Updates ttau_Inv, the expiration time for Tau. 
	                                                			    // tnext holds the last elapsed time for y (which now expired)
 
	  	        // ---------- printLog("Next Tau Inversion Time: %g \n",ttau_Inv);   // DEBUG 
	  	        // ---------- printLog("tnext from previous cycle: %g \n",sigma);   // DEBUG 	 
 
	  //#######// Recalc. ty_Bck, ty_Fwd            AND 		  
	  //#######// Calculates new PNt (Increment/Decrement PNo)

       if (zone_Tau=='f') {	// The delay was MOVING FORWARDS in time, and reached the beggining of the NEXT Y segment
	   		  // New ty_Bck
  			  ty_Bck=0; 		// Y segment exipred, the next begins, so the distance backwards to the inmediate next beggining of a segment is always 0

		  	  // New ty_Fwd
			  if (PNo==0 && Tau[0]<Bt[0]) { // Special Case: Delay began older than oldest segment.   
		  	 	      if   (PNi==(PNo+1)%MaxQ) {ty_Fwd=INF; 	// How many elements already enqueued ?
   						 						  PNt=0;} 
				                            else {ty_Fwd=Bt[(PNo+1)%MaxQ]-Bt[PNo]; 
				 						          PNt=0;};  
			  } else {
					  if (PNi==(PNo+2)%MaxQ) {      // Special Case: The NEXT segment is the last in the queue 
		                      ty_Fwd=INF;
		                      PNt=(PNo+1)%MaxQ;
			              } else {					// No special case
								  ty_Fwd=Bt[(PNo+2)%MaxQ]-Bt[(PNo+1)%MaxQ];
			                      PNt=(PNo+1)%MaxQ;
			     		         };	};
              
			  if (ty_Fwd<0) {       printLog("%g ERROR: ty_Fwd<0 -> Force ty_Fwd=0 \n",t);
			  	 		  ty_Fwd=0;}; // CONSISTENCY CHECK
			  	 		  
              if (PNt>PNi-1) {      printLog("%g ERROR: PNt>PNi-1 -> Force PNt=PNi-1 \n",t);
 		  	 			            printLog("%g ERROR: PNt=%d , PNi=%d , PNo=%d \n",t,PNt,PNi,PNo);
 		  	 			  PNt=PNi-1;}; // CONSISTENCY CHECK
       }; 											

       if (zone_Tau=='b') {		  // The delay was MOVING BACKWARDS in time, and reached the beggining of the CURRENT Y segment
	   		  // New ty_Bck
			  if (PNo==0) {       // Special Case: Crossing towards a zone older than the oldest segment.
		  	 	      ty_Bck=INF;
					  PNt=0; 
              } else {			  // No special case
              		  ty_Bck=Bt[PNo]-Bt[PNo-1];
              		  PNt=PNo-1;
  		      };
   	          if (ty_Bck<0) {     printLog("%g ERROR: ty_Bck<0 -> Force ty_Bck=0 \n",t);
				 		  ty_Bck=0;}; // CONSISTENCY CHECK

              if (PNt<0)  {        printLog("%g ERROR: PNt<0  -> Force PNt=0  \n",t);
			  	 		  PNt=0;}; // CONSISTENCY CHECK
 	
		  	  // New ty_Fwd
  			  ty_Fwd=0; 		// Y segment exipred, the previous begins, so the distance forwards to the inmediate beggining of a segment is always 0
	   };
		  	  
       if (zone_Tau=='s') {		  // The delay was remaining FIXED in time, targeting a stationary point.
       	  				  // CONSISTENCY CHECK
					      printLog("ERROR: Y changed with Tau Stationary");     // THIS SHOULDN'T HAPPEND HERE ! If Tau is stationary, a Y change is never scheduled
			 };  			  
		  break;


       if (PNt<0) {   // ---------- printLog("%g ERROR: FAILED to find new PNt or PNt less than 0 \n",t);
				      PNt=PNo;}; // ERROR, Make PNt=PNo (output segment unchanged)

	  case 5:		 
	  	        // ---------- printLog("%g 		Recalculating... (No expiration scheduled) \n",t);     // DEBUG 
		        // Do Nothing
		   break;
	  };

	  //#######// At this Point PNt holds the new target value for PNo	
	  //#######// calculated at one of the "cases" above.

                    // ---------- printLog("%g               Target PNt: %d ty_Bck: %g ty_Fwd: %g \n",t,PNt,ty_Bck,ty_Fwd); // DEBUG

	  if (PNt<0) {  // CONSISTENCY CHECK
	  	 		      printLog("%g ERROR: FAILED to find new PNt or PNt less than 0. Force PNt=PNo. \n",t);
				   PNt=PNo;}; // ERROR, Make PNt=PNo (segment unchanged)

	  //#######// Recalculates     	tnext, nxt and y 
	  //#######// according to		ty_Bck,ty_Fwd,PNt,order_Tau,zone_Tau,Tau
	  
	  refresh_state(PNt); 

   	  PNo=PNt;     	// Updates to the new target segment
  	  
	  //#######// Adjust the phase, according to the next expected change (nxt) calculated  
	  
	  switch (nxt) { 
		  case 't':	phase=3;		// (Phase where Tau will expire first)
		  	   break;
		  case 'y':	phase=4;		// (Phase where Y will expire first)
		   	   break;
		  case 'i': phase=5;		// (Infinite: Phase were no expiration is expected)
		  	   break;
		  default:      			// ---------- printLog("%g ERROR: INVALID nxt value:%g \n",t,nxt); // ERROR;
               break;
			 };

			 // ---------- printLog("%g               Output: [%g,%g,%g,%g] \n",t,y[0],y[1],y[2],y[3]);     // DEBUG 

      //#######// y[] is already properly calculated
			 	  
      return Event(y,0);
}
void dqssv2o3::exit() {

}
