%{
	#include <stdio.h>
	int yylex();
	void yyerror(char*);

	FILE *yyin, *yyout;
	int commentNum = 0;
%}


%token TYPE
%token ENTIER
%token COMMENT
%token NEWLINE
%token SPACES
%left '+'
%left '*'
%%

ROOT : 'a' ROOT
	| '\n' ROOT
	| COMMENT ROOT {++commentNum;printf("Commmentaire trouve : %c\n", $1);}
	|;

/*ROOT: SEPARATION DECLARATIONS
	| DECLARATIONS
	;
DECLARATIONS: DECLARATION SEPARATION DECLARATIONS
	| DECLARATION DECLARATIONS
	| DECLARATION
	;
LINEFEED: NEWLINE
	| NEWLINE PRECOMPILER;
SEPARATION: ' ' SEPARATIONDONE
	| LINEFEED SEPARATIONDONE
	;
SEPARATIONDONE: ' ' SEPARATIONDONE
	| LINEFEED SEPARATIONDONE
	|
	;
DECLARATION: FUNCTION // struct etc
	;
LETTERORNUMBER: 'a'
	;
PRECOMPILER: '#' PRECCONTENT '\n'
	;
PRECCONTENT: PRECOMPILERLINE
	| PRECOMPILERLINE '\\' '\n' PRECCONTENT
	;
PRECOMPILERLINE: LETTERORNUMBER
	| ' ' LETTERORNUMBER
	| LETTERORNUMBER ' '
	|
	;
FUNCTION: 'a'
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
	printf("End of parsing\n");
	printf("Found %d comments\n", commentNum);
	return 0;
}
