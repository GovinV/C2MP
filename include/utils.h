#ifndef __UTILS_C
#define __UTILS_C

#define ERROR      -1
#define PRECISION   1
#define ROUNDING    2

struct pragma 
{
    int precision;
    const char * rounding;
};

/**
 * @brief Vérifie l'existance de l'extension du pragma entrée en paramètre.
 * @return Renvoie PRECISION si c'est une extension precision, ROUNDING si
 * c'est une extension rounding, ERROR (-1) sinon.
 */
int checkExtension(char *);

#endif