%{
	#include <stdio.h>
	int yylex();
	void yyerror(char*);
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

int main()
{
	printf("hello\n");
	yyparse();
	return 0;
}
