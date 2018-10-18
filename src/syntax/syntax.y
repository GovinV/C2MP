%{
	#include <stdio.h>
	int yylex();
	void yyerror(char*);

  FILE *yyin, *yyout;
%}


%token ENTIER
%left '+' 
%left '*'
%%

axiom: E '\n' {printf("val = %d\n",$1);}
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
  ;


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
  return 0;
}
