#ifndef QUAD_H
#define QUAD_H

#include "../build/y.tab.h"
#include "utils.h"
#include "semiQuad.h"
#include "symbol.h"
#include "ast.h"

#define C2MP_QUAD_OPERAND_VARIABLE 0
#define C2MP_QUAD_OPERAND_INTEGER 1
#define C2MP_QUAD_OPERAND_FLOAT 2

typedef struct quadOperand quadOperand;
typedef struct quad quad;

quadOperand createVariableOperand(int reference);
quadOperand createIntegerOperand(int value);
quadOperand createFloatOperand(float value);
quadOperand createVoidOperand(void);

quad *createQuad(int assignment, char operator, quadOperand value1, quadOperand value2);
quad *copySemiQuad(semiQuad *sq);
quad *generateQuadsFromAST_2(expressionAST *expr);
quad *generateQuadsFromAST(expressionAST *expr);
quad *getQuadFromSemiQuad(semiQuad *sq);
quad *concatQuads(quad *q1, quad *q2);
void printOperand(quadOperand operand);
void printQuads(quad* q);

/*typedef enum 
{
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_LE,
    OP_LTE,
    OP_EQ,
    OP_NEQ,
    OP_GE,
    OP_GTE,
    OP_ASSIGN
    //OP_LABEL
} operation;

typedef struct quad_s
{
    operation op;
    Symbol* arg1;
    Symbol* arg2;
    Symbol* res;
    struct quad_s *next;
} Quad;

Quad* quad_init(char * op, Symbol* arg1, Symbol* arg2, Symbol* res);
operation quad_operation(char* op);
void quad_free(Quad *quad);
void quad_add(Quad **dest, Quad *src);
void quad_print(Quad *quad);*/

#endif
