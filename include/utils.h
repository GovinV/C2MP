#ifndef __UTILS_C
#define __UTILS_C

#define ERROR      -1
#define PRECISION   1
#define ROUNDING    2

/**
 * @brief Cette structure décrit une extension pour un pragma. Contient le nom
 * de l'extension (precision ou rounding) ainsi que la valeur, soit un int soit
 * une chaine de caractère.
 */
struct pragmaExt 
{
    char * extension;
    union 
    {
        int precision;
        char * rounding;
    };
};

/**
 * @brief Vérifie l'existance de l'extension du pragma entrée en paramètre.
 * @return Renvoie PRECISION si c'est une extension precision, ROUNDING si
 * c'est une extension rounding, ERROR (-1) sinon.
 */
int checkExtension(char *);

#endif