#ifndef YY_PdmParser_h_included
#define YY_PdmParser_h_included
/*#define YY_USE_CLASS 
*/
#line 1 "/usr/share/bison++/bison.h"
/* before anything */
#ifdef c_plusplus
 #ifndef __cplusplus
  #define __cplusplus
 #endif
#endif


 #line 8 "/usr/share/bison++/bison.h"
#define YY_PdmParser_STYPE  pdm_parser_val
#line 27 "parser.y"

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
  Port port;
	Graphic g;
	Graphic::Direction gd;
	Coupled *c;
} pdm_parser_val;


#line 21 "/usr/share/bison++/bison.h"
 /* %{ and %header{ and %union, during decl */
#ifndef YY_PdmParser_COMPATIBILITY
 #ifndef YY_USE_CLASS
  #define  YY_PdmParser_COMPATIBILITY 1
 #else
  #define  YY_PdmParser_COMPATIBILITY 0
 #endif
#endif

#if YY_PdmParser_COMPATIBILITY != 0
/* backward compatibility */
 #ifdef YYLTYPE
  #ifndef YY_PdmParser_LTYPE
   #define YY_PdmParser_LTYPE YYLTYPE
/* WARNING obsolete !!! user defined YYLTYPE not reported into generated header */
/* use %define LTYPE */
  #endif
 #endif
/*#ifdef YYSTYPE*/
  #ifndef YY_PdmParser_STYPE
   #define YY_PdmParser_STYPE YYSTYPE
  /* WARNING obsolete !!! user defined YYSTYPE not reported into generated header */
   /* use %define STYPE */
  #endif
/*#endif*/
 #ifdef YYDEBUG
  #ifndef YY_PdmParser_DEBUG
   #define  YY_PdmParser_DEBUG YYDEBUG
   /* WARNING obsolete !!! user defined YYDEBUG not reported into generated header */
   /* use %define DEBUG */
  #endif
 #endif 
 /* use goto to be compatible */
 #ifndef YY_PdmParser_USE_GOTO
  #define YY_PdmParser_USE_GOTO 1
 #endif
#endif

/* use no goto to be clean in C++ */
#ifndef YY_PdmParser_USE_GOTO
 #define YY_PdmParser_USE_GOTO 0
#endif

#ifndef YY_PdmParser_PURE

 #line 65 "/usr/share/bison++/bison.h"

#line 65 "/usr/share/bison++/bison.h"
/* YY_PdmParser_PURE */
#endif


 #line 68 "/usr/share/bison++/bison.h"

#line 68 "/usr/share/bison++/bison.h"
/* prefix */

#ifndef YY_PdmParser_DEBUG

 #line 71 "/usr/share/bison++/bison.h"
#define YY_PdmParser_DEBUG 1

#line 71 "/usr/share/bison++/bison.h"
/* YY_PdmParser_DEBUG */
#endif

#ifndef YY_PdmParser_LSP_NEEDED

 #line 75 "/usr/share/bison++/bison.h"

#line 75 "/usr/share/bison++/bison.h"
 /* YY_PdmParser_LSP_NEEDED*/
#endif

/* DEFAULT LTYPE*/
#ifdef YY_PdmParser_LSP_NEEDED
 #ifndef YY_PdmParser_LTYPE
  #ifndef BISON_YYLTYPE_ISDECLARED
   #define BISON_YYLTYPE_ISDECLARED
typedef
  struct yyltype
    {
      int timestamp;
      int first_line;
      int first_column;
      int last_line;
      int last_column;
      char *text;
   }
  yyltype;
  #endif

  #define YY_PdmParser_LTYPE yyltype
 #endif
#endif

/* DEFAULT STYPE*/
#ifndef YY_PdmParser_STYPE
 #define YY_PdmParser_STYPE int
#endif

/* DEFAULT MISCELANEOUS */
#ifndef YY_PdmParser_PARSE
 #define YY_PdmParser_PARSE yyparse
#endif

#ifndef YY_PdmParser_LEX
 #define YY_PdmParser_LEX yylex
#endif

#ifndef YY_PdmParser_LVAL
 #define YY_PdmParser_LVAL yylval
#endif

