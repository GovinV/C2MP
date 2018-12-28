#ifndef QUAD_H
#define QUAD_H

#include "utils.h"
#include "symbol.h"

typedef enum 
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
void quad_print(Quad *quad);

#endif