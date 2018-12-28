%{
    #define NUM_TYPE_FLOAT 0
    #define NUM_TYPE_INTEGER 1

	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include "utils.h"
	int yylex();
	void yyerror(char*);
 
	FILE *yyin, *yyout;
	int commentNum = 0;

    typedef struct expressionAST expressionAST;

    expressionAST *createExpressionAST(char operator, expressionAST *expr1, expressionAST *expr2);
    expressionAST *createIntAST(int integer);
    expressionAST *createFloatAST(float number);
    expressionAST *createVariableAST(int variable);
    void freeExpressionAST(expressionAST *expr);
    void printExpressionAST(expressionAST *expr);
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
    struct number
    {
        char type; // NUM_TYPE_INTEGER = int, NUM_TYPE_FLOAT = float
        union 
        {
          int valueInt;
          float valueFloat;
        };
    } number;

    struct
    {
        const char * name;
        int ref;
        union{
            int valueInt;
            float valueFloat;
        };
    } vars;
	
	struct expressionAST
    {
		char operator;
        union
        {
		    struct {
                struct expressionAST *e1;
		        struct expressionAST *e2;
            } expression;
            int valueInt;
            float valueFloat;
            int valueVariable;
        };
	} *expressionAST;
	
	struct quad
    {
		int assigned; // variable to which the operation is affected
		char operator;
		int value1;
		int value2;
		struct quad *previous;
		struct quad *next;
	} quad;
	
	struct semiQuad
    { // quad that can have an AST expression for val1
		int assigned;
		char operator;
        union
        { // depending on the operator
		    struct expressionAST *expression; // example : "="
		    int variable; // example "if"
        };
		struct semiQuad *previous;
		struct semiQuad *next;
	} semiQuad;
}

%token <string>         ID
%token <value>          INTEGER
%token <fvalue>         FLOAT
%token <string>         PRAGMA
%token <string>         SYMBOL
%token 				    OR
%token 				    AND
%token 				    GTE
%token 				    LTE
%token 				    IF
%token 				    FOR
%token 				    WHILE
%token				    NEQ
%token 				    EQ
%token				    PRECISION
%token 				    ROUNDING

%type <extension>       EXTENSION
%type <expressionAST>   RVALUE
%type <expressionAST>   EXPR
%type <number>          NUMBER
%type <vars>            VAR
%type <quadInfo>        INSTRUCTION
%type <int>             CONDITION

%left OR
%left AND
%left '|'
%left '^'
%left '&'
%left EQ NEQ
%left '<' '>' GTE LTE

%left '+' '-'
%left '*' '/'

%left '!' '~'


%%

P_PRAGMA:
	PRAGMA P_EXTENSION '\n' INSTRUCTION {printf("PRAGMA LU\n");}
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
    '{' INSTRUCTION_LIST '}'
    | IF '(' EXPR ')' INSTRUCTION
	| WHILE '(' EXPR ')' INSTRUCTION
	| FOR '(' INSTRUCTION ';' EXPR ';' INSTRUCTION ')' INSTRUCTION
	| ASSIGNMENT ';'
	| ';'
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
	//| EXPR // TODO : while(1) empeche par ce commentaire. le pb semble etre regle en dupliquant les boucles. le pb est encore plus regle en fusionnant expr et condition
	;*/

RVALUE:
	EXPR                    { $$ = $1; }
	;


ASSIGNMENT: 
    VAR '=' RVALUE          {
                                printf("AST trouve : ");
                                printExpressionAST($3);
                                printf("\n");
                            }
    | '(' ASSIGNMENT ')'
    ;



NUMBER:
      INTEGER       { $$.type = 1; $$.valueInt = $1; }
    | FLOAT         { $$.type = 0; $$.valueFloat = $1; }
    ;

