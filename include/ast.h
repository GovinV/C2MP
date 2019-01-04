#ifndef AST_H_DEFINED
#define AST_H_DEFINED

#include "utils.h"
#include <string.h>
#include "../build/y.tab.h"
#include "symbol.h"

/*
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
                char *valueString;
                int argnum;
                struct expressionAST *args[MAX_FCT_ARGS];
            } customFunction;
        };
	} *expressionAST;*/

typedef struct expressionAST expressionAST;
typedef struct argType argType;	// forward declaration

expressionAST *createExpressionAST(char operator, expressionAST *expr1, expressionAST *expr2);
expressionAST *createIntAST(int integer);
expressionAST *createFloatAST(float number);
expressionAST *createVariableAST(int variable);

/**
 * @brief Create 
 * @param string
 * @return The created string branch
 */
expressionAST *createStringAST(const char *string);

expressionAST *copyExpressionAST(expressionAST *expressionAST);
void freeExpressionAST(expressionAST *expr);
void printExpressionAST(expressionAST *expr);

/**
 * @brief 
 * @param name
 * @param argNum
 * @param ... 
 * @return The AST expression created
 */
expressionAST *createCustonFunctionAST(char *name, int argNum, ...);


#endif // AST_H_DEFINED

