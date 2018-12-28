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
    if (strncmp(symbol, "sqrtf", 5) == 0)
        return SQRTF;
    if (strncmp(symbol, "powf", 4) == 0)
        return POWF;
    if (strncmp(symbol, "sinf", 4) == 0)
        return SINF;
    return UNKNOWN;
}