%{
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

    #define C2MP_NUM_TYPE_FLOAT 0
    #define C2MP_NUM_TYPE_INTEGER 1

    #define C2MP_OPERATOR_BINARY_PLUS       '+'
    #define C2MP_OPERATOR_BINARY_MINUS      '-'
    #define C2MP_OPERATOR_BINARY_DOT        '*'
    #define C2MP_OPERATOR_BINARY_DIVIDE     '/'

    #define C2MP_OPERATOR_UNARY_PLUS        'p'
    #define C2MP_OPERATOR_UNARY_MINUS       'm'

    #define C2MP_OPERATOR_LOWER_THAN        '<'
    #define C2MP_OPERATOR_GREATER_THAN      '>'
    #define C2MP_OPERATOR_LOWER_OR_EQUAL    'i'
    #define C2MP_OPERATOR_GREATER_OR_EQUAL  's'
    #define C2MP_OPERATOR_EQUAL             'e'
    #define C2MP_OPERATOR_NOT_EQUAL         'E'

    #define C2MP_OPERATOR_LOGICAL_AND       'A'
    #define C2MP_OPERATOR_LOGICAL_OR        'O'
    #define C2MP_OPERATOR_LOGICAL_NOT       '!'
    #define C2MP_OPERATOR_BITWISE_AND       '&'
    #define C2MP_OPERATOR_BITWISE_OR        '|'
    #define C2MP_OPERATOR_BITWISE_XOR       '^'
    #define C2MP_OPERATOR_BITWISE_NOT       '~'

    #define C2MP_CHARACTER_INTEGER          'n'
    #define C2MP_CHARACTER_FLOAT            'f'
    #define C2MP_CHARACTER_VARIABLE         'v'



    #define C2MP_QUAD_ASSIGNMENT            '='
    #define C2MP_QUAD_IF                    'I'
    #define C2MP_QUAD_ELSE                  'z'
    #define C2MP_QUAD_ENDIF                 '}'



    #define MAX_VARIABLES 1024*8


	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include "utils.h"
	int yylex();
	void yyerror(char*);
 
	FILE *yyin, *yyout;
	int commentNum = 0;

    typedef struct expressionAST expressionAST;
    typedef struct semiQuad semiQuad;
    typedef struct quad quad;

    expressionAST *createExpressionAST(char operator, expressionAST *expr1, expressionAST *expr2);
    expressionAST *createIntAST(int integer);
    expressionAST *createFloatAST(float number);
    expressionAST *createVariableAST(int variable);
    void freeExpressionAST(expressionAST *expr);
    void printExpressionAST(expressionAST *expr);

    semiQuad *createSemiQuad(char operator, int assignment, expressionAST *expression);
    semiQuad *concatSemiQuad(semiQuad *q1, semiQuad *q2);
    void printSemiQuads(semiQuad *q1);

    int getVariableReference(const char name[]);
    int getReferenceFromName(const char name[]);
    const char *getNameFromReference(int reference);
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
	
	struct quad
    {
		int assigned; // variable to which the operation is affected
		char operator;
		int value1;
		int value2;
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


%%

P_PRAGMA:
	PRAGMA P_EXTENSION '\n' INSTRUCTION {
                                            printf("PRAGMA read, result :\n");
                                            printSemiQuads($4);
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
	| WHILE '(' EXPR ')' INSTRUCTION
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


void freeExpressionAST(expressionAST *expr)
{
    if(expr == NULL)
    {
        fprintf(stderr, "Warning, tried to free a NULL expression\n");
        return;
    }
    switch(expr->operator)
    {
        case C2MP_OPERATOR_BINARY_PLUS:
        case C2MP_OPERATOR_BINARY_MINUS:
        case C2MP_OPERATOR_BINARY_DOT:
        case C2MP_OPERATOR_BINARY_DIVIDE:
        case C2MP_OPERATOR_LOWER_THAN:
        case C2MP_OPERATOR_GREATER_THAN:
        case C2MP_OPERATOR_LOWER_OR_EQUAL:
        case C2MP_OPERATOR_GREATER_OR_EQUAL:
        case C2MP_OPERATOR_EQUAL:
        case C2MP_OPERATOR_NOT_EQUAL:
        case C2MP_OPERATOR_BITWISE_AND:
        case C2MP_OPERATOR_BITWISE_OR:
        case C2MP_OPERATOR_BITWISE_XOR:
        case C2MP_OPERATOR_LOGICAL_AND:
        case C2MP_OPERATOR_LOGICAL_OR:
            freeExpressionAST(expr->expression.e1);
            freeExpressionAST(expr->expression.e2);
            break;
        case C2MP_OPERATOR_UNARY_MINUS: // unary minus
        case C2MP_OPERATOR_UNARY_PLUS: // unary plus
        case C2MP_OPERATOR_LOGICAL_NOT:
        case C2MP_OPERATOR_BITWISE_NOT:
            freeExpressionAST(expr->expression.e1);
            break;
        case C2MP_CHARACTER_INTEGER: // number
        case C2MP_CHARACTER_FLOAT: // float
        case C2MP_CHARACTER_VARIABLE: // variable
            free(expr);
            break;
        default:
            fprintf(stderr, "Warning, unknown expression operation : %c\n", expr->operator);
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
        case C2MP_OPERATOR_BINARY_PLUS:
        case C2MP_OPERATOR_BINARY_MINUS:
        case C2MP_OPERATOR_BINARY_DOT:
        case C2MP_OPERATOR_BINARY_DIVIDE:
        case C2MP_OPERATOR_LOWER_THAN:
        case C2MP_OPERATOR_GREATER_THAN:
        case C2MP_OPERATOR_LOWER_OR_EQUAL:
        case C2MP_OPERATOR_GREATER_OR_EQUAL:
        case C2MP_OPERATOR_EQUAL:
        case C2MP_OPERATOR_NOT_EQUAL:
        case C2MP_OPERATOR_BITWISE_AND:
        case C2MP_OPERATOR_BITWISE_OR:
        case C2MP_OPERATOR_BITWISE_XOR:
        case C2MP_OPERATOR_LOGICAL_AND:
        case C2MP_OPERATOR_LOGICAL_OR:
            printf("(");
            printExpressionAST(expr->expression.e1);
            printf("%c", expr->operator);
            printExpressionAST(expr->expression.e2);
            printf(")");
            break;
        case C2MP_OPERATOR_UNARY_MINUS:
        case C2MP_OPERATOR_UNARY_PLUS:
        case C2MP_OPERATOR_LOGICAL_NOT:
        case C2MP_OPERATOR_BITWISE_NOT:
            printf("(");
            printf("%c", expr->operator);
            printExpressionAST(expr->expression.e1);
            printf(")");
            break;
        case C2MP_CHARACTER_INTEGER: // number
            printf("%d", expr->valueInt);
            break;
        case C2MP_CHARACTER_FLOAT: // float
            printf("%f", expr->valueFloat);
            break;
        case C2MP_CHARACTER_VARIABLE: // variable
            printf("(%s)", getNameFromReference(expr->valueVariable));
            break;
        default:
            fprintf(stderr, "Warning, unknown expression operation : %c\n", expr->operator);
    }
}


semiQuad *createSemiQuad(char operator, int assignment, expressionAST *expression)
{
    semiQuad *quad = malloc(sizeof(semiQuad));
    quad->operator = operator;
    quad->assignment = assignment;
    quad->expression = expression;
    quad->next = quad;
    quad->previous = quad;

    return quad;
}

// concatenates 2 quads or 2 quadlists
semiQuad *concatSemiQuad(semiQuad *q1, semiQuad *q2)
{
    if(q2 == NULL)
    {
        return q1;
    }
    if(q1 == NULL)
    {
        return q2;
    }

    /*if(q2->next == q2)
    {
        q2->next = q1;
        q2->previous = q1->previous;
        q1->previous->next = q2;
        q1->previous = q2;
    }
    else
    {
        q1->previous->next = q2;
        q2->previous->next = q1;
        q1->previous = q2->previous;
        q2->previous = q1->previous;
    }*/
    semiQuad *head1 = q1;
    semiQuad *head2 = q2;
    semiQuad *tail1 = q1->previous;
    semiQuad *tail2 = q2->previous;

    head1->previous = tail2;
    tail1->next = head2;
    head2->previous = tail1;
    tail2->next = head1;

    return q1;
}

void printSemiQuads(semiQuad *q)
{
    if(q == NULL)
    {
        printf("no quad\n");
    }

    semiQuad *firstQuad = q;
    semiQuad *currentQuad = q;

    int indent = 0;

    do
    {
        for(int i=0;i<indent;++i)
        { // indent quads
            printf("  ");
        }

        switch(currentQuad->operator)
        {
            case C2MP_QUAD_ASSIGNMENT:
                printf("%s = ", getNameFromReference(currentQuad->assignment));
                printExpressionAST(currentQuad->expression);
                break;
            case C2MP_QUAD_IF:
                ++indent;
                printf("if ");
                printExpressionAST(currentQuad->expression);
                break;
            case C2MP_QUAD_ELSE:
                printf("\b\belse");
                break;
            case C2MP_QUAD_ENDIF:
                --indent;
                printf("\b\bendif");
                break;
            default:
                fprintf(stderr, "Warning, unknown semi quad operation : %d (%c)\n", currentQuad->operator, currentQuad->operator);
        }
        printf("\n");

        currentQuad = currentQuad->next;
    }while(currentQuad != firstQuad);
}



char *variables[MAX_VARIABLES];
int variablesSize = 0;

// if the variable is found, returns a reference, else -1
int getVariableReference(const char name[])
{
    for(int i=0;i<variablesSize;++i)
    {
        if(strcmp(name, variables[i]) == 0)
        {
            return i;
        }
    }
    return -1;
}

// if the variable is found, returns a reference, else creates one and returns it
int getReferenceFromName(const char name[])
{
    int reference = getVariableReference(name);
    if(reference == -1)
    {
        variables[variablesSize] = strdup(name);
        return variablesSize++;
    }

    return reference;
}

// returns the name of the variable
const char *getNameFromReference(int reference)
{
    return variables[reference];
}

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
