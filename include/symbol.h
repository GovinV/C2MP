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
    MPC_T
} symbolType;

typedef struct symbol_s
{
    int reference;
    char *name;
    bool isConstant;
    bool isTemp;
    symbolType type_symbol;
} symbol;

//#define SYMBOL_MAX_STRING 42
symbol newSymbol(const char name[], bool isTemp);
int getSymbolReference(const char name[]);
int getReferenceFromName(const char name[]);
const char *getNameFromReference(int reference);

/**
 * @brief Generate a new temporary variable
 * @return The symbol associated with the variable generated
 */
symbol newTemp(void);

/**
 * @brief This function returns the symbol from the symbol table at the index
 * given as parameter.
 * @param ref Index in the symbol table
 * @return The symbol at the index specified as a parameter
 */
symbol getSymbolFromReference(int ref);

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

