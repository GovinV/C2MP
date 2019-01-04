#ifndef __GENERATE_H
#define __GENERATE_H

#include "utils.h"
#include "semiQuad.h"
#include "symbol.h"
#include "ast.h"

typedef struct quadOperand quadOperand;
typedef struct quad quad;

/**
 * @brief Generate MPC instructions
 * @param q The list of the quads parsed previously
 * @param rounding The rounding type given in the pragma (or default)
 * @param precision The precision specified int the pragma (or default)
 */
void generateCode(quad* q, char *rounding, int precision);

/**
 * @brief Generate the code necessary to initialize the temporary variables.
 * @param q This is the list of the quads generated from the parsing
 * @param precision This is the precision specified in the pragma (or default)
 * @return A table of booleans that allows us to know which temp is used.
 */
bool * generateInitCode(quad *q, int precision);

/**
 * @brief Generates the code necessary to clear temporary variables.
 * @param tempList List of the temporary variables generated and used.
 */ 
void generateClearCode(bool *tempList);

#endif