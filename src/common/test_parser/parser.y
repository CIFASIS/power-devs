/****************************************************************************
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

%name PdmParser
%define STYPE pdm_parser_val

%header{
#define YY_PdmParser_LVAL parserlval
#include <iostream>
#include <fstream>

int yylex();
void yyerror(char *);
#include <string>
#include <stack>

using namespace std;

#include <data/atomic.h>
#include <data/parameter.h>
#include <data/graphic.h>
#include <data/coupled.h>

typedef struct 
{
	int i;
	string s;
	double d;
  StringParameter *sp;
  ValueParameter *vp;
  ListParameter *lpar;
	Atomic *a;
	vector<Parameter*> lp;
  vector<Port> lport;
  vector<string> lstr;
  Port *port;
	Graphic g;
	Graphic::Direction gd;
	Coupled *c;
} pdm_parser_val;

%}

%{
#include <data/model.h>
#include <data/atomic.h>
using namespace std;
extern Coupled *current_coupled;
stack<Coupled*> coupled_stack;
%}

%token TOKCOUPLED 					
%token TOKROOT  					
%token TOKOBRACE  			
%token TOKCBRACE  		
%token TOKTYPE    	
%token TOKNAME    
%token TOKPORTS  
%token TOKGRAPHIC
%token TOKPOINTS
%token TOKCHILD
%token TOKDESCRIPTION 		
%token TOKPARAMETERS  	
%token TOKSYSTEM      
%token TOKATOMIC		
%token TOKPATH	 	
%token TOKPOINT	
%token TOKLINE
%token TOKINPORT  				
%token TOKOUTPORT				
%token TOKCOUPLEDPORT	 
%token TOKCONNECTEDLINES
%token TOKCONECTEDEXTREMS 
%token TOKPOSITION 		
%token TOKSOURCE 				
%token TOKSINK 			
%token TOKPOINTX 	
%token TOKPOINTY 
%token TOKVAL 	
%token TOKSTR	 
%token TOKLST	
%token TOKCOLON	
%token TOKPERC
%token TOKEQUAL	
%token TOKPNT	
%token TOKCMP
%token TOKPRT					
%token TOKNOC				
%token TOKFROM
%token TOKTO
%token TOKEXTERNAL
%token TOKLABEL
%token TOKOF
%token TOKINTERNAL
%token TOKARROW
%token TOKNONE		
%token TOKDST		
%token TOKORG	
%token TOKROOTCOORD		
%token TOKSIMULATOR 
%token TOKCOORDINATOR 
%token TOKEIC 			
%token TOKEOC 		
%token TOKCOMMA
%token TOKIC 		
%token TOKOPAR
%token TOKCPAR	
%token TOKDIMENSION		
%token TOKDIRECTION	
%token TOKCOLOR		
%token TOKICON	
%token TOKWINDOW
%token TOKLEFT	
%token TOKRIGHT	
%token TOKUP		
%token TOKDOWN	
%token TOKLINES

%token <s>TOKQUOTEDSTR
%token <i>TOKINTEGER
%token <d>TOKDOUBLE

%type <a> atomic
%type <c> coupled
%type <lp> params_list parameters
%type <sp> param_string 
%type <vp> param_value
%type <lpar> param_string_list 
%type <g> atomic_graphics coupled_graphics
%type <gd> direction
%type <port> port
%type <lport> port_list
%type <i> in_out
%type <lstr> string_list
%%
input: coupled 

coupled: { coupled_stack.push(current_coupled); 
           current_coupled=new Coupled(); } 
				TOKCOUPLED
					TOKOBRACE
						TOKNAME TOKEQUAL TOKQUOTEDSTR
						TOKPORTS TOKEQUAL TOKINTEGER TOKCOLON TOKINTEGER
						TOKDESCRIPTION TOKEQUAL TOKQUOTEDSTR
						parameters 
						system
						coupled_graphics 
					TOKCBRACE
					{
            current_coupled->setName($6);
            //current_coupled->setDescription($13);
					  current_coupled->setInports($9);
					  current_coupled->setOutports($11); 
						Coupled *father=coupled_stack.top();
            $$ = current_coupled;
						coupled_stack.pop();
						current_coupled->setFather(father);
						if (father!=NULL) 
							current_coupled=father;
					}
			

system:
			TOKSYSTEM
			TOKOBRACE
			child_list
			connections
			TOKCBRACE

line_list:
	| line line_list

line: TOKOBRACE
		TOKFROM TOKEQUAL connection_point
		TOKTO TOKEQUAL connection_point
		TOKLABEL TOKEQUAL TOKQUOTEDSTR
		TOKCOLOR TOKEQUAL TOKINTEGER
		TOKPOINTX TOKEQUAL integer_list
		TOKPOINTY TOKEQUAL integer_list
		TOKCBRACE
	
	
in_out: TOKINPORT { $$ = 0; }
			| TOKOUTPORT { $$ = 1; }
	
connection_point:
			in_out TOKINTEGER TOKOF TOKCHILD TOKINTEGER
		| TOKPOINT TOKINTEGER
		| TOKEXTERNAL in_out TOKINTEGER
		| TOKNOC 	
point_list:
	| point point_list


point: TOKOBRACE
			 TOKCONNECTEDLINES TOKEQUAL integer_list
			 TOKPOSITION TOKEQUAL TOKINTEGER TOKCOLON TOKINTEGER
			 TOKCBRACE

integer_list: TOKINTEGER 
		| integer_list TOKCOLON TOKINTEGER 

child_list:
	|	atomic child_list { current_coupled->addChild($1); }
	| coupled child_list{ current_coupled->addChild($1); }
			
atomic: TOKATOMIC
					TOKOBRACE
						TOKNAME TOKEQUAL TOKQUOTEDSTR
						TOKPORTS TOKEQUAL TOKINTEGER TOKCOLON TOKINTEGER
						TOKPATH TOKEQUAL TOKQUOTEDSTR
						TOKDESCRIPTION TOKEQUAL TOKQUOTEDSTR
						parameters
						atomic_graphics
				TOKCBRACE
				{
					$$ = new Atomic(current_coupled,$5,$16);	
					$$->setInports($8);
					$$->setOutports($10); 
          printf("Path is at %s\n",$13.c_str());
					$$->setPath($13); 
					$$->setGraphics($18); 
					$$->setParameters($17);
				}	

coupled_graphics: { $$ = NoGraphic(); }
		 |	TOKGRAPHIC
				TOKOBRACE
					TOKPOSITION TOKEQUAL TOKINTEGER TOKCOLON TOKINTEGER
					TOKDIMENSION TOKEQUAL TOKINTEGER TOKCOLON TOKINTEGER
					TOKDIRECTION TOKEQUAL direction
					TOKCOLOR TOKEQUAL TOKINTEGER
					TOKICON	TOKEQUAL TOKQUOTEDSTR
						TOKPORTS
						TOKOBRACE
						port_list
						TOKCBRACE

						TOKPOINTS
						TOKOBRACE
						point_list
						TOKCBRACE

						TOKLINES
						TOKOBRACE
						line_list
						TOKCBRACE

				TOKCBRACE { $$ = CoupledGraphic(); }


port_list: {}
	| port port_list  
    {
      if ($1->type()==0)
        current_coupled->addInport($1);
      else
        current_coupled->addOutport($1);
    }

port: in_out
			TOKOBRACE
			TOKNAME TOKEQUAL TOKQUOTEDSTR
			TOKDESCRIPTION TOKEQUAL TOKQUOTEDSTR
			TOKPOSITION TOKEQUAL TOKINTEGER TOKCOLON TOKINTEGER
			TOKDIMENSION TOKEQUAL TOKINTEGER
			TOKDIRECTION TOKEQUAL direction
			TOKCBRACE
      {
        Graphic g;
        g.setX($11);
        g.setY($13);
        g.setWidth($16);
        g.setDirection($19);
        $$ = new Port($1,$5,$8,g);
      }
	

connections:
						ic
						eic
						eoc

ic:
		TOKIC
		TOKOBRACE
		connection_list
		TOKCBRACE

eic:
		TOKEIC
		TOKOBRACE
		connection_list
		TOKCBRACE

eoc:
		TOKEOC
		TOKOBRACE
		connection_list
		TOKCBRACE

connection: TOKOPAR TOKINTEGER TOKCOMMA TOKINTEGER TOKCPAR TOKARROW TOKOPAR TOKINTEGER TOKCOMMA TOKINTEGER TOKCPAR

connection_list:
	| connection
	| connection_list TOKCOMMA connection
atomic_graphics: { $$ = NoGraphic(); }
		|	TOKGRAPHIC
				TOKOBRACE
					TOKPOSITION TOKEQUAL TOKINTEGER TOKCOLON TOKINTEGER
					TOKDIMENSION TOKEQUAL TOKINTEGER TOKCOLON TOKINTEGER
					TOKDIRECTION TOKEQUAL direction
					TOKCOLOR TOKEQUAL TOKINTEGER
					TOKICON	TOKEQUAL TOKQUOTEDSTR
				TOKCBRACE 
				{
					$$ = AtomicGraphic();
					$$.setX($5);
					$$.setY($7);
					$$.setWidth($10);
					$$.setHeight($12);
					$$.setDirection($15);
					$$.setColor($18);
					$$.setIcon($21);
				}

direction: TOKUP { $$ = Graphic::UP; }  | TOKDOWN  { $$ = Graphic::DOWN; } | TOKLEFT { $$ = Graphic::LEFT; } | TOKRIGHT { $$ = Graphic::RIGHT; }

parameters: 
					TOKPARAMETERS
					TOKOBRACE
					params_list
					TOKCBRACE
					{ $$ = $3; }

params_list: { $$ = vector<Parameter*>(); }
				| params_list param_string  { $1.push_back($2);$$ = $1 ; }
				| params_list param_value  { $1.push_back($2); $$=$1; }
				| params_list param_string_list { $1.push_back($2); $$=$1;}

param_string: 
		TOKQUOTEDSTR TOKEQUAL TOKSTR TOKCOLON TOKQUOTEDSTR TOKCOLON TOKQUOTEDSTR { $$ = new StringParameter($1,$7,$5); }
param_value: 
		TOKQUOTEDSTR TOKEQUAL TOKVAL TOKCOLON TOKDOUBLE TOKCOLON TOKQUOTEDSTR  { $$ = new ValueParameter($1,$7,$5); }
param_string_list: 
		TOKQUOTEDSTR TOKEQUAL TOKLST TOKCOLON TOKINTEGER TOKCOLON TOKOBRACE string_list TOKCBRACE TOKCOLON TOKQUOTEDSTR { $$ = new ListParameter($1,$11,$8,$5);
      cout << "List size:" << $8.size()<< endl;
    
    }

string_list: { $$ = vector<string>(); }
		| TOKQUOTEDSTR { vector<string> v; v.push_back($1); $$=v; cout<< " pushing " << $1<<endl;}
		| string_list TOKCOMMA TOKQUOTEDSTR  { $1.push_back($3); $$ = $1; cout << "pushing 2" << $3<<endl;}
%%


