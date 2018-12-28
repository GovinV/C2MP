#include "symbol.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Symbol *symbol_alloc(void)
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
}*/

char *variables[MAX_VARIABLES];
int variablesSize = 0;

// if the variable is found, returns a reference, else -1
int getVariableReference(const char name[])
{
    for(int i=0;i<variablesSize;++i)
    {
        if(strcmp(name, variables[i]) == 0)
        {
            return i;
        }
    }
    return -1;
}

// if the variable is found, returns a reference, else creates one and returns it
int getReferenceFromName(const char name[])
{
    int reference = getVariableReference(name);
    if(reference == -1)
    {
        variables[variablesSize] = strdup(name);
        return variablesSize++;
    }

    return reference;
}

// returns the name of the variable
const char *getNameFromReference(int reference)
{
    return variables[reference];
}



