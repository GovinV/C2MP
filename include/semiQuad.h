#ifndef SEMIQUAD_H
#define SEMIQUAD_H

#include <string.h>

#include "utils.h"
#include "../build/y.tab.h"
#include "ast.h"
#include "symbol.h"

typedef struct semiQuad semiQuad;

/**
 * @brief Creates a single semiQuad (list of one element)
 * @param operator operation done by the semiQuad
 * @param assignment variable to which the operation will be affected
 * @param expression expression affected to the variable
 * @return corresponding single semiQuad
 */
semiQuad *createSemiQuad(char operator, int assignment, expressionAST *expression);
/**
 * @brief Concatenates two lists of semiQuads
 * (careful ! The two lists are modified by this operation)
 * @param q1 first list
 * @param q2 second list
 * @return concatenated lists
 */
semiQuad *concatSemiQuad(semiQuad *q1, semiQuad *q2);
/**
 * @brief Frees the memory of the specified list of semi quads
 * @param sq list of semiQuads, which need to be freed
 */
void freeSemiQuad(semiQuad *sq);
/**
 * @brief Prints semiQuads to the standard output
 * @param q list of semiQuads, which need to be printed
 */
void printSemiQuads(semiQuad *q);

#endif // SEMIQUAD_H

