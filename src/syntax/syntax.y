%{
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include "utils.h"

	int yylex();
	void yyerror(char*);
 
	FILE *yyin, *yyout;
	int commentNum = 0;

typedef struct quadOperand quadOperand;
typedef struct semiQuad semiQuad;
typedef struct quad quad;
typedef struct expressionAST expressionAST;

quadOperand createVariableOperand(int reference);
quadOperand createIntegerOperand(int value);
quadOperand createFloatOperand(float value);
quadOperand createVoidOperand(void);

quad *createQuad(int assignment, char operator, quadOperand value1, quadOperand value2);
quad *copySemiQuad(semiQuad *sq);
quad *generateQuadsFromAST(expressionAST *expr);
quad *getQuadFromSemiQuad(semiQuad *sq);
quad *concatQuads(quad *q1, quad *q2);
void printOperand(quadOperand operand);
void printQuads(quad* q);

expressionAST *createExpressionAST(char operator, expressionAST *expr1, expressionAST *expr2);
expressionAST *createIntAST(int integer);
expressionAST *createFloatAST(float number);
expressionAST *createVariableAST(int variable);
void freeExpressionAST(expressionAST *expr);
void printExpressionAST(expressionAST *expr);

int getSymbolReference(const char name[]);
int getReferenceFromName(const char name[]);
const char *getNameFromReference(int reference);

semiQuad *createSemiQuad(char operator, int assignment, expressionAST *expression);
semiQuad *concatSemiQuad(semiQuad *q1, semiQuad *q2);
void printSemiQuads(semiQuad *q1);
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
        char type; // C2MP_NUM_TYPE_INTEGER = int, C2MP_NUM_TYPE_FLOAT = float
        union 
        {
          int valueInt;
          float valueFloat;
        };
    } number;

    struct
    {
        const char * name;
        int reference;
        /*union{
            int valueInt;
            float valueFloat;
        };*/
    } variable;
	
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
	
	struct semiQuad
    { // quad that can have an AST expression for val1
		int assignment;
		char operator;

        /*union
        { // depending on the operator
            struct expressionAST *expression; // example : "="
            int variable; // example "if"
        };*/
        struct expressionAST *expression;

        struct semiQuad *previous;
        struct semiQuad *next;
    } *semiQuad;

    struct quadOperand
    {
        int type;
        union
        {
            int reference;
            int valueInt;
            float valueFloat;
        };
    } quadOperand;

    struct quad
    {
        int assignment; // variable to which the operation is affected
        char operator;

        struct quadOperand operand1;
        struct quadOperand operand2;

        struct quad *previous;
        struct quad *next;
    } *quad;
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
%token 				    ELSE
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
%type <variable>        VAR
%type <semiQuad>        INSTRUCTION
%type <semiQuad>        INSTRUCTION_LIST
%type <int>             CONDITION
%type <semiQuad>        ASSIGNMENT

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

%right ')' ELSE


%%

P_PRAGMA:
	PRAGMA P_EXTENSION '\n' INSTRUCTION {
                                            printf("generated semi quads :\n");
                                            printSemiQuads($4);
                                            printf("generated quads :\n");
                                            quad *quads = getQuadFromSemiQuad($4);
                                            printf("... :\n");
                                            printQuads(quads);
                                        }
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
    INSTRUCTION INSTRUCTION_LIST                { $$ = concatSemiQuad($1, $2); }
    | INSTRUCTION                               { $$ = $1; }
    ;

INSTRUCTION:
    '{' INSTRUCTION_LIST '}'                    {
                                                    $$ = $2;
                                                }
    | IF '(' EXPR ')' INSTRUCTION               {
                                                    semiQuad *ifQuad = createSemiQuad(C2MP_QUAD_IF, -1, $3);
                                                    semiQuad *endIfQuad = createSemiQuad(C2MP_QUAD_ENDIF, -1, NULL);
                                                    semiQuad *result;
                                                    result = concatSemiQuad(ifQuad, $5);
                                                    result = concatSemiQuad(result, endIfQuad);
                                                    $$ = result;
                                                }
    | IF '(' EXPR ')' INSTRUCTION ELSE INSTRUCTION {
                                                    semiQuad *ifQuad = createSemiQuad(C2MP_QUAD_IF, -1, $3);
                                                    semiQuad *elseQuad = createSemiQuad(C2MP_QUAD_ELSE, -1, NULL);
                                                    semiQuad *endIfQuad = createSemiQuad(C2MP_QUAD_ENDIF, -1, NULL);
                                                    semiQuad *result;
                                                    result = concatSemiQuad(ifQuad, $5);
                                                    result = concatSemiQuad(result, elseQuad);
                                                    result = concatSemiQuad(result, $7);
                                                    result = concatSemiQuad(result, endIfQuad);
                                                    $$ = result;
                                                }
	| WHILE '(' EXPR ')' INSTRUCTION            {
                                                    /*semiQuad *whileQuad = createSemiQuad(C2MP_QUAD_WHILE, -1, $3);
                                                    semiQuad *endWhileQuad = createSemiQuad(C2MP_QUAD_ENDWHILE, -1, NULL);
                                                    semiQuad *result;*/
                                                }
	| FOR '(' INSTRUCTION ';' EXPR ';' INSTRUCTION ')' INSTRUCTION
	| ASSIGNMENT ';'                                                    { $$ = $1; }
	| ';'                                                               { $$ = NULL; }
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
                                $$ = createSemiQuad(C2MP_QUAD_ASSIGNMENT, $1.reference, $3);
                            }
    | '(' ASSIGNMENT ')'
    ;



