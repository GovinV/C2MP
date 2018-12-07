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
}

%token <string>     ID
%token <value>      INTEGER
%token <fvalue>     FLOAT
%token <string>     PRAGMA
%token <string>     SYMBOL
%type <extension>   EXTENSION


%%

P_PRAGMA: 
  PRAGMA P_EXTENSION {printf("Passe dans 1\n");}
  ;

P_EXTENSION:
  P_EXTENSION ' ' EXTENSION {printf("Passe dans 2\n");}
  | { printf(" passe vide\n");}
  ;

EXTENSION: 
  ID SPACE '(' SPACE INTEGER SPACE ')' SPACE 
           { 
              int type;
              printf("Passe preci\n");
              if ( (type = checkExtension($1) ) == ERROR ) 
              {
                printf("Not supported %s\n",$1);
                return 1;
              }
              if ( type == PRECISION ) 
              {
                printf("Ext %s\n",$1);
                $$.precision = $5;
              }
           }
  | ID SPACE '(' SPACE ID SPACE ')' SPACE
           { 
              int type;
              printf("Passe roundings\n");
              if ( (type=checkExtension($1) ) == ERROR ) 
              {
                printf("Not supported %s\n",$1);
                return 1;
              }
              if (type == ROUNDING) 
              {
                printf("Ext %s\n",$1);
                $$.rounding = $5;
              }
           }
  ;
  
/**
 * Not necessary space
 */
SPACE: 
  ' '
  |
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
	printf("End of parsing\n");
	return 0;
}
