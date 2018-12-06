%{
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include "utils.h"
  int yylex();
  void yyerror(char*);
%}

%union {
  char* string;
  int value;
  int precision;
  const char * rounding;
}

%token <string> ID
%token <value> NUMBER
%token <string> PRAGMA

%left '+'
%left '*'

%%

PRAGMA: P_EXTENSION
  ;

P_EXTENSION:
  | EXTENSION EXTENSION
  ;

EXTENSION: ID SPACE '(' SPACE NUMBER SPACE ')' SPACE 
           { int type;
             if ((type=checkExtension($1)) == ERROR) return 1;
             if (type == PRECISION) $$.precision = $2.value;
             if (type == ROUNDING) $$.rounding = $2.string;
           }
  | VOID
  ;
  
/**
 * Not necessary space
 */
SPACE: 
  | " "
  | VOID
  ;

VOID:
  | ""
  ;

%%

int main() {
  printf("Entrez une expression :\n");
  return yyparse();
}
