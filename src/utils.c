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