#ifndef SEMIQUAD_H
#define SEMIQUAD_H

#include "../build/y.tab.h"
#include "utils.h"
#include "ast.h"
#include "symbol.h"

typedef struct semiQuad semiQuad;

semiQuad *createSemiQuad(char operator, int assignment, expressionAST *expression);
semiQuad *concatSemiQuad(semiQuad *q1, semiQuad *q2);
void printSemiQuads(semiQuad *q1);

#endif // SEMIQUAD_H

