#ifndef SEMIQUAD_H
#define SEMIQUAD_H

#include "utils.h"
#include <string.h>
#include "../build/y.tab.h"
#include "ast.h"
#include "symbol.h"

typedef struct semiQuad semiQuad;

semiQuad *createSemiQuad(char operator, int assignment, expressionAST *expression);
semiQuad *concatSemiQuad(semiQuad *q1, semiQuad *q2);
void printSemiQuads(semiQuad *q1);

/**
 * @brief Frees the memory of the list of semi quads specified
 * @param sq A list of semiQuads
 */
void freeSemiQuad(semiQuad *sq);

#endif // SEMIQUAD_H

