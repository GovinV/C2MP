#include "optimization.h"

refRow refTable[MAX];
int refSize = 0;
int refCount = 0;

exprHashRow exprHashTable[MAX];
int exprHashSize = 0;

constRow constTable[MAX];
int constSize = 0;

quad* optimizeQuad(quad* quads);
/*
	fun remove from reftable in 


*/


quad* removeAllCommonSubExpressions(quad* quads)
{
    quad *firstQuad = quads;
    quad* q = quads;

    q = removeCommonSubExpression(q, firstQuad);

    do
    {
        switch(q->operator)
        {
            case C2MP_QUAD_IF:
            case C2MP_QUAD_WHILE:
            case C2MP_QUAD_DOWHILE:
                q = removeCommonSubExpression(q->next, firstQuad);
                break;
            default:
                break;
        }
        q=q->next;
    }while(q != firstQuad);

    return firstQuad;
}


quad* removeCommonSubExpression(quad* quads, quad* firstQuad)
{
	if(quads == NULL)
    {
        printf("no quads for optimization\n");
    }
	
    resetTables();
	quad* q = quads;
	int optimizationRefOp1;
	int optimizationRefOp2;
	int isCommutative;
	int refTableIndex;
	int operandOptimizationRef;
	
	do
	{
		/*ignoring quad which aren't expr or assignment,
		 which means with voidOperand*/
		optimizationRefOp1 = -1;
		optimizationRefOp2 = -1;
		isCommutative = 0;
		switch(q->operator)
		{
			case C2MP_QUAD_ASSIGNMENT:
				operandOptimizationRef = getOperandOptimizationRef(q->operands[0].reference);

				
				if((refTableIndex = findRefTable(q->assignment)) < 0)
				{
					refTableIndex=createRefTableRow(q->assignment);
				}

				refTable[refTableIndex].optimizationRef = 
					operandOptimizationRef;
				break;



            case C2MP_OPERATOR_BINARY_PLUS:
            case C2MP_OPERATOR_BINARY_DOT:
            case C2MP_OPERATOR_EQUAL:
            case C2MP_OPERATOR_NOT_EQUAL:
            case C2MP_OPERATOR_BITWISE_AND:
            case C2MP_OPERATOR_BITWISE_OR:
            case C2MP_OPERATOR_BITWISE_XOR:
            case C2MP_OPERATOR_LOGICAL_AND:
            case C2MP_OPERATOR_LOGICAL_OR:
            case C2MP_FUNCTION_SQRT:
            	isCommutative = 1;
                __attribute__ ((fallthrough));
                /*Remove GCC fallthrough warning)*/
            case C2MP_OPERATOR_BINARY_MINUS:
            case C2MP_OPERATOR_BINARY_DIVIDE:
            case C2MP_OPERATOR_LOWER_THAN:
            case C2MP_OPERATOR_GREATER_THAN:
            case C2MP_OPERATOR_LOWER_OR_EQUAL:
            case C2MP_OPERATOR_GREATER_OR_EQUAL:

				optimizationRefOp2 = getOperandOptimizationRef(q->operands[1].reference);
                __attribute__ ((fallthrough));
            case C2MP_OPERATOR_UNARY_MINUS:
            case C2MP_OPERATOR_UNARY_PLUS:
            case C2MP_OPERATOR_LOGICAL_NOT:
            case C2MP_OPERATOR_BITWISE_NOT:
				optimizationRefOp1 = getOperandOptimizationRef(q->operands[0].reference);


				
				if((refTableIndex = findRefTable(q->assignment)) < 0)
				{
					refTableIndex=createRefTableRow(q->assignment);
				}

				/*handle commutativity*/
				if(isCommutative)
				{
					if(optimizationRefOp2 < optimizationRefOp1)
					{//todo pour les commutative op only
						int temp = optimizationRefOp1;
						optimizationRefOp1 = optimizationRefOp2;
						optimizationRefOp2 = temp;
					}
				}

				int exprHashTableIndex = findExprHashTable(q->operator,
											optimizationRefOp1,
											optimizationRefOp2);
				if(exprHashTableIndex < 0)
				{
					exprHashTableIndex = createExprHash(q->operator,
											optimizationRefOp1,
											optimizationRefOp2);
				}

				setOptimizationRef(refTableIndex,exprHashTableIndex);

				int optimizationRefUsed;
				if((optimizationRefUsed = 
						findOptimizationRefTable(refTable[refTableIndex].optimizationRef)) >= 0)
				{
					if(optimizationRefUsed != refTableIndex)
					{
						optimizeExprToAssignment(q,refTable[optimizationRefUsed].reference);
					}
				}	
                break;

            case C2MP_QUAD_IF:
            case C2MP_QUAD_WHILE:
            case C2MP_QUAD_DOWHILE:
                q = ignoreBlocForCommonSubExpression(q->next, firstQuad);
                break;
            case C2MP_QUAD_ELSE:
            case C2MP_QUAD_ENDIF:
            case C2MP_QUAD_ENDWHILE:
            case C2MP_QUAD_ENDDOWHILE:
            	return q->next;
                break; 
            default:
            	panic("optimization.c","removeCommonSubExpressions","Not recognized operator\n");
            	break;
		}
		q = q->next;
	} while(q != firstQuad);

	return firstQuad;
}

