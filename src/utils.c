#include <string.h>
#include "utils.h"

int checkExtension(char * s)
{
    if (strncmp(s, "precision", 9) == 0)
        return PRECISION;
    if (strncmp(s, "rounding", 8) == 0)
        return ROUNDING;
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