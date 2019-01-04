#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "ast.h"

/*ast* ast_new_operation(char* op, ast* left, ast* right) 
{
	ast* new = malloc(sizeof(ast));
	new->type = strdup(op);
	new->u.operation.left  = left;
	new->u.operation.right = right;
	return new;
}

ast* ast_new_number(int number) 
{
	ast* new = malloc(sizeof(ast));
	new->type = strdup("number");
	new->u.number = number;
	return new;
}

ast* ast_new_id(char* id) 
{
	ast* new = malloc(sizeof(ast));
	new->type = strdup("id");
	new->u.id = strdup(id);
	return new;
}

void ast_print(ast* ast, int indent)
{
	for (int i = 0; i < indent; i++)
		printf("    ");
	printf("%s", ast->type);
	if (strcmp(ast->type, "number") == 0)
		printf(" (%d)\n", ast->u.number);
	else if (strcmp(ast->type, "id") == 0)
		printf(" (%s)\n", ast->u.id);
	else 
	{
		printf("\n");
		ast_print(ast->u.operation.left, indent + 1);
		ast_print(ast->u.operation.right, indent + 1);
	}
}*/

expressionAST *createExpressionAST(char operator, expressionAST *expr1, expressionAST *expr2)
{
    expressionAST *expr = malloc(sizeof(expressionAST));
    expr->operator = operator;
    expr->expression.e1 = expr1;
    expr->expression.e2 = expr2;
    return expr;
}

expressionAST *createCustonFunctionAST(char *name, int argNum, ...)
{
    expressionAST *expr = malloc(sizeof(expressionAST));
    expr->operator = C2MP_FUNCTION_UNKNOWN;

    expr->customFunction.name = strdup(name);
    expr->customFunction.argnum = argNum;

    va_list argList;
    va_start(argList, argNum);

    for (int i = 0; i < argNum; i++)
    {
        expr->customFunction.args[i] = va_arg(argList, expressionAST*);
    }

    va_end(argList);

    return expr;

}

expressionAST *createStringAST(const char *string)
{
    expressionAST *expr = malloc(sizeof(expressionAST));
    expr->operator = C2MP_CHARACTER_STRING;
    expr->valueString = strdup(string);
    return expr;
}

expressionAST *createIntAST(int integer)
{
    expressionAST *expr = malloc(sizeof(expressionAST));
    expr->operator = C2MP_CHARACTER_INTEGER;
    expr->valueInt = integer;
    return expr;
}

expressionAST *createFloatAST(float number)
{
    expressionAST *expr = malloc(sizeof(expressionAST));
    expr->operator = C2MP_CHARACTER_FLOAT;
    expr->valueFloat = number;
    return expr;
}

expressionAST *createVariableAST(int variable)
{
    expressionAST *expr = malloc(sizeof(expressionAST));
    expr->operator = C2MP_CHARACTER_VARIABLE;
    expr->valueVariable = variable;
    return expr;
}

expressionAST *copyExpressionAST(expressionAST *expressionAST)
{
    if(expressionAST == NULL)
    {
        return NULL;
    }

    struct expressionAST *currentAST = malloc(sizeof(struct expressionAST));
    currentAST->operator = expressionAST->operator;

    switch(expressionAST->operator)
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
            currentAST->expression.e1 = copyExpressionAST(expressionAST->expression.e1);
            currentAST->expression.e2 = copyExpressionAST(expressionAST->expression.e2);
            break;
        case C2MP_OPERATOR_UNARY_MINUS:
        case C2MP_OPERATOR_UNARY_PLUS:
        case C2MP_OPERATOR_LOGICAL_NOT:
        case C2MP_OPERATOR_BITWISE_NOT:
            currentAST->expression.e1 = copyExpressionAST(expressionAST->expression.e1);
            break;
        case C2MP_CHARACTER_INTEGER: // number
            currentAST->valueInt = expressionAST->valueInt;
            break;
        case C2MP_CHARACTER_FLOAT: // float
            currentAST->valueFloat = expressionAST->valueFloat;
            break;
        case C2MP_CHARACTER_VARIABLE: // variable
            currentAST->valueVariable = expressionAST->valueVariable;
            break;
        default:
            fprintf(stderr, "Warning, unknown expression operation : %c\n", expressionAST->operator);
    }

    return currentAST;
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
        case C2MP_FUNCTION_POW:
            printf("pow("); 
            printExpressionAST(expr->expression.e1);
            printf(", ");
            printExpressionAST(expr->expression.e2);
            printf(")");
            break;
        case C2MP_FUNCTION_SQRT:
            printf("sqrt(");
            printExpressionAST(expr->expression.e1);
            printf(")");
            break;
        case C2MP_FUNCTION_ABS:
            printf("abs(");
            printExpressionAST(expr->expression.e1);
            printf(")");
            break;
        case C2MP_FUNCTION_EXP:
            printf("exp(");
            printExpressionAST(expr->expression.e1);
            printf(")");
            break;
        case C2MP_FUNCTION_LOG:
            printf("log(");
            printExpressionAST(expr->expression.e1);
            printf(")");
            break;
        case C2MP_FUNCTION_LOG10:
            printf("log10(");
            printExpressionAST(expr->expression.e1);
            printf(")");
            break;
        case C2MP_FUNCTION_COS:
            printf("cos(");
            printExpressionAST(expr->expression.e1);
            printf(")");
            break;
        case C2MP_FUNCTION_COSH:
            printf("cosh(");
            printExpressionAST(expr->expression.e1);
            printf(")");
            break;
        case C2MP_FUNCTION_SIN:
            printf("sin(");
            printExpressionAST(expr->expression.e1);
            printf(")");
            break;
        case C2MP_FUNCTION_SINH:
            printf("sinh(");
            printExpressionAST(expr->expression.e1);
            printf(")");
            break;
        case C2MP_FUNCTION_UNKNOWN:
            printf("%s(", expr->customFunction.name);
            for (int i = 0; i < expr->customFunction.argnum; i++)
            {
                printExpressionAST(expr->customFunction.args[i]);
            }
            break;
        default:
            fprintf(stderr, "Warning, unknown expression operation : %c\n", expr->operator);
    }
}
