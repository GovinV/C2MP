#ifndef OPTI_H
#define OPTI_H

#include "utils.h"
#include <string.h>
#include "quad.h"
#include "symbol.h"
#include "mpc.h"



// Number table
// Expression Hash table
// Name Table (Constant Table)

typedef struct referenceRow
{
	int reference;
	int optimizationRef;
} refRow;

typedef struct expressionHashRow
{
	char operator;
	int operand1;
	int operand2;
	int optimizationRef;
} exprHashRow;

typedef struct constantRow
{
	int reference;
	mpc_t constant;
	int flag;
} constRow;

quad* optimizeQuad(quad* quads);
quad* removeCommonSubExpressions(quad* quads);
void resetTables(void);

int findOptimizationRefTable(int reference);
int findRefTable(int reference);
int findExprHashTable(char op, int operand1, int operand2);
int findConstantTable(int reference);
void resetTables(void);

#endif //OPTI_H