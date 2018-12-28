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

symbol variables[MAX_VARIABLES];
int variablesSize = 0;

symbol newSymbol(const char name[])
{
    variables[variablesSize].name = strdup(name);
    variables[variablesSize].reference = variablesSize;
    variables[variablesSize].isConstant = 0;

    return variables[variablesSize++];
}

// if the variable is found, returns a reference, else -1
int getSymbolReference(const char name[])
{
    for(int i=0;i<variablesSize;++i)
    {
        if(strcmp(name, variables[i].name) == 0)
        {
            return i;
        }
    }
    return -1;
}

// if the variable is found, returns a reference, else creates one and returns it
int getReferenceFromName(const char name[])
{
    int reference = getSymbolReference(name);
    if(reference == -1)
    {
        return newSymbol(name).reference;
    }

    return reference;
}

// returns the name of the variable
const char *getNameFromReference(int reference)
{
    return variables[reference].name;
}

symbol newTemp(void)
{
    char buffer[1024];
    static int tempCount = 0;
    snprintf(buffer, 1024, "C2MP___temp_%d",tempCount);

    ++tempCount;

    return newSymbol(buffer);
}



