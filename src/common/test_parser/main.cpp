#include "parser.h"
#include "atomic.h"
#include "coupled.h"
using namespace std;

yyFlexLexer l;
void yyerror(char *s)
{
	printf("%s\n",s);
}

int yylex()
{
	return l.yylex();
};
int yyparse();
Coupled *current_coupled;
int main(int argc, char ** argv)
{
	current_coupled=NULL;
	yyparse();
	return 0;
}

