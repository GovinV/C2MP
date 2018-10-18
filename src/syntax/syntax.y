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

/*ROOT: DECLARATION
	| ROOT SEPARATION
	;
LINEFEED: '\n'
	| '\n' PRECOMPILER;
SEPARATION: SPACES SEPARATIONDONE
	| LINEFEED SEPARATIONDONE
	;
SEPARATIONDONE: SEPARATION
	|
	;
DECLARATION: FUNCTION // struct etc
	;
PRECOMPILER: '#' PRECCONTENT '\n'
	;
PRECCONTENT: LETTERORNUMBER
	| SPACES LETTERORNUMBER
	| LETTERORNUMBER SPACES
	|
	| PRECCONTENT '\\' '\n' PRECCONTENT
	;
FUNCTION: '$'
	;*/
ROOT: DECLARATION
	| ROOT SEPARATION
	;
SEPARATION: SEPARATIONDONE ' '
	| SEPARATIONDONE '\n'
	;
SEPARATIONDONE: ' ' SEPARATION
	|
	;
DECLARATION: FUNCTION // struct etc
	;
FUNCTION: '$'
	;

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