NUMBER:
      INTEGER       { $$.type = 1; $$.valueInt = $1; }
    | FLOAT         { $$.type = 0; $$.valueFloat = $1; }
    ;

EXPR:
      EXPR '+' EXPR     { $$ = createExpressionAST(C2MP_OPERATOR_BINARY_PLUS, $1, $3); }
    | EXPR '*' EXPR     { $$ = createExpressionAST(C2MP_OPERATOR_BINARY_DOT, $1, $3); }
    | EXPR '-' EXPR     { $$ = createExpressionAST(C2MP_OPERATOR_BINARY_MINUS, $1, $3); }
    | EXPR '/' EXPR     { $$ = createExpressionAST(C2MP_OPERATOR_BINARY_DIVIDE, $1, $3); }
    | '-' EXPR          { $$ = createExpressionAST(C2MP_OPERATOR_UNARY_MINUS, $2, NULL); }
    | '+' EXPR          { $$ = createExpressionAST(C2MP_OPERATOR_UNARY_PLUS, $2, NULL); }
    | EXPR '<' EXPR     { $$ = createExpressionAST(C2MP_OPERATOR_LOWER_THAN, $1, $3); }
	| EXPR '>' EXPR     { $$ = createExpressionAST(C2MP_OPERATOR_GREATER_THAN, $1, $3); }
	| EXPR LTE EXPR     { $$ = createExpressionAST(C2MP_OPERATOR_LOWER_OR_EQUAL, $1, $3); }
	| EXPR GTE EXPR     { $$ = createExpressionAST(C2MP_OPERATOR_GREATER_OR_EQUAL, $1, $3); }
	| EXPR EQ EXPR      { $$ = createExpressionAST(C2MP_OPERATOR_EQUAL, $1, $3); }
	| EXPR NEQ EXPR     { $$ = createExpressionAST(C2MP_OPERATOR_NOT_EQUAL, $1, $3); }
    | EXPR OR EXPR      { $$ = createExpressionAST(C2MP_OPERATOR_LOGICAL_OR, $1, $3); }
    | EXPR AND EXPR     { $$ = createExpressionAST(C2MP_OPERATOR_LOGICAL_AND, $1, $3); }
    | EXPR '&' EXPR     { $$ = createExpressionAST(C2MP_OPERATOR_BITWISE_AND, $1, $3); }
    | EXPR '^' EXPR     { $$ = createExpressionAST(C2MP_OPERATOR_BITWISE_XOR, $1, $3); }
    | EXPR '|' EXPR     { $$ = createExpressionAST(C2MP_OPERATOR_BITWISE_OR, $1, $3); }
    | '!' EXPR          { $$ = createExpressionAST(C2MP_OPERATOR_LOGICAL_NOT, $2, NULL); }
    | '~' EXPR          { $$ = createExpressionAST(C2MP_OPERATOR_BITWISE_NOT, $2, NULL); }
    | '(' EXPR ')'      { $$ = $2; }
    | VAR               { $$ = createVariableAST($1.reference); }
    | NUMBER            {
                            switch($1.type)
                            {
                                case C2MP_NUM_TYPE_FLOAT:
                                    $$ = createFloatAST($1.valueFloat);
                                    break;
                                case C2MP_NUM_TYPE_INTEGER:
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
      SYMBOL            {
                            $$.name = $1;
                            $$.reference = getReferenceFromName($1);
                        }
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

    /*semiQuad *bloc1 = createSemiQuad(C2MP_QUAD_ASSIGNMENT, 0, createIntAST(1));
    bloc1 = concatSemiQuad(bloc1, createSemiQuad(C2MP_QUAD_ASSIGNMENT, 0, createIntAST(1)));
    bloc1 = concatSemiQuad(bloc1, createSemiQuad(C2MP_QUAD_ASSIGNMENT, 0, createIntAST(2)));
    bloc1 = concatSemiQuad(bloc1, createSemiQuad(C2MP_QUAD_ASSIGNMENT, 0, createIntAST(3)));

    semiQuad *bloc2 = createSemiQuad(C2MP_QUAD_ASSIGNMENT, 0, createIntAST(1));
    bloc2 = concatSemiQuad(bloc2, createSemiQuad(C2MP_QUAD_ASSIGNMENT, 0, createIntAST(4)));
    bloc2 = concatSemiQuad(bloc2, createSemiQuad(C2MP_QUAD_ASSIGNMENT, 0, createIntAST(3)));
    bloc2 = concatSemiQuad(bloc2, createSemiQuad(C2MP_QUAD_ASSIGNMENT, 0, createIntAST(2)));

    semiQuad *code = createSemiQuad(C2MP_QUAD_IF, -1, createIntAST(1));
    code = concatSemiQuad(code, bloc1);
    code = concatSemiQuad(code, createSemiQuad(C2MP_QUAD_ELSE, -1, NULL));
    code = concatSemiQuad(code, bloc2);
    code = concatSemiQuad(code, createSemiQuad(C2MP_QUAD_ENDIF, -1, NULL));

    printSemiQuads(code);*/
 
	yyin = fopen(argv[1], "r");
	yyout = stdout;
	yyparse();
	printf("End of parsing\n");
	return 0;
}
