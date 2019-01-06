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

quad *quads;    // the list of the quads we will create

expressionAST *copyExpressionAST(expressionAST *expressionAST);

quadOperand createVariableOperand(int reference);
quadOperand createIntegerOperand(int value);
quadOperand createFloatOperand(float value);
quadOperand createStringOperand(char *string);

quad *createQuad(int assignment, char operator, char * name, int operandsNum, ...);
quad *copySemiQuad(semiQuad *sq);
quad *generateQuadsFromAST(expressionAST *expr);
quad *getQuadsFromSemiQuads(semiQuad *sq);
quad *concatQuads(quad *q1, quad *q2);
void printOperand(quadOperand operand);
void printQuads(quad* q);
void freeQuads(quad *q);

expressionAST *createExpressionAST(char operator, expressionAST *expr1, expressionAST *expr2);
expressionAST *createCustomFunctionAST(char *name, int argNum, struct expressionAST **list);
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

symbol newTemp(symbolType type);

// generate.h
void generateCode(quad* q, char *rounding, int precision);

// optimization.h
quad* optimizeQuads(quad* quads);
quad* removeCommonSubExpressions(quad* quads);
quad* removeAllCommonSubExpressions(quad* quads);
quad* removeUselessTemp(quad* quads);


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

    struct{
        int argsNum;
        struct expressionAST *tabAST[MAX_FCT_ARGS]; 
    } argumentsAST; // for functions arguments
	
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
            char *valueString;
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

%type <argumentsAST>    ARG
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
axiom : 
     /* empty */
     | P_PRAGMA
     ;

P_PRAGMA:
	PRAGMA P_EXTENSION BACKSLASH BLOC
        {
            if (option_verbose)
            {
                printf("***** Found Pragma *****\n");
                printf("Rounding = %s\n", $2.rounding);
                printf("Precision = %d\n", $2.precision);
            }
            if (option_print_semiquads)
            {
                printf("SemiQuads generation...\n");
                printSemiQuads($4);
                printf("End of semiQuads generation.\n");
            }
            quads = getQuadsFromSemiQuads($4);
            if (option_print_quads)
            {
                printf("Quads generation...\n");
                printQuads(quads);
                printf("End of quads generation.\n");
            }
            if (option_flag)
            {
                printf("Optimization:\n");
                quads = optimizeQuads(quads);
                printf("End of Optimization.\n");
            }
            generateCode(quads, $2.rounding, $2.precision);
            // some memory frees
            freeQuads(quads);
            free($2.rounding); // strdup of rounding
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
            int conditionVariable = newTemp(INTEGER_NUMBER).reference; 
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
            int conditionVariable = newTemp(INTEGER_NUMBER).reference;
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
            int conditionVariable = newTemp(INTEGER_NUMBER).reference;
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
    SYMBOL '(' PARAM ARG ')'
    {
        int type;
        if ((type = parseFct($1)) == C2MP_FUNCTION_UNKNOWN)
        {
            // we need to add the first param to the tab of ARG
            $4.argsNum++;
            $4.tabAST[$4.argsNum-1] = $3;
            $$ = createCustomFunctionAST($1, $4.argsNum, $4.tabAST);
        }
        else {
            // known function (we use a switch to avoid creating a new function)
            switch (type)
            {
                // binary functions
                case C2MP_FUNCTION_POW:
                    $$ = createExpressionAST(type, $3, $4.tabAST[0]);
                    break;
                // unary functions
                case C2MP_FUNCTION_SQRT:
                //case C2MP_FUNCTION_ABS:
                case C2MP_FUNCTION_EXP:
                case C2MP_FUNCTION_LOG:
                case C2MP_FUNCTION_LOG10:
                case C2MP_FUNCTION_COS:
                case C2MP_FUNCTION_SIN:
                case C2MP_FUNCTION_COSH:
                case C2MP_FUNCTION_SINH:
                    $$ = createExpressionAST(type, $3, NULL);
                    break;
                // this should not happen...
                default:
                    panic("syntax.y", "parsing", "recognized unknown fct?!");
            }
        }
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
      {
          $$ = $3;
          $$.argsNum = $$.argsNum + 1;
          $$.tabAST[$$.argsNum-1] = $2;
      }
    | 
      { 
          $$.argsNum = 0;
      }
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
    char *resultFileName = strdup("result.c");

 
    option_print_quads = 0;
    option_print_semiquads = 0;
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
    while ( (opt = getopt(argc, argv, "Oo:aqsv") ) != -1)
    {
        switch (opt) 
        {
            // apply optimization
            case 'O':
                option_flag = 1;
                break;
            // naming resulting file
            case 'o':
                free(resultFileName);
                resultFileName = strdup(optarg);
                break;
            // print quads
            case 'q':
                option_print_quads = 1;
                break;
            // print semiquads
            case 's':
                option_print_semiquads = 1;
                break;
            // maximal verbose
            case 'v':
                option_verbose = 1;
                option_print_quads = 1;
                option_print_semiquads = 1;
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

    if ( rename(ret, resultFileName) == -1)
    {
        panic("syntax.y", "main", "Error Rename File\n");
    }

    if ( fclose(finput) != 0)
        panic("syntax.y", "main", "Error Close File\n");
 
    if (option_verbose)
    {
        printf("\n%d pragma in source. Final Result in file : result.c\n",i-1);
        printf("End of parsing\n");
    }

    free(resultFileName);    
    close_file();
 
    return 0;
}
