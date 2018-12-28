#include "symbol.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Symbol *symbol_alloc(void)
{
    Symbol *new = (Symbol*)malloc(sizeof(Symbol));
    if (new == NULL)
        panic("symbol.c", "symbol_alloc", "malloc error");
    return new;
}

void symbol_free(Symbol *sym)
{
    if(sym == NULL)
    {
        panic("symbol.c", "symbol_free", "argument NULL");
    }

    Symbol * tmp;
    tmp = sym;

    while(tmp != NULL)
    {
        sym = sym->next;
        tmp->next = NULL;
        free(tmp);
    }
}

Symbol *symbol_newtemp(Symbol **TS)
{
    char buffer[1024];
    static int cptTemp = 0;
    snprintf(buffer,1024,"temp_%d",cptTemp);

    return symbol_add(TS,buffer);
}

Symbol *symbol_lookup(Symbol *TS, char *name)
{
    while(TS)
    {
        if(strcmp(name,TS->id)==0)
        {
            return TS;
        }
        TS = TS->next;
    }
    return NULL;
}

Symbol *symbol_add(Symbol **TS, char *name)
{
    Symbol *new = symbol_alloc();
    new->id = strdup(name);
    if(*TS == NULL)
    {
        *TS = new;
        return *TS;
    }
    else
    {
        Symbol *current = *TS;
        while(current->next != NULL)
        {
            current = current->next;
        }
        current->next = new;
    }
    return *TS;
}

void symbol_print(Symbol *sym)
{
    if(sym->type_symbol == INTEGER)
        printf("%d\t%s\n", sym->val, (sym->isconstant == true ? "constante" : "variable"));
    else 
        printf("%s\t%s\n", sym->id, (sym->isconstant == true ? "constante" : "variable"));
}