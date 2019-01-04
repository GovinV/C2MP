%{
	#include <stdio.h>
    #include "utils.h"
	#include <stdlib.h>
	#include <string.h>
    #include <unistd.h>
    #include <stdbool.h>
	#include "symbol.h"
	

	int yylex();
	void yyerror(char*);
 
	FILE *yyin, *yyout;
	int commentNum = 0;

typedef struct quadOperand quadOperand;
typedef struct semiQuad semiQuad;
typedef struct quad quad;
typedef struct expressionAST expressionAST;
typedef struct argType argType;

expressionAST *copyExpressionAST(expressionAST *expressionAST);

quadOperand createVariableOperand(int reference);
quadOperand createIntegerOperand(int value);
quadOperand createFloatOperand(float value);
quadOperand createVoidOperand(void);

quad *createQuad(int assignment, char operator, char * name, int operandsNum, ...);
quad *copySemiQuad(semiQuad *sq);
quad *generateQuadsFromAST(expressionAST *expr);
quad *getQuadFromSemiQuad(semiQuad *sq);
quad *concatQuads(quad *q1, quad *q2);
void printOperand(quadOperand operand);
void printQuads(quad* q);

expressionAST *createExpressionAST(char operator, expressionAST *expr1, expressionAST *expr2);
expressionAST *createCustonFunctionAST(char *name, int argNum, ...);
expressionAST *createIntAST(int integer);
expressionAST *createFloatAST(float number);
expressionAST *createVariableAST(int variable);
expressionAST *createStringAST(const char *string);
void freeExpressionAST(expressionAST *expr);
void printExpressionAST(expressionAST *expr);

int getSymbolReference(const char name[]);
int getReferenceFromName(const char name[]);
const char *getNameFromReference(int reference);

semiQuad *createSemiQuad(char operator, int assignment, expressionAST *expression);
semiQuad *concatSemiQuad(semiQuad *q1, semiQuad *q2);
void printSemiQuads(semiQuad *q1);

symbol newTemp(void);

// generate.h
void generateCode(quad* q, char *rounding, int precision);

// optimization.h
quad* removeCommonSubExpressions(quad* quads);
quad* removeAllCommonSubExpressions(quad* quads);

%}

%union
{
	char  * string;
	int     value;
	float   fvalue;
	
	struct  pragmaExt 
	{
		enum {ROUNDING_T, PRECISION_T} type;
		union {    
            int     precision;
		    char *  rounding;
        };
    } extension;

    struct 
    {
        int precision;
        char * rounding;
    } p_extension;

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
            char *valueString;
            struct{
                char *name;
                int argnum;
                struct expressionAST *args[MAX_FCT_ARGS];
            } customFunction;
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
        char *fctName; // used if the quad represents a function
        struct quadOperand operands[MAX_FCT_ARGS];
        int operandsNum;
        struct quad *previous;
        struct quad *next;
    } *quad;

}

%token <string>         ID
%token <value>          INTEGER
%token <fvalue>         FLOAT
%token <string>         PRAGMA
%token <string>         SYMBOL
%token <string>         STRING
%token 				    OR
%token 				    AND
%token 				    GTE
%token 				    LTE
%token 				    IF
%token 				    ELSE
%token 				    FOR
%token 				    WHILE
%token 				    DO
%token				    NEQ
%token 				    EQ
%token				    PRECISION
%token 				    ROUNDING
%token                  BACKSLASH

%type <extension>       EXTENSION
%type <p_extension>     P_EXTENSION
%type <expressionAST>   RVALUE
%type <expressionAST>   EXPR
%type <expressionAST>   FCT
%type <expressionAST>   PARAM
%type <number>          NUMBER
%type <variable>        VAR
%type <semiQuad>        INSTRUCTION
%type <semiQuad>        BLOC
%type <semiQuad>        BLOC_LIST
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
	PRAGMA P_EXTENSION BACKSLASH BLOC
        {
            printf("***** Found Pragma *****\n");
            printf("Rounding = %s\n", $2.rounding);
            printf("Precision = %d\n", $2.precision);
            printf("generated semi quads :\n");
            printSemiQuads($4);
            printf("Quads generation:\n");
            quad *quads = getQuadFromSemiQuad($4);
            printf("End of quads generation\n");
            if (option_flag == 1)
            {
                printf("Optimization:\n");
                //printQuads(quads);
                quads = removeAllCommonSubExpressions(quads);
                printf("End of Optimization.\n");
            }
            generateCode(quads, $2.rounding, $2.precision);
            printf("\n");
        }
	;