#ifndef YY_PdmParser_LLOC
 #define YY_PdmParser_LLOC yylloc
#endif

#ifndef YY_PdmParser_CHAR
 #define YY_PdmParser_CHAR yychar
#endif

#ifndef YY_PdmParser_NERRS
 #define YY_PdmParser_NERRS yynerrs
#endif

#ifndef YY_PdmParser_DEBUG_FLAG
 #define YY_PdmParser_DEBUG_FLAG yydebug
#endif

#ifndef YY_PdmParser_ERROR
 #define YY_PdmParser_ERROR yyerror
#endif

#ifndef YY_PdmParser_PARSE_PARAM
 #ifndef __STDC__
  #ifndef __cplusplus
   #ifndef YY_USE_CLASS
    #define YY_PdmParser_PARSE_PARAM
    #ifndef YY_PdmParser_PARSE_PARAM_DEF
     #define YY_PdmParser_PARSE_PARAM_DEF
    #endif
   #endif
  #endif
 #endif
 #ifndef YY_PdmParser_PARSE_PARAM
  #define YY_PdmParser_PARSE_PARAM void
 #endif
#endif

/* TOKEN C */
#ifndef YY_USE_CLASS

 #ifndef YY_PdmParser_PURE
  #ifndef yylval
   extern YY_PdmParser_STYPE YY_PdmParser_LVAL;
  #else
   #if yylval != YY_PdmParser_LVAL
    extern YY_PdmParser_STYPE YY_PdmParser_LVAL;
   #else
    #warning "Namespace conflict, disabling some functionality (bison++ only)"
   #endif
  #endif
 #endif


 #line 169 "/usr/share/bison++/bison.h"
#define	TOKCOUPLED	258
#define	TOKROOT	259
#define	TOKOBRACE	260
#define	TOKCBRACE	261
#define	TOKTYPE	262
#define	TOKNAME	263
#define	TOKPORTS	264
#define	TOKGRAPHIC	265
#define	TOKPOINTS	266
#define	TOKCHILD	267
#define	TOKDESCRIPTION	268
#define	TOKPARAMETERS	269
#define	TOKSYSTEM	270
#define	TOKATOMIC	271
#define	TOKPATH	272
#define	TOKPOINT	273
#define	TOKLINE	274
#define	TOKINPORT	275
#define	TOKOUTPORT	276
#define	TOKCOUPLEDPORT	277
#define	TOKCONNECTEDLINES	278
#define	TOKCONECTEDEXTREMS	279
#define	TOKPOSITION	280
#define	TOKSOURCE	281
#define	TOKSINK	282
#define	TOKPOINTX	283
#define	TOKPOINTY	284
#define	TOKVAL	285
#define	TOKSTR	286
#define	TOKLST	287
#define	TOKCOLON	288
#define	TOKPERC	289
#define	TOKEQUAL	290
#define	TOKPNT	291
#define	TOKCMP	292
#define	TOKPRT	293
#define	TOKNOC	294
#define	TOKFROM	295
#define	TOKTO	296
#define	TOKEXTERNAL	297
#define	TOKLABEL	298
#define	TOKOF	299
#define	TOKINTERNAL	300
#define	TOKARROW	301
#define	TOKNONE	302
#define	TOKDST	303
#define	TOKORG	304
#define	TOKROOTCOORD	305
#define	TOKSIMULATOR	306
#define	TOKCOORDINATOR	307
#define	TOKEIC	308
#define	TOKEOC	309
#define	TOKCOMMA	310
#define	TOKIC	311
#define	TOKOPAR	312
#define	TOKCPAR	313
#define	TOKDIMENSION	314
#define	TOKDIRECTION	315
#define	TOKCOLOR	316
#define	TOKICON	317
#define	TOKWINDOW	318
#define	TOKLEFT	319
#define	TOKRIGHT	320
#define	TOKUP	321
#define	TOKDOWN	322
#define	TOKLINES	323
#define	TOKQUOTEDSTR	324
#define	TOKINTEGER	325
#define	TOKDOUBLE	326


#line 169 "/usr/share/bison++/bison.h"
 /* #defines token */
