#ifndef SYMBOL_H
#define SYMBOL_H

#include <stdbool.h>
#include "utils.h"
#include <string.h>

#define MAX 1024*8

typedef enum 
{
    FLOAT_NUMBER,
    INTEGER_NUMBER,
    STRING
} symbolType;

typedef struct symbol_s
{
    int reference;
    char *name;
    bool isConstant;
    symbolType type_symbol;
} symbol;

//#define SYMBOL_MAX_STRING 42
symbol newSymbol(const char name[]);
int getSymbolReference(const char name[]);
int getReferenceFromName(const char name[]);
const char *getNameFromReference(int reference);
symbol newTemp(void);
char * type_symbol(symbolType type);

/*
typedef enum 
{
    INTEGER,
    LABEL,
    STRING
} symbolType;

typedef struct symbol_s
{
    char *id;
    bool isconstant;
    int val;
    type type_symbol;
    struct symbol_s *next;
} Symbol;

Symbol *symbol_alloc(void);
void symbol_free(Symbol *);
Symbol *symbol_newtemp(Symbol **);
Symbol *symbol_lookup(Symbol *, char *);
Symbol *symbol_add(Symbol **, char *);
void symbol_print(Symbol *);*/

#endif // SYMBOL_H

