#ifndef __UTILS_C
#define __UTILS_C

/*
list of operators :
< > : binary
i s : <= (i) and >= (s)
e E : binary == (e) and != (E)
+ - * / : binary
| : bitwise or
& : bitwise and
^ : xor
O : logical or
A : logical and


m : unary -
p : unary +
! : logical not
~ : bitwise not
*/

#define C2MP_NUM_TYPE_FLOAT 0
#define C2MP_NUM_TYPE_INTEGER 1

#define C2MP_OPERATOR_BINARY_PLUS       '+'
#define C2MP_OPERATOR_BINARY_MINUS      '-'
#define C2MP_OPERATOR_BINARY_DOT        '*'
#define C2MP_OPERATOR_BINARY_DIVIDE     '/'

#define C2MP_OPERATOR_UNARY_PLUS        'p'
#define C2MP_OPERATOR_UNARY_MINUS       'm'

#define C2MP_OPERATOR_LOWER_THAN        '<'
#define C2MP_OPERATOR_GREATER_THAN      '>'
#define C2MP_OPERATOR_LOWER_OR_EQUAL    'i'
#define C2MP_OPERATOR_GREATER_OR_EQUAL  's'
#define C2MP_OPERATOR_EQUAL             'e'
#define C2MP_OPERATOR_NOT_EQUAL         'E'

#define C2MP_OPERATOR_LOGICAL_AND       'A'
#define C2MP_OPERATOR_LOGICAL_OR        'O'
#define C2MP_OPERATOR_LOGICAL_NOT       '!'
#define C2MP_OPERATOR_BITWISE_AND       '&'
#define C2MP_OPERATOR_BITWISE_OR        '|'
#define C2MP_OPERATOR_BITWISE_XOR       '^'
#define C2MP_OPERATOR_BITWISE_NOT       '~'

#define C2MP_CHARACTER_INTEGER          'n'
#define C2MP_CHARACTER_FLOAT            'f'
#define C2MP_CHARACTER_VARIABLE         'v'
#define C2MP_CHARACTER_STRING           'S'



#define C2MP_QUAD_ASSIGNMENT            '='
#define C2MP_QUAD_IF                    'I'
#define C2MP_QUAD_ELSE                  'z'
#define C2MP_QUAD_ENDIF                 '}'
#define C2MP_QUAD_WHILE                 'W'
#define C2MP_QUAD_ENDWHILE              'w'
#define C2MP_QUAD_DOWHILE               'D'
#define C2MP_QUAD_ENDDOWHILE            'd'

// Known functions
#define C2MP_FUNCTION_POW               1
#define C2MP_FUNCTION_SQRT              2
#define C2MP_FUNCTION_ABS               3
#define C2MP_FUNCTION_EXP               4
#define C2MP_FUNCTION_LOG               5
#define C2MP_FUNCTION_LOG10             6
#define C2MP_FUNCTION_COS               7
#define C2MP_FUNCTION_SIN               8
#define C2MP_FUNCTION_COSH              9
#define C2MP_FUNCTION_SINH              10
#define C2MP_FUNCTION_UNKNOWN           -1

// max args for a function
#define MAX_FCT_ARGS 10

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define ERROR      -1


int pragmaOn;
int pragmaBlocOn;
int pragmaBlocIndex;
int option_flag;
int pragmaMet;
FILE * output;
FILE * finput;

void panic(char *, char * , char *);

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

int open_file(void);
int close_file(void);
int write_file(const char * expr);
int open_file2(char * name);
#endif // __UTILS_C
