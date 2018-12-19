#ifndef __UTILS_C
#define __UTILS_C

#define ERROR      -1


/**
 * Est-ce que ces macros ne seraient pas générés avec yacc?
 * Comment les choper depuis yacc?
 */
enum {UNKNOWN, SQRTF, POWF, SINF};

/**
 * @brief Cette structure décrit une extension pour un pragma. Contient le nom
 * de l'extension (precision ou rounding) ainsi que la valeur, soit un int soit
 * une chaine de caractère.
 */


/**
 * @brief Vérifie l'existance de l'extension du pragma entrée en paramètre.
 * @return Renvoie PRECISION si c'est une extension precision, ROUNDING si
 * c'est une extension rounding, ERROR (-1) sinon.
 */
int checkExtension(char *);

/**
 * @brief Vérifie si le symbole en argument représente une fonction mathématique
 * connue que l'on peut traduire pour MPC.
 */
int parseFct(char *);

#endif