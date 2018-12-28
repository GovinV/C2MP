#ifndef QUAD_H
#define QUAD_H

#include "symbol.h"

typedef struct quad_s
{
    char op;
    Symbol* arg1;
    Symbol* arg2;
    Symbol* res;
    struct quad_s *next;
} Quad;

Quad* quad_init(char op, Symbol* arg1, Symbol* arg2, Symbol* res);
void quad_free(Quad *quad);
void quad_add(Quad **dest, Quad *src);
void quad_print(Quad *quad);

#endif