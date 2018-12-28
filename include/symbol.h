#ifndef SYMBOL_H
#define SYMBOL_H

#include <stdbool.h>

#define SYMBOL_MAX_STRING 42

typedef enum {
    INTEGER,
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

Symbol *symbol_alloc();
void symbol_free(Symbol *);
Symbol *symbol_newtemp(Symbol **);
Symbol *symbol_lookup(Symbol *, char *);
Symbol *symbol_add(Symbol **, char *);
void symbol_print(Symbol *);

#endif