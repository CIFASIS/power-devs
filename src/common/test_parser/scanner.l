/***************************************************************************E
**
**  Copyright (C) 2009 Facultad de Ciencia Exactas Ingeniería y Agrimensura
**										 Universidad Nacional de Rosario - Argentina.
**  Contact: PowerDEVS Information (kofman@fceia.unr.edu.ar, fbergero@fceia.unr.edu.ar)
**
**  This file is part of PowerDEVS.
**
**  PowerDEVS is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  PowerDEVS is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with PowerDEVS.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/


%option c++
%option noyywrap
%option caseless
%{
#include <string>
#ifndef TEST
#include <parser_yacc.h>
#else
#include <parser.h>
#endif
char string_buf[1024];
char *string_buf_ptr;
%}

%option debug

%x str

%%
 
[0-9]+\.[0-9]+ { parserlval.d=atof(yytext); ; return TOKDOUBLE;}
[0-9]+				 { parserlval.i=atoi(yytext); ; return  TOKINTEGER;}
Root 				 ; return TOKROOT; 
Coupled 		 ; return TOKCOUPLED; 
Type 				 ; return TOKTYPE; 
Name				 ; return TOKNAME ;    						
Ports 			 ; return TOKPORTS   ; 					
Graphic			 ; return TOKGRAPHIC   ; 				
Description  ; return TOKDESCRIPTION 	; 		
Parameters	 ; return TOKPARAMETERS ; 
System  ; return TOKSYSTEM   ;    			
Atomic ; return TOKATOMIC	; 					
Path  ; return TOKPATH	 ; 
Line  ; return TOKLINE		;  	
Inport  ; return TOKINPORT ; 
Outport  ; return TOKOUTPORT		; 				
CoupledPort  ; return TOKCOUPLEDPORT	 ; 			
ConnectedLines  ; return TOKCONNECTEDLINES;
Position  ; return TOKPOSITION; 
Source  ; return TOKSOURCE 			; 	
Lines ; return TOKLINES;
Sink  ; return TOKSINK 							; 
PointX  ; return TOKPOINTX 					; 
PointY  ; return TOKPOINTY; 
Val  ; return TOKVAL 		; 
Str  ; return TOKSTR	 				; 		
List  ; return TOKLST	 	; 
Points  ; return TOKPOINTS		; 	
Model  ; return TOKCMP		; 
Port	 ; return TOKPRT	; 
NoConnection  ; return TOKNOC	; 			
Dest	 ; return TOKDST		; 
Org		 ; return TOKORG	; 
Root-Coordinator  ; return TOKROOTCOORD			; 
Simulator  ; return TOKSIMULATOR 		; 
Coordinator  ; return TOKCOORDINATOR 		; 
EIC  ; return TOKEIC ; 
EOC  ; return TOKEOC 	; 
IC	 ; return TOKIC 				; 
Dimension  ; return TOKDIMENSION  ; 				
Direction  ; return TOKDIRECTION			; 		
Color  ; return TOKCOLOR ; 
Icon  ; return TOKICON		; 
Window  ; return TOKWINDOW;  
Left  ; return TOKLEFT		; 	
Right  ; return TOKRIGHT		; 	
Up  ; return TOKUP						; 	
Down   ; return TOKDOWN; 
From 	; return TOKFROM;
To 	; return TOKTO;
Label ; return TOKLABEL;
External ; return TOKEXTERNAL;
Child ; return TOKCHILD;
Point ; return TOKPOINT;
Of ; return TOKOF;

"{"  					 ; return TOKOBRACE; 
"}" 					 ; return TOKCBRACE;
;	 ; return TOKCOLON		; 
"="	 ; return TOKEQUAL	; 
"("	 ; return TOKOPAR			; 
")"  ; return TOKCPAR					; 
-> 	; return TOKARROW;
,		; return TOKCOMMA;
\"      string_buf_ptr = string_buf; BEGIN(str);
\/\/([^\n])*\n 
[\n\t\ ]	 ;

<str>\" { /* saw closing quote - all done */
        BEGIN(INITIAL);
        *string_buf_ptr = '\0';
				 parserlval.s = string( string_buf);
				 ; return TOKQUOTEDSTR;
        }

<str>\n {
        /* error - unterminated string constant */
        /* generate error message */
        }

<str>\\[0-7]{1,3} {
        /* octal escape sequence */
        int result;

        (void) sscanf( yytext + 1, "%o", &result );

        if ( result > 0xff )
                /* error, constant is out-of-bounds */

        *string_buf_ptr++ = result;
        }

<str>\\[0-9]+ {
        /* generate error - bad escape sequence; something
         * like '\48' or '\0777777'
         */
        }

<str>\\n  *string_buf_ptr++ = '\n';
<str>\\t  *string_buf_ptr++ = '\t';
<str>\\r  *string_buf_ptr++ = '\r';
<str>\\b  *string_buf_ptr++ = '\b';
<str>\\f  *string_buf_ptr++ = '\f';

<str>\\(.|\n)  *string_buf_ptr++ = yytext[1];

<str>[^\\\n\"]+        {
        char *yptr = yytext;

        while ( *yptr )
                *string_buf_ptr++ = *yptr++;
        }



%%
