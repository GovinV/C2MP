#ifndef SYMBOL_H
#define SYMBOL_H

#include <stdbool.h>

#include "utils.h"

#define MAX_VARIABLES 1024*8

//#define SYMBOL_MAX_STRING 42

int getVariableReference(const char name[]);
int getReferenceFromName(const char name[]);
const char *getNameFromReference(int reference);

/*typedef enum 
{
    INTEGER,
    LABEL,
    STRING
} type;

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