quad* ignoreBlocForCommonSubExpression(quad* quads, quad* firstQuad)
{
    if(quads == NULL)
    {
        printf("no quads for optimization\n");
    }

    quad* q = quads;
    
    do
    {
        switch(q->operator)
        {
            case C2MP_QUAD_ASSIGNMENT:
            case C2MP_OPERATOR_BINARY_PLUS:
            case C2MP_OPERATOR_BINARY_DOT:
            case C2MP_OPERATOR_EQUAL:
            case C2MP_OPERATOR_NOT_EQUAL:
            case C2MP_OPERATOR_BITWISE_AND:
            case C2MP_OPERATOR_BITWISE_OR:
            case C2MP_OPERATOR_BITWISE_XOR:
            case C2MP_OPERATOR_LOGICAL_AND:
            case C2MP_OPERATOR_LOGICAL_OR:
            case C2MP_FUNCTION_SQRT:
            case C2MP_OPERATOR_BINARY_MINUS:
            case C2MP_OPERATOR_BINARY_DIVIDE:
            case C2MP_OPERATOR_LOWER_THAN:
            case C2MP_OPERATOR_GREATER_THAN:
            case C2MP_OPERATOR_LOWER_OR_EQUAL:
            case C2MP_OPERATOR_GREATER_OR_EQUAL:
            case C2MP_OPERATOR_UNARY_MINUS:
            case C2MP_OPERATOR_UNARY_PLUS:
            case C2MP_OPERATOR_LOGICAL_NOT:
            case C2MP_OPERATOR_BITWISE_NOT:
                printf("%s\n",getSymbolFromReference(q->assignment).name);
                assignNewOptimizationRef(q->assignment);
                break;
            case C2MP_QUAD_IF:
            case C2MP_QUAD_WHILE:
            case C2MP_QUAD_DOWHILE:
                q = ignoreBlocForCommonSubExpression(q->next, firstQuad);
                break;
            case C2MP_QUAD_ELSE:
                // ignore
                break;
            case C2MP_QUAD_ENDIF:
            case C2MP_QUAD_ENDWHILE:
            case C2MP_QUAD_ENDDOWHILE:
                return q;
                break; 
            default:
                panic("optimization.c","ignoreBlocForCommonSubExpression","Not recognized operator\n");
                break;
        }
        q = q->next;
    } while(q != firstQuad);

    return q;
}

void optimizeExprToAssignment(quad* q, int reference)
{
	q->operator = C2MP_QUAD_ASSIGNMENT;
	q->operands[0] = createVariableOperand(reference);
	q->operands[1] = createVoidOperand();
	q->operandsNum = 2;
}

void assignNewOptimizationRef(int reference)
{
    int refOp = findRefTable(reference);
    if((refOp) < 0)
    {
        refOp=createRefTableRow(reference);
    }
    refTable[refOp].optimizationRef = refCount;
    refCount++;
}

void setOptimizationRef(int reference, int hashReference)
{
	refTable[reference].optimizationRef = 
				exprHashTable[hashReference].optimizationRef;	
}

int getOperandOptimizationRef(int reference)
{
	int refOp = findRefTable(reference);
	if((refOp) < 0)
	{
		refOp=createRefTableRow(reference);
		refTable[refOp].optimizationRef = refCount;
		refCount++;
	}	
	return refTable[refOp].optimizationRef;
}

int createExprHash(char op, int operand1, int operand2)
{
	exprHashTable[exprHashSize].operator = op;
	exprHashTable[exprHashSize].operand1 = operand1;
	exprHashTable[exprHashSize].operand2 = operand2;
	exprHashTable[exprHashSize].optimizationRef = refCount;
	int exprHashTableIndex = exprHashSize;
	refCount++;
	exprHashSize++;
	return exprHashTableIndex;
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