P_EXTENSION:
	EXTENSION P_EXTENSION 
        { 
            $$ = $2;
            if ($1.type == ROUNDING_T)
                $$.rounding = $1.rounding;
            if ($1.type == PRECISION_T)
                $$.precision = $1.precision;
        }
	|                      
        { 
            $$.rounding = strdup("MPC_RNDZZ");
            $$.precision = 128; 
        }
	;


EXTENSION:
	PRECISION '(' INTEGER ')'
        {
            $$.type = PRECISION_T;
            $$.precision = $3;
        }
	| ROUNDING '(' SYMBOL ')'
        {
            $$.type = ROUNDING_T;
            $$.rounding = $3;
        }
	;

BLOC_LIST:
    BLOC BLOC_LIST
        { $$ = concatSemiQuad($1, $2); }
    | BLOC
        { $$ = $1; }
    ;

BLOC:
    '{' BLOC_LIST '}'                    
        { $$ = $2; }
    | IF '(' EXPR ')' BLOC               
        {
            semiQuad *ifQuad = createSemiQuad(C2MP_QUAD_IF, -1, $3);
            semiQuad *endIfQuad = createSemiQuad(C2MP_QUAD_ENDIF, -1, NULL);
            /*  ifs are assign to nothing so -1, and the condition is $3
                for endifs, same but just end so NULL */
            semiQuad *result;
            result = concatSemiQuad(ifQuad, $5);
            result = concatSemiQuad(result, endIfQuad);
            /*  if_semiquad bloc_semiquad endif_semiquad 
                the same is done for avery special case belove*/
            $$ = result;
        }
    | IF '(' EXPR ')' BLOC ELSE BLOC 
        {
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
	| WHILE '(' EXPR ')' BLOC            
        {
            int conditionVariable = newTemp().reference; 
            /* loops need to know which variable they are looping on */
            semiQuad *whileQuad = createSemiQuad(C2MP_QUAD_WHILE, 
                                                 conditionVariable, $3);
            semiQuad *endWhileQuad = createSemiQuad(C2MP_QUAD_ENDWHILE, 
                                                    conditionVariable, 
                                                    copyExpressionAST($3));
            semiQuad *result;
            result = concatSemiQuad(whileQuad, $5);
            result = concatSemiQuad(result, endWhileQuad);
            $$ = result;
        }
	| DO BLOC WHILE '(' EXPR ')' ';'     
        {
            int conditionVariable = newTemp().reference;
            semiQuad *doWhileQuad = createSemiQuad(C2MP_QUAD_DOWHILE,
                                                   conditionVariable,
                                                   $5);
            semiQuad *endDoWhileQuad = createSemiQuad(C2MP_QUAD_ENDDOWHILE,
                                                      conditionVariable,
                                                      copyExpressionAST($5));
            /*  Redo the condition's expression at every end of while bloc
                because condition here is only on a variable
                ex: while(var1<10) => 
                    t1 = var1 < 10 
                    while(t1)
                        ...(modification of var1)
                        t1 = var1 < 10
            */   
            semiQuad *result;
            result = concatSemiQuad(doWhileQuad, $2);
            result = concatSemiQuad(result, endDoWhileQuad);
            $$ = result;
        }
	| FOR '(' INSTRUCTION ';' EXPR ';' INSTRUCTION ')' BLOC  
        {
            int conditionVariable = newTemp().reference;
            semiQuad *whileQuad = createSemiQuad(C2MP_QUAD_WHILE,
                                                 conditionVariable,
                                                 $5);
            semiQuad *endWhileQuad = createSemiQuad(C2MP_QUAD_ENDWHILE,
                                                    conditionVariable,
                                                    copyExpressionAST($5));
            semiQuad *result;
            result = concatSemiQuad($3, whileQuad); // initialization + while
            result = concatSemiQuad(result, $9); // inside of the loop
            result = concatSemiQuad(result, $7); // "increment"
            result = concatSemiQuad(result, endWhileQuad); // endwhile
            $$ = result;
        }
	| INSTRUCTION ';' 
        { $$ = $1; }
	;

INSTRUCTION:
    ASSIGNMENT                                                   
        { $$ = $1; }
	|                                                              
        { $$ = NULL; }
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
	//| EXPR // TODO : while(1) empeche par ce commentaire. 
    le pb semble etre regle en dupliquant les boucles.
    le pb est encore plus regle en fusionnant expr et condition
	;*/

RVALUE:
	EXPR                    
        { $$ = $1; }
	;


ASSIGNMENT: 
    VAR '=' RVALUE          
        {
            $$ = createSemiQuad(C2MP_QUAD_ASSIGNMENT, $1.reference, $3);
        }
    | '(' ASSIGNMENT ')'
        { $$ = $2;}
    ;



NUMBER:
    INTEGER       
        { $$.type = 1; $$.valueInt = $1; }
    | FLOAT         
        { $$.type = 0; $$.valueFloat = $1; }
    ;

EXPR:
    EXPR '+' EXPR     
        { $$ = createExpressionAST(C2MP_OPERATOR_BINARY_PLUS, $1, $3); }
    | EXPR '*' EXPR     
        { $$ = createExpressionAST(C2MP_OPERATOR_BINARY_DOT, $1, $3); }
    | EXPR '-' EXPR     
        { $$ = createExpressionAST(C2MP_OPERATOR_BINARY_MINUS, $1, $3); }
    | EXPR '/' EXPR     
        { $$ = createExpressionAST(C2MP_OPERATOR_BINARY_DIVIDE, $1, $3); }
    | '-' EXPR          
        { $$ = createExpressionAST(C2MP_OPERATOR_UNARY_MINUS, $2, NULL); }
    | '+' EXPR          
        { $$ = createExpressionAST(C2MP_OPERATOR_UNARY_PLUS, $2, NULL); }
    | EXPR '<' EXPR     
        { $$ = createExpressionAST(C2MP_OPERATOR_LOWER_THAN, $1, $3); }
	| EXPR '>' EXPR     
        { $$ = createExpressionAST(C2MP_OPERATOR_GREATER_THAN, $1, $3); }
	| EXPR LTE EXPR     
        { $$ = createExpressionAST(C2MP_OPERATOR_LOWER_OR_EQUAL, $1, $3); }
	| EXPR GTE EXPR     
        { $$ = createExpressionAST(C2MP_OPERATOR_GREATER_OR_EQUAL, $1, $3); }
	| EXPR EQ EXPR      
        { $$ = createExpressionAST(C2MP_OPERATOR_EQUAL, $1, $3); }
	| EXPR NEQ EXPR     
        { $$ = createExpressionAST(C2MP_OPERATOR_NOT_EQUAL, $1, $3); }
    | EXPR OR EXPR      
        { $$ = createExpressionAST(C2MP_OPERATOR_LOGICAL_OR, $1, $3); }
    | EXPR AND EXPR     
        { $$ = createExpressionAST(C2MP_OPERATOR_LOGICAL_AND, $1, $3); }
    | EXPR '&' EXPR     
        { $$ = createExpressionAST(C2MP_OPERATOR_BITWISE_AND, $1, $3); }
    | EXPR '^' EXPR     
        { $$ = createExpressionAST(C2MP_OPERATOR_BITWISE_XOR, $1, $3); }
    | EXPR '|' EXPR     
        { $$ = createExpressionAST(C2MP_OPERATOR_BITWISE_OR, $1, $3); }
    | '!' EXPR          
        { $$ = createExpressionAST(C2MP_OPERATOR_LOGICAL_NOT, $2, NULL); }
    | '~' EXPR          
        { $$ = createExpressionAST(C2MP_OPERATOR_BITWISE_NOT, $2, NULL); }
    | '(' EXPR ')'      
        { $$ = $2; }
    | VAR               
        { $$ = createVariableAST($1.reference); }
    | NUMBER            
        {
            switch($1.type)
            {
                case C2MP_NUM_TYPE_FLOAT:
                    $$ = createFloatAST($1.valueFloat);
                    break;
                case C2MP_NUM_TYPE_INTEGER:
                    $$ = createIntAST($1.valueInt);
                    break;
                default:
                    fprintf(stderr, "Unknown number type %d\n", $1.type);
            }
        }
	| FCT               
        { $$ = $1; }
    ;

FCT:
    SYMBOL '(' PARAM ')'
      {
        int type;
        if ((type = parseFct($1)) == C2MP_FUNCTION_UNKNOWN)
        {
            printf("Unknown function %s\n", $1);
            $$ = createCustonFunctionAST($1, 1, $3);
        }
        else {
            $$ = createExpressionAST(type, $3, NULL);
        }
      }
    | SYMBOL '(' PARAM ',' PARAM ')'
      {
        int type;
        if ((type = parseFct($1)) == C2MP_FUNCTION_UNKNOWN)
        {
            printf("Unknown function %s\n", $1); 
            $$ = createCustonFunctionAST($1, 2, $3, $5);
        }
        else {
            $$ = createExpressionAST(type, $3, $5);
        }
      }
    | SYMBOL '(' PARAM ',' PARAM ARG ')' 
      { 
        printf("not supported function %s: over 2 arguments\n", $1); 
        // we still need to get the parameters from ARG !
        $$ = createCustonFunctionAST($1, 2, $3, $5); 
      }
    ;

VAR:
    SYMBOL            
        {
            $$.name = $1;
            $$.reference = getReferenceFromName($1);
        }
    ;

ARG:
      ',' PARAM ARG
    | 
    ;

PARAM:
      EXPR      
      { 
        $$ = $1;
      }
    | STRING
      {
        $$ = createStringAST($1);
      }
    ;

%%


int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        panic("syntax.y", "main", "Missing argument - usage : ./C2MP <file>.c -O");
    }
 
    int opt,
        i,
        errflag;
 
    char ch, 
        ret[50], 
        ret2[50];
    char * rc;

 
    opt         = 0;
    errflag     = 0;
    option_flag = 0;
    i           = 1;
    ch          = '.';

 
    pragmaOn        = 0;
    pragmaBlocOn    = 0;
    pragmaBlocIndex = 0;
 
    rc = strrchr(argv[1], ch);
    if (strncmp(rc, ".c", 2) != 0)
    {
        panic("syntax.y", "main", "Extension File Error");
    }
 
    finput = fopen(argv[1], "r");
    if(finput == NULL)
        panic("syntax.y", "main", "Error Open File\n");
    yyin = finput;
 
    /* Use getopt to handle option */
    while ( (opt = getopt(argc, argv, "O") ) != -1)
    {
        switch (opt) 
        {
            case 'O':
                            option_flag = 1;
                break;
            /* Character not recognized */
            case '?':
                errflag++;
                break;
        }
    }
 
    if (errflag)
    {
        panic("syntax.y", "main", "usage : ./C2MP <fichier> -o");
    }
 
    open_file();    
    yyout = output;

    while(pragmaMet == 0)
    {
        yyparse();
        if (pragmaMet == 1) // if we met a pragma
        {
            pragmaMet = 0; // reset 

            // close file actual
            if ( fclose(finput) != 0)
                panic("syntax.y", "main", "Error Close File\n");
            if ( fclose(output) != 0)
                panic("syntax.y", "main", "Error Close File\n");

            // create new output
            snprintf(ret, 10, "output%d.c", i); 
            snprintf(ret2, 10, "output%d.c", i-1); 

            // open new file
            output = fopen(ret, "w+");
            if(output == NULL)
                panic("syntax.y", "main", "Error Open File\n");
            if (i == 1)
            {
                finput = fopen("output0.c", "r");
                if(finput == NULL)
                    panic("syntax.y", "main", "Error Open File\n");
            }
            else
            {
                finput = fopen(ret2, "r");
                if(finput == NULL)
                    panic("syntax.y", "main", "Error Open File\n");
            }
            // new input : output with pragma actual
            yyin = finput;

            // new output
            yyout = output;

            i++;

        }
        else break;
    }

    printf("%d pragma in source. Final Result in file : result.c\n",i-1);
    if ( rename(ret, "result.c") == -1)
    {
        panic("syntax.y", "main", "Error Rename File\n");
    }

    if ( fclose(finput) != 0)
        panic("syntax.y", "main", "Error Close File\n");
 
    printf("End of parsing\n");
 
    close_file();
 
    return 0;
}
