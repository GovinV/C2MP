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
 * @param rounding The rounding type given in the pragma
 */
void generateCode(quad* q, char *rounding);

#endif