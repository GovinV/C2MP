%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "utils.h"
    int yylex();
    void yyerror(char*);
    FILE *yyin, *yyout;
%}

%union {
    
    char *string;
    int value;
    float fvalue;

    union 
    {
      int valueInt;
      float valueFloat;
    } nvalue;

    struct
    {
        const char * name;
        int ref;
        union{
            int valueInt;
            float valueFloat;
        };
    } vars;

}

%token <value>      INTEGER
%token <fvalue>     FLOAT
%token <string>     PRAGMA
%token <string>     SYMBOL

%type <nvalue>      EXPR
%type <nvalue>      NUMBER
%type <vars>        VAR

%left '*' '/'
%left '+' '-'

%%

Axiom: AFF;

AFF: 
      VAR '=' EXPR ';'   { printf("VAR = EXPR\n"); }
    | '(' VAR '=' EXPR ')'
    ;

VAR: 
      SYMBOL        { printf("VAR %s\n", $1); }
    ;

NUMBER: 
      INTEGER       { printf("NUMBER = INTEGER = %d\n", $1); }
    | FLOAT         { printf("NUMBER = FLOAT = %f\n", $1); }
    ;

EXPR: 
      EXPR '+' EXPR { printf("EXPR + EXPR\n"); }
    | EXPR '*' EXPR { printf("EXPR * EXPR\n"); }
    | EXPR '-' EXPR { printf("EXPR - EXPR\n"); }
    | EXPR '/' EXPR { printf("EXPR / EXPR\n"); }
    | '(' EXPR ')'  { printf("(EXPR)\n"); }
    | SYMBOL        { printf("EXPR = SYMBOL %s\n", $1); }
    | NUMBER        { printf("EXPR = NUMBER\n"); }
    ;

FCT:
      SYMBOL '(' EXPR ARG ')'
    ;

ARG:
      ',' EXPR ARG
    | 
    ;

%%

int main(int argc, char *argv[])
{
  if(argc != 2)
 	{
        fprintf(stderr, "Erreur arg manquant\n");
 		return 1;
 	}
 
	yyin = fopen(argv[1], "r");
	yyout = stdout;
	yyparse();
	printf("End of parsing\n");
	return 0;
}