/* after #define tokens, before const tokens S5*/
#else
 #ifndef YY_PdmParser_CLASS
  #define YY_PdmParser_CLASS PdmParser
 #endif

 #ifndef YY_PdmParser_INHERIT
  #define YY_PdmParser_INHERIT
 #endif

 #ifndef YY_PdmParser_MEMBERS
  #define YY_PdmParser_MEMBERS 
 #endif

 #ifndef YY_PdmParser_LEX_BODY
  #define YY_PdmParser_LEX_BODY  
 #endif

 #ifndef YY_PdmParser_ERROR_BODY
  #define YY_PdmParser_ERROR_BODY  
 #endif

 #ifndef YY_PdmParser_CONSTRUCTOR_PARAM
  #define YY_PdmParser_CONSTRUCTOR_PARAM
 #endif
 /* choose between enum and const */
 #ifndef YY_PdmParser_USE_CONST_TOKEN
  #define YY_PdmParser_USE_CONST_TOKEN 0
  /* yes enum is more compatible with flex,  */
  /* so by default we use it */ 
 #endif
 #if YY_PdmParser_USE_CONST_TOKEN != 0
  #ifndef YY_PdmParser_ENUM_TOKEN
   #define YY_PdmParser_ENUM_TOKEN yy_PdmParser_enum_token
  #endif
 #endif

class YY_PdmParser_CLASS YY_PdmParser_INHERIT
{
public: 
 #if YY_PdmParser_USE_CONST_TOKEN != 0
  /* static const int token ... */
  
 #line 212 "/usr/share/bison++/bison.h"
static const int TOKCOUPLED;
static const int TOKROOT;
static const int TOKOBRACE;
static const int TOKCBRACE;
static const int TOKTYPE;
static const int TOKNAME;
static const int TOKPORTS;
static const int TOKGRAPHIC;
static const int TOKPOINTS;
static const int TOKCHILD;
static const int TOKDESCRIPTION;
static const int TOKPARAMETERS;
static const int TOKSYSTEM;
static const int TOKATOMIC;
static const int TOKPATH;
static const int TOKPOINT;
static const int TOKLINE;
static const int TOKINPORT;
static const int TOKOUTPORT;
static const int TOKCOUPLEDPORT;
static const int TOKCONNECTEDLINES;
static const int TOKCONECTEDEXTREMS;
static const int TOKPOSITION;
static const int TOKSOURCE;
static const int TOKSINK;
static const int TOKPOINTX;
static const int TOKPOINTY;
static const int TOKVAL;
static const int TOKSTR;
static const int TOKLST;
static const int TOKCOLON;
static const int TOKPERC;
static const int TOKEQUAL;
static const int TOKPNT;
static const int TOKCMP;
static const int TOKPRT;
static const int TOKNOC;
static const int TOKFROM;
static const int TOKTO;
static const int TOKEXTERNAL;
static const int TOKLABEL;
static const int TOKOF;
static const int TOKINTERNAL;
static const int TOKARROW;
static const int TOKNONE;
static const int TOKDST;
static const int TOKORG;
static const int TOKROOTCOORD;
static const int TOKSIMULATOR;
static const int TOKCOORDINATOR;
static const int TOKEIC;
static const int TOKEOC;
static const int TOKCOMMA;
static const int TOKIC;
static const int TOKOPAR;
static const int TOKCPAR;
static const int TOKDIMENSION;
static const int TOKDIRECTION;
static const int TOKCOLOR;
static const int TOKICON;
static const int TOKWINDOW;
static const int TOKLEFT;
static const int TOKRIGHT;
static const int TOKUP;
static const int TOKDOWN;
static const int TOKLINES;
static const int TOKQUOTEDSTR;
static const int TOKINTEGER;
static const int TOKDOUBLE;


#line 212 "/usr/share/bison++/bison.h"
 /* decl const */
 #else
  enum YY_PdmParser_ENUM_TOKEN { YY_PdmParser_NULL_TOKEN=0
  
