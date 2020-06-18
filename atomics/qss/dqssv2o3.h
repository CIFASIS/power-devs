//CPP:qss/qsstools.cpp

//CPP:qss/dqssv2o3.cpp
#if !defined dqssv2o3_h
#define dqssv2o3_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "qss/qsstools.h"


class dqssv2o3: public Simulator { 
// Declare the state, output
// variables and parameters

#define INF 1e20
#define epsilon 1e-12
#define MaxQ 10000000

// States

double Bt[MaxQ];
double Bu[MaxQ][4];
long PNi, PNo;

double sigma;
double Tau[10];	// The Delay(t)
double tTau;	// The timestamp of the current Delay segment

double ty_Fwd;	// Forward Lifetime of current output segment
	   			// (from "now" until the segment's "end")
	   			
double ty_Bck;	// Backward Lifetime of current output segment
	   			// (from "now" until the segment's "beggining")
	   			
double ttau_Inv;// Lifetime of Tau in its current direction (Backwards or Forward),
	   			// before it changes its direction (i.e., crosses the f(t)=t slope). (only for QSS3 and higher)

double tnext;	// Time remaining for the next sooner change (in Tau or Y)	   			

char nxt;		// nxt=“t”: (Tau will expire first)
	 			// nxt=“y”: (Y will expire first)
	   			// nxt=“inf”(No expiration expected)

char zone_Tau ; // zone of the derivative of Tau with respect to 
	 		  	// the evolution of derivative=1 (slope=45 degrees)
	 		  	// "b" (backwards): derivative<1
				// "s" (stationary): derivative=1 
				// "f" (forwards): derivative>1
				
int order_Tau ; // Maximum non-zero coefficient of the Tau polinomial segment

int phase;  // 1=NewT, 2=NewY, 3=NxtChgT, 4=NxtChgY, 5=NxtChgInf

char cycle; // "A":Recalculate and Send Output Cycle, "B":Schedule Cycle
	 		  	
//output
double y[10];

// Helper Functions

int det_order(double *coeff) {
	int ordr;
        if (coeff[3]!=0){
			ordr=3;
		} 
		else if (coeff[2]!=0) {
			ordr=2;
		} 
		else if (coeff[1]!=0) {
			ordr=1;
		}
		else {
			ordr=0;
		};	
	return ordr;
};

char det_zone(double *coeff) {
	 char zn;
		if (coeff[1]>1)      {
			zn='b';
		} 
		else if (coeff[1]==1){
			 if (coeff[2]==0){zn='s';} else // Stationary. Follows f(t)=t.
 			 if (coeff[2]<0) {zn='f';} else // Forward, defined by the sign of the Quadratic degree.
			 if (coeff[2]>0) {zn='b';};		// Backwards, defined by the sign of the Quadratic degree.
		} 
		else if (coeff[1]<1) {
			zn='f';
		};
	return zn;
};

void refresh_state(long PNt){

        //#######// Recalculates tnext, nxt, and y

 		    //
 			// Repositions at the Beggining of the Target Segment
 			//
 			
            y[0]=Bu[PNt][0]; 
		    y[1]=Bu[PNt][1];
		    y[2]=Bu[PNt][2];
		    y[3]=Bu[PNt][3];

    switch (order_Tau) {

	   case 0:	// Tau is CONSTANT segments
                // ---------- printLog("      	Refreshing for CONSTANT Tau \n"); // DEBUG

	   		tnext=ty_Fwd;
	   		
	   		if (tnext==INF) {nxt='i';}
                       else {nxt='y';};
                       
	   		if (ty_Bck==INF) {// Special case: Delay older than oldest segment.
			   				 // Calculates new Segment from the Initial Position
							 advance_time(y,-ty_Fwd,-1);}  // Extrapolates backwards ("invents" a piece of segment).
                       else {// No special case
			   				 // Calculates new Segment from the Initial Position
							 advance_time(y,ty_Bck,-1);};  // Advances the segment forward in time, since its beggining, ty_Bck units ahead
	   		break;

	   case 1:	// Tau is LINEAR segments
	   
       			 // ---------- printLog("      	Refreshing for LINEAR Tau \n"); // DEBUG

	   		if (zone_Tau=='b'){   // Tau is moving BACKWARDS
	   		
		         // ---------- printLog("      	(moving backwards) \n"); // DEBUG
		         
				  if (ty_Bck==INF) {        // Special case: Delay older than oldest segment.
		  	 			 tnext=INF;  
			 			 nxt='i';
				         // Adjust Initial Position (Find target Instant inside target Segment)
			             advance_time(y,-ty_Fwd,-1); // Extrapolates backwards ("invents" a piece of segment backwards in time).
						 // Calculates new coefficients
					  	     // y[0]=y[0]-y[1]*Tau[0];  // This is already achieved with the previous adjustment
		 				 y[1]=y[1]-y[1]*Tau[1];
				   } else { 	           // No special case
	 	 	             /// !CBH! tnext=ty_Bck/Tau[1]; // Calculation by slopes
				  	   	 tnext=ty_Bck/(Tau[1]-1); // Calculation by slopes
				  	   	 nxt='y';
				         // Adjust Initial Position (Find target Instant inside target Segment)
			             advance_time(y,ty_Bck,-1);  
						 // Calculates new coefficients
					  	     // y[0]=y[0]-y[1]*Tau[0];  // This is already achieved with the previous adjustment
		 				 y[1]=y[1]-y[1]*Tau[1];
						 };
				   };
			  
	   		if (zone_Tau=='s') {   // Tau is STATIONARY
	   		
		         // ---------- printLog("      	(stationary) \n"); // DEBUG
		         
  	 	             tnext=INF;
			  	   	 nxt='i';
			         // Adjust Initial Position (Find target Instant inside target Segment)
			  		 if  (ty_Bck==INF) {    // Special case: Delay older than oldest segment.
        			 	 advance_time(y,-ty_Fwd,-1);
					 } else {             // No Special Case 
		                 advance_time(y,ty_Bck,-1);};  
						 // Calculates new coefficients
					  	     // y[0]=y[0]-y[1]*Tau[0];  // This is already achieved with the previous adjustment
		 				 y[1]=0;           // Only leaves the constant coeff y[0] present at the output
		 				 y[2]=0; 		   // 
		 				 y[3]=0; 		   // 
			   	     };

	   		if (zone_Tau=='f') {   // Tau is moving FORWARDS 
	   		
		         // ---------- printLog("      	(moving forwards) \n"); // DEBUG
			    if (ty_Bck==INF) {           // Special case: Delay older than oldest segment.
		         // ---------- printLog("      	(enter b inf) \n"); // DEBUG
	  	 			 // !CBH! tnext=ty_Fwd/Tau[1]; // Calculation by slopes 
	  	 			 tnext=ty_Fwd/(1-Tau[1]); // Calculation by slopes 
		 			 nxt='y';
			         // Adjust Initial Position (Find target Instant inside target Segment)
		             advance_time(y,-ty_Fwd,-1); // Extrapolates backwards ("invents" a piece of segment backwards in time).
					 // Calculates new coefficients
				  	     // y[0]=y[0]-y[1]*Tau[0];  // This is already achieved with the previous adjustment
	 				 y[1]=y[1]-y[1]*Tau[1];
			   } else { if (ty_Fwd==INF) {   // Special case: This is the Last Segment in tue queue
		         // ---------- printLog("      	(enter f inf) \n"); // DEBUG
	  	 			 tnext=INF;  
		 			 nxt='i';
			         // Adjust Initial Position (Find target Instant inside target Segment)
		             advance_time(y,ty_Bck,-1);  
					 // Calculates new coefficients
				  	     // y[0]=y[0]-y[1]*Tau[0];  // This is already achieved with the previous adjustment
	 				 y[1]=y[1]-y[1]*Tau[1];
			   } else { 	                // No special case
		         // ---------- printLog("      	(enter normal) \n"); // DEBUG
 	  	 			 tnext=ty_Fwd/(1-Tau[1]); // Calculation by slopes 
		 			 nxt='y';
  					   				 		         // ---------- printLog("Initial Y: [%g,%g,%g,%g] \n",y[0],y[1],y[2],y[3]);     // DEBUG 
			         // Adjust Initial Position (Find target Instant inside target Segment)
		             advance_time(y,ty_Bck,-1);  
  					 						         // ---------- printLog("Readj.  Y: [%g,%g,%g,%g] \n",y[0],y[1],y[2],y[3]);     // DEBUG 
					 // Calculates new coefficients
				  	     // y[0]=y[0]-y[1]*Tau[0];  // This is already achieved with the previous adjustment
	 				 y[1]=y[1]-y[1]*Tau[1];
	                                                 // ---------- printLog("Readj.Slope: [%g,%g,%g,%g] \n",y[0],y[1],y[2],y[3]);   // DEBUG 					 };	}; 
 											  	     // ---------- printLog("with Tau[1]: %g \n",Tau[1]);     // DEBUG 					 
			   		   };	};   };
 	   		break;

	   case 2:	// Tau is PARABOLIC segments
		         // ---------- printLog("      	Refreshing for PARABOLIC Tau \n"); // DEBUG
				 
	   		if (zone_Tau=='b'){   // Tau is moving BACKWARDS
		         // ---------- printLog("      	(moving backwards) \n"); // DEBUG

			    if (ty_Bck==INF) {        // Special case: Delay older than oldest segment.
 			  	      if (ttau_Inv==INF) {tnext=INF;  	  // Tau does not change direction
		 			 			          nxt='i'; 
					              } else {tnext=ttau_Inv; // Tau direction change will predominate
		 			  	                  nxt='t';};		   
			         // Adjust Initial Position (Find target Instant inside target Segment)
		             advance_time(y,-ty_Fwd,-1); // Extrapolates backwards ("invents" a piece of segment backwards in time).

					 // Calculates new coefficients
				  	     // for QSS2: y[0]=y[0]-y[1]*Tau[0];  // This is already achieved with the previous adjustment
      					 // for QSS2: y[1]=y[1]-y[1]*Tau[1];
//                       //  (Tau[0]^2 y[2] - Tau[0] y[1] + y[0])  // This is already achieved with the previous adjustment

					 y[1]=(2*Tau[0]*0*Tau[1]*y[2] - 2*Tau[0]*0*y[2] - Tau[1]*y[1] + y[1]);
					 y[2]=(2*Tau[0]*0*Tau[2]*y[2] + Tau[1]*Tau[1]*y[2] - 2*Tau[1]*y[2] + y[2] - Tau[2]*y[1]);
					 y[3]=(Tau[1]*Tau[2]*y[2] - 2*Tau[2]*y[2]);
//					 y[4]=(Tau[2]*Tau[2]*y[2]);

 			   } else { 	           // No special case
			   	 
                  double Tau_linDetrend_ty_Nxt[4] ; 
				  Tau_linDetrend_ty_Nxt[0]=-ty_Bck ; 
				  Tau_linDetrend_ty_Nxt[1]=Tau[1]-1 ;  
				  Tau_linDetrend_ty_Nxt[2]=Tau[2] ; // 
				  Tau_linDetrend_ty_Nxt[3]=0 ; // 
				  double ty_Nxt=minposroot(Tau_linDetrend_ty_Nxt,2);	// Find when the next direction change will happend
				  // ---------- printLog("Tau_linDetrend_ty_Nxt: [%g,%g,%g,%g] \n",Tau_linDetrend_ty_Nxt[0],Tau_linDetrend_ty_Nxt[1],Tau_linDetrend_ty_Nxt[2],Tau_linDetrend_ty_Nxt[3]);   // DEBUG 					 
				  // ---------- printLog("ty_Nxt: %g \n",ty_Nxt);   // DEBUG 					 

 					   if (ty_Nxt<ttau_Inv) { // Change in Y will occur before a change in Tau					   
	   		   			   	  tnext=ty_Nxt;  
						      nxt='y';};
					   if (ty_Nxt>ttau_Inv) { // Direction change in Tau will occur before a change in Y
	   		   			   	  tnext=ttau_Inv;  
					  	   	  nxt='t';};
					   if (ty_Nxt==ttau_Inv) {   // Change in Y will occur at the same time as Tau					   
	   		   			   	  tnext=ttau_Inv; 	 // DECIDE TO GIVE PRIORITY TO THE TAU CHANGE. THIS DECISSION REQUIRES FURTHER ANALYSIS. !CBH!
					  	   	  nxt='t';};

			         // Adjust Initial Position (Find the target Instant inside target Segment)
		             advance_time(y,ty_Bck,-1);  

				  	     // for QSS2: // y[0]=y[0]-y[1]*Tau[0];  // This is already achieved with the previous adjustment
      					 // for QSS2: y[1]=y[1]-y[1]*Tau[1];

//                       //  (Tau[0]^2 y[2] - Tau[0] y[1] + y[0])  // This is already achieved with the previous adjustment
					 y[1]=(2*Tau[0]*0*Tau[1]*y[2] - 2*Tau[0]*0*y[2] - Tau[1]*y[1] + y[1]);
					 y[2]=(2*Tau[0]*0*Tau[2]*y[2] + Tau[1]*Tau[1]*y[2] - 2*Tau[1]*y[2] + y[2] - Tau[2]*y[1]);
					 y[3]=(Tau[1]*Tau[2]*y[2] - 2*Tau[2]*y[2]);
//					 y[4]=(Tau[2]*Tau[2]*y[2]);
			  }; };
			  
	   		if (zone_Tau=='s') {   // Tau is STATIONARY
		         // ---------- printLog("      	(stationary) \n"); // DEBUG
  	 	             tnext=INF;
			  	   	 nxt='i';
			         // Adjust Initial Position (Find target Instant inside target Segment)
			  		 if  (ty_Bck==INF) {    // Special case: Delay older than oldest segment.
        			 	 advance_time(y,-ty_Fwd,-1);
					 } else {             // No Special Case 
		                 advance_time(y,ty_Bck,-1);};  
					 // Calculates new coefficients
				  	     // y[0]=y[0]-y[1]*Tau[0];  // This is already achieved with the previous adjustment
	 				 y[1]=0;           // Only leaves the constant coeff y[0] present at the output
	 				 y[2]=0; 		   // 
	 				 y[3]=0; 		   //
//				     y[4]=0;			//
			   				  };

	   		if (zone_Tau=='f') {   // Tau is moving FORWARDS 
		         // ---------- printLog("      	(moving forwards) \n"); // DEBUG
			    if (ty_Bck==INF) {           // Special case: Delay older than oldest segment.
		         // ---------- printLog("      	(enter b inf) \n"); // DEBUG

                  double Tau_linDetrend_ty_Nxt[4] ; 
				  Tau_linDetrend_ty_Nxt[0]=ty_Fwd ; 
				  Tau_linDetrend_ty_Nxt[1]=Tau[1]-1 ;  
				  Tau_linDetrend_ty_Nxt[2]=Tau[2] ; // 
				  Tau_linDetrend_ty_Nxt[3]=0 ; // 
				  double ty_Nxt=minposroot(Tau_linDetrend_ty_Nxt,2);	// Find when the next direction change will happend
				  // ---------- printLog("Tau_linDetrend_ty_Nxt: [%g,%g,%g,%g] \n",Tau_linDetrend_ty_Nxt[0],Tau_linDetrend_ty_Nxt[1],Tau_linDetrend_ty_Nxt[2],Tau_linDetrend_ty_Nxt[3]);   // DEBUG 					 
				  // ---------- printLog("ty_Nxt: %g \n",ty_Nxt);   // DEBUG 					 

	  	 			   if (ty_Nxt<ttau_Inv) { // Change in y will occur before a change in Tau					   
	   		   			   	  tnext=ty_Nxt;
					  	   	  nxt='y';};
					   if (ty_Fwd>ttau_Inv) { // Change in Tau will occur before a change in Y
	   		   			   	  tnext=ttau_Inv;  
					  	   	  nxt='t';};
					   if (ty_Fwd==ttau_Inv) { // Change in y will occur at the same time as Tau					   
	   		   			   	  tnext=ttau_Inv; 	 // DECIDE TO GIVE PRIORITY TO THE TAU CHANGE. THIS DECISSION REQUIRES FURTHER ANALYSIS. !CBH!
					  	   	  nxt='t';};

			         // Adjust Initial Position (Find target Instant inside target Segment)
		             advance_time(y,-ty_Fwd,-1); // Extrapolates backwards ("invents" a piece of segment backwards in time).
		             
				  	     // for QSS2: // y[0]=y[0]-y[1]*Tau[0];  // This is already achieved with the previous adjustment
      					 // for QSS2: y[1]=y[1]-y[1]*Tau[1];

//                       //  (Tau[0]^2 y[2] - Tau[0] y[1] + y[0])  // This is already achieved with the previous adjustment
					 y[1]=(2*Tau[0]*0*Tau[1]*y[2] - 2*Tau[0]*0*y[2] - Tau[1]*y[1] + y[1]);
					 y[2]=(2*Tau[0]*0*Tau[2]*y[2] + Tau[1]*Tau[1]*y[2] - 2*Tau[1]*y[2] + y[2] - Tau[2]*y[1]);
					 y[3]=(Tau[1]*Tau[2]*y[2] - 2*Tau[2]*y[2]);
//					 y[4]=(Tau[2]*Tau[2]*y[2]);

			   } else { if (ty_Fwd==INF) {   // Special case: This is the Last Segment in que queue
		                        // ---------- printLog("      	(enter f inf) \n"); // DEBUG
                         if (ttau_Inv==INF) {tnext=INF;  	  // Tau does not change direction
		 			 			          nxt='i'; 
		                        // ---------- printLog("      	(enter 1 inf) \n"); // DEBUG
 					             } else { tnext=ttau_Inv;  		  // Tau will predominate
		                        // ---------- printLog("      	(enter 2 inf with tnext %g) \n",tnext); // DEBUG
 		 			  	                  nxt='t';};		   
			         // Adjust Initial Position (Find target Instant inside target Segment)
		             advance_time(y,ty_Bck,-1);  

				  	     // for QSS2: // y[0]=y[0]-y[1]*Tau[0];  // This is already achieved with the previous adjustment
      					 // for QSS2: y[1]=y[1]-y[1]*Tau[1];

//                       //  (Tau[0]^2 y[2] - Tau[0] y[1] + y[0])  // This is already achieved with the previous adjustment
					 y[1]=(2*Tau[0]*0*Tau[1]*y[2] - 2*Tau[0]*0*y[2] - Tau[1]*y[1] + y[1]);
					 y[2]=(2*Tau[0]*0*Tau[2]*y[2] + Tau[1]*Tau[1]*y[2] - 2*Tau[1]*y[2] + y[2] - Tau[2]*y[1]);
					 y[3]=(Tau[1]*Tau[2]*y[2] - 2*Tau[2]*y[2]);
//					 y[4]=(Tau[2]*Tau[2]*y[2]);

			   } else { 	                // No special case
		         // ---------- printLog("      	(enter normal) \n"); // DEBUG

                  double Tau_linDetrend_ty_Nxt[4] ; 
				  Tau_linDetrend_ty_Nxt[0]=ty_Fwd; 
				  Tau_linDetrend_ty_Nxt[1]=Tau[1]-1 ;  
				  Tau_linDetrend_ty_Nxt[2]=Tau[2] ; // 
				  Tau_linDetrend_ty_Nxt[3]=0 ; // 
				  double ty_Nxt=minposroot(Tau_linDetrend_ty_Nxt,2);	// Find when the next direction change will happend
				  // ---------- printLog("Tau_linDetrend_ty_Nxt: [%g,%g,%g,%g] \n",Tau_linDetrend_ty_Nxt[0],Tau_linDetrend_ty_Nxt[1],Tau_linDetrend_ty_Nxt[2],Tau_linDetrend_ty_Nxt[3]);   // DEBUG 					 
				  // ---------- printLog("ty_Nxt: %g \n",ty_Nxt);   // DEBUG 					 

					   if (ty_Nxt<ttau_Inv) { // Change in y will occur before a change in Tau					   
	   		   			   	  tnext=ty_Nxt; 
					  	   	  nxt='y';};
					   if (ty_Nxt>ttau_Inv) { // Change in Tau will occur before a change in Y
	   		   			   	  tnext=ttau_Inv;  
					  	   	  nxt='t';};
					   if (ty_Nxt==ttau_Inv) { // Change in y will occur at the same time as Tau					   
	   		   			   	  tnext=ttau_Inv;  // DECIDE TO GIVE PRIORITY TO THE TAU CHANGE. THIS DECISSION REQUIRES FURTHER ANALYSIS. !CBH!
					  	   	  nxt='t';};
	  	                                                        // ---------- printLog("Initial Y: [%g,%g,%g,%g] \n",y[0],y[1],y[2],y[3]);     // DEBUG 
	  	                                                        
			         // Adjust Initial Position (Find target Instant inside target Segment)
		             advance_time(y,ty_Bck,-1);  
		             
                                                                // ---------- printLog("Readj.  Y: [%g,%g,%g,%g] \n",y[0],y[1],y[2],y[3]);     // DEBUG 
                                                                
				  	 // for QSS2: // y[0]=y[0]-y[1]*Tau[0];  // This is already achieved with the previous adjustment
      				 // for QSS2: y[1]=y[1]-y[1]*Tau[1];

//                       //  (Tau[0]^2 y[2] - Tau[0] y[1] + y[0])  // This is already achieved with the previous adjustment
					 y[1]=(2*Tau[0]*0*Tau[1]*y[2] - 2*Tau[0]*0*y[2] - Tau[1]*y[1] + y[1]);
					 y[2]=(2*Tau[0]*0*Tau[2]*y[2] + Tau[1]*Tau[1]*y[2] - 2*Tau[1]*y[2] + y[2] - Tau[2]*y[1]);
					 y[3]=(Tau[1]*Tau[2]*y[2] - 2*Tau[2]*y[2]);
//					 y[4]=(Tau[2]*Tau[2]*y[2]);
	                                              	 			// ---------- printLog("Readj.Slope: [%g,%g,%g,%g] \n",y[0],y[1],y[2],y[3]);   // DEBUG 					 };	}; 
 											  	  		   		// ---------- printLog("with Tau[1]: %g \n",Tau[1]);     // DEBUG 					 
			   		   };	};   };
 	   		break;

	   case 3:	// Tau is HYPERBOLIC segments
	   		  	// THIS ONLY HAPPENS in QSS4 or HIGHER. NOT IMPLEMENTED YET.
                // ---------- printLog("      	Refreshing for HYPERBOLIC Tau \n"); // DEBUG
	   		break;
   	  };
return ;
};








public:
	dqssv2o3(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
