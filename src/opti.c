#include "opti.h"

refRow refTable[MAX];
int refSize = 0;

exprHashRow exprHashTable[MAX];
int exprHashSize = 0;

constRow constTable[MAX];
int constSize = 0;

quad* optimizeQuad(quad* quads);

quad* removeCommonSubExpressions(quad* quads)
{
	
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
	for(i = 0; i < exprHashSize;i++)
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
	for(i = 0; i < constSize;i++)
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
	exprHashSize = 0;
	constSize = 0;
}