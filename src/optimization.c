#include "optimization.h"

refRow refTable[MAX];
int refSize = 0;
int refCount = 0;

exprHashRow exprHashTable[MAX];
int exprHashSize = 0;

constRow constTable[MAX];
int constSize = 0;

quad* optimizeQuad(quad* quads);

quad* removeCommonSubExpressions(quad* quads)
{
	if(quads == NULL)
    {
        printf("no quads for optimization\n");
    }
	
	quad *firstQuad = quads;
	quad* q = quads;

	do
	{
		/*ignoring quad which aren't expr or assignment,
		 which means with voidOperand*/
		/*Assignment*/
		if(q->operator == C2MP_QUAD_ASSIGNMENT)
		{
			printf("wshA\n");

			int refOp1;
			if((refOp1=findRefTable(q->operand1.reference)) < 0)
			{
				refOp1=createRefTableRow(q->operand1.reference);
				refTable[refOp1].optimizationRef = refCount;
				refCount++;
			}

			int refTableIndex;
			if((refTableIndex = findRefTable(q->assignment)) < 0)
			{
				refTableIndex=createRefTableRow(q->assignment);
			}
			refTable[refTableIndex].optimizationRef = 
				refTable[refOp1].optimizationRef; 

		}
		/*expression*/
		if(q->operand1.type != -1 && q->operand2.type != -1)
		{
			printf("wsh\n");
			/*  Create entry in refTable if reference don't exist
				Example a = x + y;
				have to create the entry for x and y before
				*/
			int refOp1, refOp2;
			if((refOp1=findRefTable(q->operand1.reference)) < 0)
			{
				refOp1=createRefTableRow(q->operand1.reference);
				refTable[refOp1].optimizationRef = refCount;
				refCount++;
			}
			if((refOp2=findRefTable(q->operand2.reference)) < 0)
			{
				refOp2=createRefTableRow(q->operand2.reference);
				refTable[refOp2].optimizationRef = refCount;
				refCount++;
			}

			int refTableIndex;
			if((refTableIndex = findRefTable(q->assignment)) < 0)
			{
				refTableIndex=createRefTableRow(q->assignment);
			}
			
			int optimizationRefOp1 = refTable[refOp1].optimizationRef;
			int optimizationRefOp2 = refTable[refOp2].optimizationRef;

			/*handle commutativity*/
			if(optimizationRefOp2 < optimizationRefOp1)
			{
				int temp = optimizationRefOp1;
				optimizationRefOp1 = optimizationRefOp2;
				optimizationRefOp2 = temp;
			}

			int exprHashTableIndex;
			if((exprHashTableIndex = findExprHashTable(q->operator,
										optimizationRefOp1,
										optimizationRefOp2)) < 0)
			{
				exprHashTable[exprHashSize].operator = q->operator;
				exprHashTable[exprHashSize].operand1 = optimizationRefOp1;
				exprHashTable[exprHashSize].operand2 = optimizationRefOp2;
				exprHashTable[exprHashSize].optimizationRef = refCount;
				exprHashTableIndex = exprHashSize;
				refCount++;
				exprHashSize++;
			}

			refTable[refTableIndex].optimizationRef = 
				exprHashTable[exprHashTableIndex].optimizationRef;

			int optimizationRefUsed;
			if((optimizationRefUsed = 
					findOptimizationRefTable(refTable[refTableIndex].optimizationRef)) >= 0)
			{
				if(optimizationRefUsed != refTableIndex)
				{
					q->operator = C2MP_QUAD_ASSIGNMENT;
					q->operand1.type = C2MP_QUAD_OPERAND_VARIABLE;
					q->operand1.reference = refTable[optimizationRefUsed].reference;
					q->operand2.type = -1;
				}
			}			
		}
		q = q->next;
	} while(q != firstQuad);

	return quads;
}

int createRefTableRow(int reference)
{
	refTable[refSize].reference = reference;
	int refTableIndex = refSize;
	refSize++;
	return refTableIndex;
}

int findOptimizationRefTable(int reference)
{
	int i = 0;
	for(i = 0; i < refSize;i++)
	{
		if(refTable[i].optimizationRef == reference)
		{
			return i;
		}
	}
	return -1;
}

int findRefTable(int reference)
{
	int i = 0;
	for(i = 0; i < refSize;i++)
	{
		if(refTable[i].reference == reference)
		{
			return i;
		}
	}
	return -1;
}

int findExprHashTable(char op, int operand1, int operand2)
{
	int i = 0;
	for(i = 0; i < exprHashSize;++i)
	{
		if(   exprHashTable[i].operator == op 
		   && exprHashTable[i].operand1 == operand1
		   && exprHashTable[i].operand2 == operand2
		  )
		{
			return i;
		}
	}
	return -1;
}

int findConstantTable(int reference)
{
	int i = 0;
	for(i = 0; i < constSize;++i)
	{
		if(constTable[i].reference == reference)
		{
			return i;
		}
	}
	return -1;
}

void resetTables(void)
{
	refSize = 0;
	refCount = 0;
	exprHashSize = 0;
	constSize = 0;
}