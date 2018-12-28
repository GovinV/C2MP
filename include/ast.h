#ifndef AST_H_DEFINED
#define AST_H_DEFINED

#include "../build/y.tab.h"
#include "utils.h"

typedef struct expressionAST expressionAST;

/*typedef struct ast 
{
	char * type;
	union 
	{
    	struct 
		{
      		struct ast* left;
      		struct ast* right;
    	} operation;
    	int number;
    	char * id;
  	} u;
} ast;

ast* ast_new_operation(char*, ast*, ast*);
ast* ast_new_number(int);
ast* ast_new_id(char*);
void ast_print(ast*, int);*/

expressionAST *createExpressionAST(char operator, expressionAST *expr1, expressionAST *expr2);
expressionAST *createIntAST(int integer);
expressionAST *createFloatAST(float number);
expressionAST *createVariableAST(int variable);
void freeExpressionAST(expressionAST *expr);
void printExpressionAST(expressionAST *expr);

#endif // AST_H_DEFINED

