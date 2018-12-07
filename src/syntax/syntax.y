%{
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include "utils.h"
	int yylex();
	void yyerror(char*);
 
	FILE *yyin, *yyout;
	int commentNum = 0;
%}

%union 
{
	char  * string;
	int     value;
	float   fvalue;
	
	struct  pragmaExt 
	{
		char * ext;
		union 
		{
			int     precision;
			char *  rounding;
		};
	} extension;
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

%token <string>     ID
%token <value>      INTEGER
%token <fvalue>     FLOAT
%token <string>     PRAGMA
%token <string>     SYMBOL

%type <extension>   EXTENSION
%type <nvalue>      EXPR
%type <nvalue>      NUMBER
%type <vars>        VAR

%left '*' '/'
%left '+' '-'


%%

P_PRAGMA: 
	PRAGMA P_EXTENSION {printf("Passe dans 1\n");}
	;

P_EXTENSION:
	 EXTENSION P_EXTENSION {printf("Passe dans 2\n");}
	| { printf(" passe vide\n");}
	;

EXTENSION: 
	SYMBOL '(' INTEGER ')'
	 { 
		int type;
		printf("Passe preci\n");
		if ( (type = checkExtension($1) ) == ERROR ) 
		{
			printf("Not supported %s\n",$1);
			// return 1;
		}
		if ( type == PRECISION ) 
		{
			printf("Ext %s\n",$1);
			$$.precision = $3;
		}
	 }
	| SYMBOL '(' SYMBOL ')'
	 { 
		int type;
		printf("Passe roundings\n");
		if ( (type=checkExtension($1) ) == ERROR ) 
		{
			printf("Not supported %s\n",$1);
			// return 1;
		}
		if (type == ROUNDING) 
		{
			printf("Ext %s\n",$1);
			$$.rounding = $3;
		}
	 }
	;

INSTRUC:
	'{' INTRUC '}'
	| IF '(' CONDITION ')' INSTRUC
	| WHILE '(' CONDITION ')' INTRUC
	| FOR '(' INTRUC ';' CONDITION ';' INSTRUC ')' INSTRUC 
	| AFF
	;

CONDITION:
	CONDITIONp OR CONDITIONp;
	| CONDITIONp;

CONDITIONp:
	CONDITIONpp AND CONDITIONpp
	| CONDITIONpp;

CONDITIONpp:
	'(' CONDITION ')'
	| COMPARISON
	| TRUE
	| FALSE;

COMPARISON:
	EXPR '<' EXPR
	| EXPR '>' EXPR
	| EXPR LTE EXPR
	| EXPR GTE EXPR
	| EXPR EQ EXPR
	| EXPR NEQ EXPR
	;

AFF: 
      VAR '=' EXPR ';'      { printf("VAR = EXPR\n"); }
    | VAR '=' FCT ';'       { printf("VAR = FCT\n"); }
    | '(' VAR '=' EXPR ')' 
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
      { printf("FCT %s\n", $1); 
        if (parseFct($1) == UNKNOWN) printf("Unknown function\n"); 
      }
    ;

VAR: 
      SYMBOL        { printf("VAR %s\n", $1); }
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
