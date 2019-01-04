#include "utils.h"

/* sortie d'erreur */
void panic(char * file, char * function, char * error)
{
    fprintf(stderr, "%s.c : Unexpected error occurred - function '%s'\n", file, function);
    fprintf(stderr, "\t%s\n", error);
    exit(EXIT_FAILURE);
}

int checkExtension(char * s)
{
    if (strncmp(s, "precision", 9) == 0)
        return 1;
    if (strncmp(s, "rounding", 8) == 0)
        return 2;
    return ERROR;
}

int parseFct(char *symbol)
{
    /** 
     * This could be buggy: for example a function called "sqrthello" would
     * match with this implementation...
     */
    if (strncmp(symbol, "sqrt", 5) == 0)
        return C2MP_FUNCTION_SQRT;
    if (strncmp(symbol, "pow", 4) == 0)
        return C2MP_FUNCTION_POW;
    return UNKNOWN;
}