 #line 215 "/usr/share/bison++/bison.h"
	,TOKCOUPLED=258
	,TOKROOT=259
	,TOKOBRACE=260
	,TOKCBRACE=261
	,TOKTYPE=262
	,TOKNAME=263
	,TOKPORTS=264
	,TOKGRAPHIC=265
	,TOKPOINTS=266
	,TOKCHILD=267
	,TOKDESCRIPTION=268
	,TOKPARAMETERS=269
	,TOKSYSTEM=270
	,TOKATOMIC=271
	,TOKPATH=272
	,TOKPOINT=273
	,TOKLINE=274
	,TOKINPORT=275
	,TOKOUTPORT=276
	,TOKCOUPLEDPORT=277
	,TOKCONNECTEDLINES=278
	,TOKCONECTEDEXTREMS=279
	,TOKPOSITION=280
	,TOKSOURCE=281
	,TOKSINK=282
	,TOKPOINTX=283
	,TOKPOINTY=284
	,TOKVAL=285
	,TOKSTR=286
	,TOKLST=287
	,TOKCOLON=288
	,TOKPERC=289
	,TOKEQUAL=290
	,TOKPNT=291
	,TOKCMP=292
	,TOKPRT=293
	,TOKNOC=294
	,TOKFROM=295
	,TOKTO=296
	,TOKEXTERNAL=297
	,TOKLABEL=298
	,TOKOF=299
	,TOKINTERNAL=300
	,TOKARROW=301
	,TOKNONE=302
	,TOKDST=303
	,TOKORG=304
	,TOKROOTCOORD=305
	,TOKSIMULATOR=306
	,TOKCOORDINATOR=307
	,TOKEIC=308
	,TOKEOC=309
	,TOKCOMMA=310
	,TOKIC=311
	,TOKOPAR=312
	,TOKCPAR=313
	,TOKDIMENSION=314
	,TOKDIRECTION=315
	,TOKCOLOR=316
	,TOKICON=317
	,TOKWINDOW=318
	,TOKLEFT=319
	,TOKRIGHT=320
	,TOKUP=321
	,TOKDOWN=322
	,TOKLINES=323
	,TOKQUOTEDSTR=324
	,TOKINTEGER=325
	,TOKDOUBLE=326


#line 215 "/usr/share/bison++/bison.h"
 /* enum token */
     }; /* end of enum declaration */
 #endif
public:
 int YY_PdmParser_PARSE(YY_PdmParser_PARSE_PARAM);
 virtual void YY_PdmParser_ERROR(char *msg) YY_PdmParser_ERROR_BODY;
 #ifdef YY_PdmParser_PURE
  #ifdef YY_PdmParser_LSP_NEEDED
   virtual int  YY_PdmParser_LEX(YY_PdmParser_STYPE *YY_PdmParser_LVAL,YY_PdmParser_LTYPE *YY_PdmParser_LLOC) YY_PdmParser_LEX_BODY;
  #else
   virtual int  YY_PdmParser_LEX(YY_PdmParser_STYPE *YY_PdmParser_LVAL) YY_PdmParser_LEX_BODY;
  #endif
 #else
  virtual int YY_PdmParser_LEX() YY_PdmParser_LEX_BODY;
  YY_PdmParser_STYPE YY_PdmParser_LVAL;
  #ifdef YY_PdmParser_LSP_NEEDED
   YY_PdmParser_LTYPE YY_PdmParser_LLOC;
  #endif
  int YY_PdmParser_NERRS;
  int YY_PdmParser_CHAR;
 #endif
 #if YY_PdmParser_DEBUG != 0
  public:
   int YY_PdmParser_DEBUG_FLAG;	/*  nonzero means print parse trace	*/
 #endif
public:
 YY_PdmParser_CLASS(YY_PdmParser_CONSTRUCTOR_PARAM);
public:
 YY_PdmParser_MEMBERS 
};
/* other declare folow */
#endif


#if YY_PdmParser_COMPATIBILITY != 0
 /* backward compatibility */
 /* Removed due to bison problems
 /#ifndef YYSTYPE
 / #define YYSTYPE YY_PdmParser_STYPE
 /#endif*/

 #ifndef YYLTYPE
  #define YYLTYPE YY_PdmParser_LTYPE
 #endif
 #ifndef YYDEBUG
  #ifdef YY_PdmParser_DEBUG 
   #define YYDEBUG YY_PdmParser_DEBUG
  #endif
 #endif

#endif
/* END */

 #line 267 "/usr/share/bison++/bison.h"
#endif
