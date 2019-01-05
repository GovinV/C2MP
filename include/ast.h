#ifndef AST_H_DEFINED
#define AST_H_DEFINED

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "utils.h"
#include "../build/y.tab.h"
#include "symbol.h"

typedef struct expressionAST expressionAST;
typedef struct argType argType;	// forward declaration

/**
 * @brief Creates an AST containing an expression (example : a+b is an expression)
 * @param operator operation of the expression
 * @param operator expr1 operand 1
 * @param operator expr2 operand 2 (if not unary)
 * @return The created expressionAST
 */
expressionAST *createExpressionAST(char operator, expressionAST *expr1, expressionAST *expr2);
/**
 * @brief Creates an AST containing an integer (leaf)
 * @param integer value of the integer
 * @return The created expressionAST
 */
expressionAST *createIntAST(int integer);
/**
 * @brief Creates an AST containing a float (leaf)
 * @param number value of the float
 * @return The created expressionAST
 */
expressionAST *createFloatAST(float number);
/**
 * @brief Creates an AST containing a variable (leaf)
 * @param variable reference of the variable (int)
 * @return The created expressionAST
 */
expressionAST *createVariableAST(int variable);
/**
 * @brief Creates an AST containing a string (leaf)
 * @param string value of the string
 * @return The created expressionAST
 */
expressionAST *createStringAST(const char *string);
/**
 * @brief Creates an AST containing a custom function
 * @param name name of the custom function that needs to be called
 * @param argNum arity of the custom function (number of parameters)
 * @param list parameter list of the custom function
 * @return The created AST expression
 */
expressionAST *createCustomFunctionAST(char *name, int argNum, struct expressionAST **list);


/**
 * @brief Creates an expressionAST by copy of an other expressionAST
 * @param expressionAST expressionAST to be copied
 * @return Copy of the given expressionAST
 */
expressionAST *copyExpressionAST(expressionAST *expressionAST);
/**
 * @brief Frees an expressionAST
 * @param expr expressionAST to be freed
 */
void freeExpressionAST(expressionAST *expr);
/**
 * @brief Prints an expressionAST to the standard output
 * @param expr expressionAST to be printed
 */
void printExpressionAST(expressionAST *expr);


#endif // AST_H_DEFINED

