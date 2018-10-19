%{
	#include <stdio.h>
	int yylex();
	void yyerror(char*);

  FILE *yyin, *yyout;
%}


%token TYPE
%token ENTIER
%token LETTERORNUMBER
%token SPACES
%left '+' 
%left '*'
%%

RULE: '$';

/*ROOT: DECLARATION ROOT
	|
	;
LINEFEED: '\n'
	| '\n' PRECOMPILER;
SEPARATION: ' ' SEPARATIONDONE
	| LINEFEED SEPARATIONDONE
	;
SEPARATIONDONE: ' ' SEPARATIONDONE
	| LINEFEED SEPARATIONDONE
	|
	;
DECLARATION: FUNCTION // struct etc
	;
PRECOMPILER: '#' PRECCONTENT '\n'
	;
PRECCONTENT: PRECOMPILERLINE
	| PRECOMPILERLINE '\\' '\n' PRECCONTENT
	;
PRECOMPILERLINE: LETTERORNUMBER
	| SPACES LETTERORNUMBER
	| LETTERORNUMBER SPACES
	|
	;
FUNCTION: '$'
	;*/

/*axiom: E '\n' {printf("val = %d\n",$1);}
     ;

E : E '+' E   {printf("E: E + E\n");
               $$ = $1 + $3;
              }  
  | E '*' E   {printf("E: E * E\n");
	       $$ = $1 * $3;
              }
  | '(' E ')' {printf("E: (E)\n");
               $$ = $2; 
              }
  | ENTIER    {printf("E: ENTIER (%d)\n",$1);
               $$ = $1;
	      }
  ;*/


%%

int main(int argc, char *argv[])
{
  if(argc != 2)
  {
    return 1;
  }

  yyin = fopen(argv[1], "r");
  yyout = stdout;
  yyparse();
  printf("c bon\n");
  return 0;
}
