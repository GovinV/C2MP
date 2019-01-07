#ifndef OPTI_H
#define OPTI_H

/**
 * \file optimization
 * \brief Optimize Quad or Operation to reduce the generate code
 * \author DIVRIOTIS C. FLINT C. JUNG L. VETRIVEL G. 
 * \version 0.1
 * \date Janvier 2018
 *
 */
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

typedef struct referenceList
{
	int ref;
	struct referenceList *next;
} referenceList;

referenceList *addReference(referenceList *list, int ref);
referenceList *concatReferenceList(referenceList *list1, referenceList *list2);
bool referenceIsIn(int reference, referenceList *list);


quad* optimizeQuads(quad* quads);
quad* removeLoopsInvariants(quad* quads);
quad* removeLoopInvariants(quad* quads); // removes invariants from ONE loop, returns the LAST quad of the bloc
referenceList *getModifiedVariablesInBloc(quad* quads); // returns a list of reference which can be modified in this bloc
referenceList *getUsedVariablesInBloc(quad* quads);
quad* reuseTemporaries(quad* quads);
quad* replaceUntil(quad* begin, quad*end, int oldRef, int newRef);
quad* getSafeReuseQuad(quad* quads, int reference);
quad* removeUselessTemp(quad* quads);
quad* removeAllCommonSubExpressions(quad* quads);
quad* removeCommonSubExpression(quad* quads, quad* firstQuad);
quad* ignoreBlocForCommonSubExpression(quad* quads, quad* firstQuad);
void resetTables(void);

void optimizeExprToAssignment(quad* q, int reference);
void assignNewOptimizationRef(int reference);
void setOptimizationRef(int reference, int hashReference);
int getOperandOptimizationRef(int reference);
int createExprHash(char op, int operand1, int operand2);
int createRefTableRow(int reference);
int findOptimizationRefTable(int reference);
int findRefTable(int reference);
int findExprHashTable(char op, int operand1, int operand2);
int findConstantTable(int reference);
void resetTables(void);

#endif //OPTIMIZATION_H