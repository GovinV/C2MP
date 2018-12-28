#include "quad.h"

#include <stdlib.h>
#include <stdio.h>

Quad *quad_init(char op, Symbol *arg1, Symbol *arg2, Symbol *res)
{
    Quad *new = (Quad *)malloc(sizeof(Quad));
    new->arg1 = arg1;
    new->arg2 = arg2;
    new->op = op;
    new->res = res;

    return new;
}

void quad_free(Quad *quad)
{
    free(quad);
}

void quad_add(Quad **dest, Quad *src)
{
    if(src != NULL)
    {
        Quad *current = dest[0];
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = src;
        src->next = NULL;
    }
}

void quad_print(Quad *quad)
{
    if(quad->arg2 != NULL) // if 2 args
    {
        printf("%s = %s(%d) %c %s(%d)\n",
            quad->res->id,
            quad->arg1->id,quad->arg1->val,
            quad->op,
            quad->arg2->id,quad->arg2->val
        );
    }
    else
    {
        printf("%s = %c %s(%d)\n",
            quad->res->id,
            quad->op,
            quad->arg1->id,quad->arg1->val
        );
    }
}