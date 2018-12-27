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
	
	struct insQuad{
		int aff;
		char op;
		int val1;
		int val2;
	} insQuad;
}

%token <string>     ID
%token <value>      INTEGER
%token <fvalue>     FLOAT
%token <string>     PRAGMA
%token <string>     SYMBOL
%token 				OR
%token 				AND
%token 				GTE
%token 				LTE
%token 				IF
%token 				FOR
%token 				WHILE
%token				NEQ
%token 				EQ
%token				PRECISION
%token 				ROUNDING

%type <extension>   EXTENSION
%type <nvalue>      EXPR
%type <nvalue>      NUMBER
%type <vars>        VAR
%type <insQuad>     INSTRUCTION
%type <int>         CONDITION

%left OR
%left AND
%left '|'
%left '^'
%left '&'
%left EQ NEQ
%left '<' '>' GTE LTE

%left '+' '-'
%left '*' '/'

%left '!'


%%

P_PRAGMA:
	PRAGMA P_EXTENSION '\n' INSTRUCTION {printf("PRAGMA trouve\n");}
	;

P_EXTENSION:
	 EXTENSION P_EXTENSION {printf("extension trouvee\n");}
	|
	;


EXTENSION:
	PRECISION '(' INTEGER ')'
	 {
		int type;
		printf("Tentative d'assigner une precision\n");
		/*if ( (type = checkExtension($1) ) == ERROR ) 
		{
			printf("Precision non reconnue %s\n",$1);
			// return 1;
		}
		if ( type == PRECISION ) 
		{
			printf("Ext %s\n",$1);
		}*/
		$$.precision = $3;
		printf("Precision mise a %d\n", $$.precision);
	 }
	| ROUNDING '(' SYMBOL ')'
	 {
		int type;
		printf("Tentative d'assigner un arrondi\n");
		/*if ( (type=checkExtension($1) ) == ERROR ) 
		{
			printf("Arrondi non reconnu %s\n",$1);
			//return 1;
		}
		if (type == ROUNDING) 
		{
			printf("Ext %s\n",$1);
		}*/
		$$.rounding = $3;
		printf("rounding\n");
		printf("Arrondi mis a %s\n", $$.rounding);
	 }
	;

INSTRUCTION_LIST:
    INSTRUCTION INSTRUCTION_LIST
    | INSTRUCTION
    ;

INSTRUCTION:
    '{' INSTRUCTION_LIST '}'                                            {struct insQuad q;$$ = q;; /* pour enlever les warning, mais a terme ca sera des gencodes */ }
    | IF '(' EXPR ')' INSTRUCTION                                       {$$ = $5;}
	| WHILE '(' EXPR ')' INSTRUCTION                                    {$$ = $5;}
	| FOR '(' INSTRUCTION ';' EXPR ';' INSTRUCTION ')' INSTRUCTION      {$$ = $7;}
	| AFF ';'                                                           {struct insQuad q;$$ = q;}
	| ';'                                                               {struct insQuad q;$$ = q;}
	;

/*CONDITION:
    COMPARISON
    | '(' CONDITION ')'
    | CONDITION OR CONDITION        { printf("REDUCED OR\n"); }
    | CONDITION AND CONDITION       { printf("REDUCED AND\n"); }
    | CONDITION '&' CONDITION
    | CONDITION '^' CONDITION
    | CONDITION '|' CONDITION
    | '!' CONDITION
    ;*/

/*COMPARISON:
	EXPR '<' EXPR
	| EXPR '>' EXPR
	| EXPR LTE EXPR
	| EXPR GTE EXPR
	| EXPR EQ EXPR
	| EXPR NEQ EXPR
	//| EXPR // TODO : while(1) empeche par ce commentaire. le pb semble etre regle en dupliquant les boucles
	;*/

RVALUE:
	EXPR
	;	


AFF: 
    VAR '=' RVALUE          { printf("VAR = osef\n"); }
    | '(' AFF ')'
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
    | '-' EXPR      { printf("- EXPR\n"); }
    | '+' EXPR      { printf("+ EXPR\n"); }
    | EXPR '<' EXPR { printf("EXPR < EXPR\n"); }
	| EXPR '>' EXPR { printf("EXPR > EXPR\n"); }
	| EXPR LTE EXPR { printf("EXPR <= EXPR\n"); }
	| EXPR GTE EXPR { printf("EXPR >= EXPR\n"); }
	| EXPR EQ EXPR  { printf("EXPR == EXPR\n"); }
	| EXPR NEQ EXPR { printf("EXPR != EXPR\n"); }
    | EXPR OR EXPR  { printf("EXPR || EXPR\n"); }
    | EXPR AND EXPR { printf("EXPR && EXPR\n"); }
    | EXPR '&' EXPR
    | EXPR '^' EXPR
    | EXPR '|' EXPR
    | '!' EXPR      { printf("!EXPR\n"); }
    | '(' EXPR ')'  { printf("(EXPR)\n"); }
    | VAR           { printf("EXPR = SYMBOL %s\n", $1.name); }
    | NUMBER        { printf("EXPR = NUMBER\n"); }
	| FCT           { printf("EXPR = FUNCTION\n"); }
    ;

FCT:
      SYMBOL '(' EXPR ARG ')' /* PEUTETRE QUON SEN FOUT DES ARGUMENTS */
      { printf("FCT %s\n", $1); 
        if (parseFct($1) == UNKNOWN) printf("Unknown function\n"); 
      }
    ;

VAR:
      SYMBOL        //{ printf("VAR %s\n", $1); }
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
		fprintf(stderr, "Missing arg\n");
		return 1;
	}
 
	yyin = fopen(argv[1], "r");
	yyout = stdout;
	yyparse();
	printf("End of parsing\n");
	return 0;
}