EXPR:
      EXPR '+' EXPR     { $$ = createExpressionAST('+', $1, $3); }
    | EXPR '*' EXPR     { $$ = createExpressionAST('*', $1, $3); }
    | EXPR '-' EXPR     { $$ = createExpressionAST('-', $1, $3); }
    | EXPR '/' EXPR     { $$ = createExpressionAST('/', $1, $3); }
    | '-' EXPR          { $$ = createExpressionAST('m', $2, NULL); }
    | '+' EXPR          { $$ = createExpressionAST('p', $2, NULL); }
    | EXPR '<' EXPR     { $$ = createExpressionAST('<', $1, $3); }
	| EXPR '>' EXPR     { $$ = createExpressionAST('>', $1, $3); }
	| EXPR LTE EXPR     { $$ = createExpressionAST('i', $1, $3); }
	| EXPR GTE EXPR     { $$ = createExpressionAST('s', $1, $3); }
	| EXPR EQ EXPR      { $$ = createExpressionAST('e', $1, $3); }
	| EXPR NEQ EXPR     { $$ = createExpressionAST('E', $1, $3); }
    | EXPR OR EXPR      { $$ = createExpressionAST('O', $1, $3); }
    | EXPR AND EXPR     { $$ = createExpressionAST('A', $1, $3); }
    | EXPR '&' EXPR     { $$ = createExpressionAST('&', $1, $3); }
    | EXPR '^' EXPR     { $$ = createExpressionAST('^', $1, $3); }
    | EXPR '|' EXPR     { $$ = createExpressionAST('|', $1, $3); }
    | '!' EXPR          { $$ = createExpressionAST('!', $2, NULL); }
    | '~' EXPR          { $$ = createExpressionAST('~', $2, NULL); }
    | '(' EXPR ')'      { $$ = $2; }
    | VAR               { $$ = createVariableAST(5); }
    | NUMBER            {
                            switch($1.type)
                            {
                                case NUM_TYPE_FLOAT:
                                    $$ = createFloatAST($1.valueFloat);
                                    break;
                                case NUM_TYPE_INTEGER:
                                    $$ = createFloatAST($1.valueInt);
                                    break;
                                default:
                                    fprintf(stderr, "Unknown number type %d\n", $1.type);
                            }
                        }
	| FCT               { printf("EXPR = FUNCTION\n"); }
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

expressionAST *createExpressionAST(char operator, expressionAST *expr1, expressionAST *expr2)
{
    expressionAST *expr = malloc(sizeof(expressionAST));
    expr->operator = operator;
    expr->expression.e1 = expr1;
    expr->expression.e2 = expr2;
    return expr;
}

expressionAST *createIntAST(int integer)
{
    expressionAST *expr = malloc(sizeof(expressionAST));
    expr->operator = 'n';
    expr->valueInt = integer;
    return expr;
}

expressionAST *createFloatAST(float number)
{
    expressionAST *expr = malloc(sizeof(expressionAST));
    expr->operator = 'f';
    expr->valueFloat = number;
    return expr;
}

expressionAST *createVariableAST(int variable)
{
    expressionAST *expr = malloc(sizeof(expressionAST));
    expr->operator = 'v';
    expr->valueVariable = variable;
    return expr;
}

/*
list of operators :
< > : binary
i s : <= (i) and >= (s)
e E : binary == (e) and != (E)
+ - * / : binary
| : bitwise or
& : bitwise and
^ : xor
O : logical or
A : logical and


m : unary -
p : unary +
! : logical not
~ : bitwise not
*/
void freeExpressionAST(expressionAST *expr)
{
    if(expr == NULL)
    {
        fprintf(stderr, "Warning, tried to free a NULL expression\n");
        return;
    }
    switch(expr->operator)
    {
        case '+':
        case '-':
        case '*':
        case '/':
        case '<':
        case '>':
        case 'i':
        case 's':
        case 'e':
        case 'E':
        case '&':
        case '|':
        case '^':
        case 'A':
        case 'O':
            freeExpressionAST(expr->expression.e1);
            freeExpressionAST(expr->expression.e2);
            break;
        case 'm': // unary minus
        case 'p': // unary plus
        case '!':
        case '~':
            freeExpressionAST(expr->expression.e1);
            break;
        case 'n': // number
        case 'f': // float
        case 'v': // variable
            free(expr);
            break;
        default:
            fprintf(stderr, "Warning, unknown operation : %c\n", expr->operator);
    }
}

void printExpressionAST(expressionAST *expr)
{
    if(expr == NULL)
    {
        fprintf(stderr, "Warning, tried to print a NULL expression\n");
        return;
    }
    switch(expr->operator)
    {
        case '+':
        case '-':
        case '*':
        case '/':
        case '<':
        case '>':
        case 'i':
        case 's':
        case 'e':
        case 'E':
        case '&':
        case '|':
        case '^':
        case 'A':
        case 'O':
            printf("(");
            printExpressionAST(expr->expression.e1);
            printf("%c", expr->operator);
            printExpressionAST(expr->expression.e2);
            printf(")");
            break;
        case 'm': // unary minus
        case 'p': // unary plus
        case '!':
        case '~':
            printf("(");
            printf("%c", expr->operator);
            printExpressionAST(expr->expression.e1);
            printf(")");
            break;
        case 'n': // number
            printf("%d", expr->valueInt);
            break;
        case 'f': // float
            printf("%f", expr->valueFloat);
            break;
        case 'v': // variable
            printf("var(%d)", expr->valueVariable);
            break;
        default:
            fprintf(stderr, "Warning, unknown operation : %c\n", expr->operator);
    }
}

int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		fprintf(stderr, "Missing arg\n");
		return 1;
	}

    /*expressionAST *testAST = createExpressionAST('+', createExpressionAST('m', createVariableAST(5), NULL), createFloatAST(5));
    freeExpressionAST(testAST);*/
 
	yyin = fopen(argv[1], "r");
	yyout = stdout;
	yyparse();
	printf("End of parsing\n");
	return 0;
}
