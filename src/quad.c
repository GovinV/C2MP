#include <stdarg.h>
#include "quad.h"

quadOperand createVariableOperand(int reference)
{
    quadOperand operand;
    operand.type = C2MP_QUAD_OPERAND_VARIABLE;
    operand.reference = reference;
    return operand;
}

quadOperand createIntegerOperand(int value)
{
    quadOperand operand;
    operand.type = C2MP_QUAD_OPERAND_INTEGER;
    operand.valueInt = value;
    return operand;
}

quadOperand createFloatOperand(float value)
{
    quadOperand operand;
    operand.type = C2MP_QUAD_OPERAND_FLOAT;
    operand.valueFloat = value;
    return operand;
}

quadOperand createVoidOperand(void)
{
    quadOperand operand;
    operand.type = -1;
    return operand;
}

quad *createQuad(int assignment, char operator, char * name, int operandsNum, ...)
{
    quad *q = malloc(sizeof(quad));
    q->assignment = assignment;
    q->operator = operator;

    if (operator == C2MP_FUNCTION_UNKNOWN)
    {
        q->fctName = strdup(name);
    }
    
    va_list operandsList;
    va_start(operandsList, operandsNum);

    for (int i = 0; i < operandsNum; i ++)
    {
        q->operands[i] = va_arg(operandsList, quadOperand);
    }

    va_end(operandsList);

    q->next = q;
    q->previous = q;

    return q;
}

/* copy result */
quad *copySemiQuad(semiQuad *sq)
{
    if(sq == NULL)
    {
        fprintf(stderr, "Warning, tried to copy quads from NULL semiQuad\n");
        return NULL;
    }

    quad *generatedQuads;
    switch(sq->operator)
    {
        case C2MP_QUAD_ASSIGNMENT:
        case C2MP_QUAD_IF:
            generatedQuads = generateQuadsFromAST(sq->expression);
            return concatQuads(generatedQuads, 
                        createQuad(sq->assignment, sq->operator, NULL, C2MP_QUAD_UNARY, 
                            createVariableOperand(generatedQuads->previous->assignment)));
            break;

        case C2MP_QUAD_ELSE:
        case C2MP_QUAD_ENDIF:
            return createQuad(-1, sq->operator, NULL, 0);
            break;

        case C2MP_QUAD_WHILE:
            generatedQuads = generateQuadsFromAST(sq->expression);
            generatedQuads = concatQuads(generatedQuads, 
                                createQuad(sq->assignment,
                                    C2MP_QUAD_ASSIGNMENT,
                                    NULL,
                                    C2MP_QUAD_UNARY,
                                    createVariableOperand(generatedQuads->previous->assignment)));
            generatedQuads = concatQuads(generatedQuads, 
                                    createQuad(sq->assignment, sq->operator, NULL, 0));
            return generatedQuads;
            break;
        case C2MP_QUAD_DOWHILE:
            return createQuad(-1, sq->operator, NULL, 0);
            break;
        case C2MP_QUAD_ENDWHILE:
        case C2MP_QUAD_ENDDOWHILE:
            generatedQuads = generateQuadsFromAST(sq->expression);
            generatedQuads = concatQuads(generatedQuads, 
                                createQuad(sq->assignment,
                                    C2MP_QUAD_ASSIGNMENT,
                                    NULL,
                                    C2MP_QUAD_UNARY,
                                    createVariableOperand(generatedQuads->previous->assignment)));
            generatedQuads = concatQuads(generatedQuads, 
                                createQuad(sq->assignment, sq->operator, NULL, 0));
            return generatedQuads;
            break;
        default:
            fprintf(stderr, "Warning, unknown semi quad operation : %d (%c)\n",
                sq->operator, sq->operator);
    }

    return NULL;
}


// TO BE EFFICIENT : C2MP_OPERATOR_LOGICAL_AND and C2MP_OPERATOR_LOGICAL_OR 
// should not do this big logic with if then else if the remaining tree is small
// TODO
// example 1 : a && (3*x*y*z+3241 > 3*d/g) in this case it is smart to check 
// for the second operand only if a is true
// example 2 : a && b in this case it is better just to do : temporary = a && b
/* read AST : a<b&&b<c become :
 *
 *     &&
 *  <       <
 * a  b   b   c
*/
quad *generateQuadsFromAST(expressionAST *expr)
{
    if(expr == NULL)
    {
        fprintf(stderr, "Warning, tried to generate quads from NULL AST\n");
        return NULL;
    }

    quad *quadExpr1, *quadExpr2, *quadExpr, *finalQuads = NULL;
    int reference1, reference2, reference;
    int resultTemporary;
    switch(expr->operator)
    {
        case C2MP_OPERATOR_LOGICAL_AND:
            quadExpr1 = generateQuadsFromAST(expr->expression.e1);
            quadExpr2 = generateQuadsFromAST(expr->expression.e2);

            reference1 = quadExpr1->previous->assignment;
            reference2 = quadExpr2->previous->assignment;

            resultTemporary = newTemp().reference;

            /* generate these quads :
            a = ...
            t = a!=0
            if(t)
            {
                b = ...
                t = b
            }
            else
            {
                t=0
            }

            a result of expr1
            b result of expr2
            */

            finalQuads = concatQuads(quadExpr1,
                            createQuad(resultTemporary,
                                C2MP_OPERATOR_NOT_EQUAL,
                                NULL,
                                C2MP_QUAD_BINARY,
                                createVariableOperand(reference1),
                                createIntegerOperand(0))); // if expr1 == 0
            finalQuads = concatQuads(finalQuads,
                            createQuad(-1, C2MP_QUAD_IF, NULL, 
                                C2MP_QUAD_UNARY,
                                createVariableOperand(resultTemporary))); // if resultTemporary != 0
            finalQuads = concatQuads(finalQuads, quadExpr2);
            finalQuads = concatQuads(finalQuads,
                            createQuad(resultTemporary,
                                C2MP_QUAD_ASSIGNMENT,
                                NULL,
                                C2MP_QUAD_UNARY,
                                createVariableOperand(reference2))); // t = expr2
            finalQuads = concatQuads(quadExpr1, 
                                     createQuad(-1, C2MP_QUAD_ELSE, NULL, 0)); // else
            finalQuads = concatQuads(finalQuads,
                            createQuad(resultTemporary,
                                C2MP_QUAD_ASSIGNMENT, NULL, 
                                C2MP_QUAD_UNARY,
                                createIntegerOperand(0))); // t = 0
            finalQuads = concatQuads(quadExpr1,
                            createQuad(-1, C2MP_QUAD_ENDIF, NULL, 0)); // endif
            finalQuads = concatQuads(finalQuads,
                            createQuad(resultTemporary,
                                C2MP_QUAD_ASSIGNMENT, NULL, C2MP_QUAD_UNARY,
                                createVariableOperand(resultTemporary)));
                    // t = t, needed for generation and will be optimized

            return finalQuads;
            break;

        case C2MP_OPERATOR_LOGICAL_OR:
            quadExpr1 = generateQuadsFromAST(expr->expression.e1);
            quadExpr2 = generateQuadsFromAST(expr->expression.e2);

            reference1 = quadExpr1->previous->assignment;
            reference2 = quadExpr2->previous->assignment;

            resultTemporary = newTemp().reference;

            /* generate these quads :
            a = ...
            t = a==0
            if(t)
            {
                b = ...
                t = b
            }
            else
            {
                t=1
            }

            a result of expr1
            b result of expr2
            */

            finalQuads = concatQuads(quadExpr1,
                            createQuad(resultTemporary,
                                C2MP_OPERATOR_EQUAL,
                                NULL,
                                C2MP_QUAD_BINARY,
                                createVariableOperand(reference1),
                                createIntegerOperand(0))); // if expr1 == 0
            finalQuads = concatQuads(finalQuads,
                            createQuad(-1,
                                C2MP_QUAD_IF,
                                NULL,
                                C2MP_QUAD_UNARY,
                                // if resultTemporary == 0
                                createVariableOperand(resultTemporary))); 
            finalQuads = concatQuads(finalQuads, quadExpr2);
            finalQuads = concatQuads(finalQuads,
                            createQuad(resultTemporary,
                                C2MP_QUAD_ASSIGNMENT,
                                NULL,
                                C2MP_QUAD_UNARY,
                                createVariableOperand(reference2))); // t = expr2
            finalQuads = concatQuads(quadExpr1,
                            createQuad(-1, C2MP_QUAD_ELSE, NULL, 0)); // else
            finalQuads = concatQuads(finalQuads,
                            createQuad(resultTemporary,
                                C2MP_QUAD_ASSIGNMENT,
                                NULL,
                                C2MP_QUAD_UNARY,
                                createIntegerOperand(1))); // t = 1
            finalQuads = concatQuads(quadExpr1,
                            createQuad(-1,
                                C2MP_QUAD_ENDIF, NULL, 0)); // endif
            finalQuads = concatQuads(finalQuads,
                            createQuad(resultTemporary,
                                C2MP_QUAD_ASSIGNMENT,
                                NULL,
                                C2MP_QUAD_UNARY,
                                createVariableOperand(resultTemporary)));
                    // t = t, needed for generation and will be optimized

            return finalQuads;
            break;

        case C2MP_OPERATOR_BINARY_PLUS:
        case C2MP_OPERATOR_BINARY_MINUS:
        case C2MP_OPERATOR_BINARY_DOT:
        case C2MP_OPERATOR_BINARY_DIVIDE:
        case C2MP_OPERATOR_LOWER_THAN:
        case C2MP_OPERATOR_GREATER_THAN:
        case C2MP_OPERATOR_LOWER_OR_EQUAL:
        case C2MP_OPERATOR_GREATER_OR_EQUAL:
        case C2MP_OPERATOR_EQUAL:
        case C2MP_OPERATOR_NOT_EQUAL:
        case C2MP_OPERATOR_BITWISE_AND:
        case C2MP_OPERATOR_BITWISE_OR:
        case C2MP_OPERATOR_BITWISE_XOR:
        /******************** FUNCTIONS ********************/
        case C2MP_FUNCTION_POW:
            quadExpr1 = generateQuadsFromAST(expr->expression.e1);
            quadExpr2 = generateQuadsFromAST(expr->expression.e2);

            // the reference is the assigned variable of the last quad
            reference1 = quadExpr1->previous->assignment;
            reference2 = quadExpr2->previous->assignment;

            finalQuads = concatQuads(quadExpr1, quadExpr2);
            finalQuads = concatQuads(finalQuads, 
                            createQuad(newTemp().reference, expr->operator,
                                NULL,
                                C2MP_QUAD_BINARY,
                                createVariableOperand(reference1), 
                                createVariableOperand(reference2)));

            return finalQuads;
            break;

        case C2MP_OPERATOR_UNARY_MINUS:
        case C2MP_OPERATOR_UNARY_PLUS:
        case C2MP_OPERATOR_LOGICAL_NOT:
        case C2MP_OPERATOR_BITWISE_NOT:
        /******************** FUNCTIONS ********************/
        case C2MP_FUNCTION_ABS:
        case C2MP_FUNCTION_EXP:
        case C2MP_FUNCTION_COS:
        case C2MP_FUNCTION_COSH:
        case C2MP_FUNCTION_LOG:
        case C2MP_FUNCTION_LOG10:
        case C2MP_FUNCTION_SIN:
        case C2MP_FUNCTION_SINH:
        case C2MP_FUNCTION_SQRT:

            quadExpr = generateQuadsFromAST(expr->expression.e1);

            // the reference is the assigned variable of the last quad
            reference = quadExpr->previous->assignment;

            finalQuads = concatQuads(quadExpr, 
                            createQuad(newTemp().reference, expr->operator,
                                NULL,
                                C2MP_QUAD_UNARY,
                                createVariableOperand(reference)));

            return finalQuads;
            break;

        case C2MP_CHARACTER_INTEGER: // number
            return createQuad(newTemp().reference, C2MP_QUAD_ASSIGNMENT,
                        NULL,
                        C2MP_QUAD_UNARY,
                        createIntegerOperand(expr->valueInt));
            break;

        case C2MP_CHARACTER_FLOAT: // float

            return createQuad(newTemp().reference, C2MP_QUAD_ASSIGNMENT,
                        NULL,
                        C2MP_QUAD_UNARY,
                        createFloatOperand(expr->valueFloat));
            break;

        case C2MP_CHARACTER_VARIABLE: // variable
            return createQuad(newTemp().reference, C2MP_QUAD_ASSIGNMENT,
                        NULL,
                        C2MP_QUAD_UNARY,
                        createVariableOperand(expr->valueVariable));
            break;

        // cutom function
        case C2MP_FUNCTION_UNKNOWN:
            finalQuads = generateQuadsFromAST(expr->customFunction.ags[0]);
            for (int i = 1; i < expr->customFunction.argnum; i++)
            {
                quadExpr = generateQuadsFromAST(expr->customFunction.ags[i]);
                concat(finalQuads, quadExpr);
            }
            finalQuads = concatQuads(finalQuads,
                                     createQuad(newTemp().reference, expr->operator,
                                     NULL, C2MP_QUAD_UNARY,
                                     createVariableOperand(reference)));
            return finalQuads;

        default:
            fprintf(stderr, "Warning, unknown expression operation : %c\n", expr->operator);
    }

    return NULL;
}

quad *getQuadFromSemiQuad(semiQuad *sq)
{
    if(sq == NULL)
    {
        printf("no quad\n");
    }

    semiQuad *firstSemiQuad = sq;
    semiQuad *currentSemiQuad = sq;

    quad *currentQuad = copySemiQuad(firstSemiQuad);

    currentSemiQuad = currentSemiQuad->next;

    // 
    while(currentSemiQuad != firstSemiQuad)
    {
        currentQuad = concatQuads(currentQuad, copySemiQuad(currentSemiQuad));

        currentSemiQuad = currentSemiQuad->next;
    }

    return currentQuad;
}

quad *concatQuads(quad *q1, quad *q2)
{
    if(q2 == NULL)
    {
        return q1;
    }
    if(q1 == NULL)
    {
        return q2;
    }

    quad *head1 = q1;
    quad *head2 = q2;
    quad *tail1 = q1->previous;
    quad *tail2 = q2->previous;

    head1->previous = tail2;
    tail1->next = head2;
    head2->previous = tail1;
    tail2->next = head1;

    return q1;
}

symbolType getSymbolTypeFromOperand(quadOperand ope)
{
    switch (ope.type)
    {
        case C2MP_QUAD_OPERAND_INTEGER:
            return INTEGER_NUMBER;

        case C2MP_QUAD_OPERAND_FLOAT:
            return FLOAT_NUMBER;

        case C2MP_QUAD_OPERAND_VARIABLE:
            return getSymbolFromReference(ope.reference).type_symbol;

        default:
            panic("quad.c", "getSymbolTypeFromOperan", "Unknown symbol type");
    }
    return ERROR;
}

void printOperand(quadOperand operand)
{
    switch(operand.type)
    {
        case C2MP_QUAD_OPERAND_INTEGER:
            fprintf(output, "%d", operand.valueInt);
            break;
        case C2MP_QUAD_OPERAND_FLOAT:
            fprintf(output, "%f", operand.valueFloat);
            break;
        case C2MP_QUAD_OPERAND_VARIABLE:
            fprintf(output, "%s", getNameFromReference(operand.reference));
            break;
        default:
            fprintf(stderr, "Warning, unknown operand type : %d\n", operand.type);
    }
}

void printQuads(quad* q)
{
    if(q == NULL)
    {
        printf("no quad\n");
    }

    quad *firstQuad = q;
    quad *currentQuad = q;

    int indent = 0;

    do
    {
        for(int i=0;i<indent;++i)
        { // indent quads
            printf("  ");
        }

        switch(currentQuad->operator)
        {
            case C2MP_OPERATOR_BINARY_PLUS:
            case C2MP_OPERATOR_BINARY_MINUS:
            case C2MP_OPERATOR_BINARY_DOT:
            case C2MP_OPERATOR_BINARY_DIVIDE:
            case C2MP_OPERATOR_LOWER_THAN:
            case C2MP_OPERATOR_GREATER_THAN:
            case C2MP_OPERATOR_LOWER_OR_EQUAL:
            case C2MP_OPERATOR_GREATER_OR_EQUAL:
            case C2MP_OPERATOR_EQUAL:
            case C2MP_OPERATOR_NOT_EQUAL:
            case C2MP_OPERATOR_BITWISE_AND:
            case C2MP_OPERATOR_BITWISE_OR:
            case C2MP_OPERATOR_BITWISE_XOR:
            case C2MP_OPERATOR_LOGICAL_AND:
            case C2MP_OPERATOR_LOGICAL_OR:
                printf("%s = ", getNameFromReference(currentQuad->assignment));
                printOperand(currentQuad->operand1);
                printf(" %c ", currentQuad->operator);
                printOperand(currentQuad->operand2);
                break;
            case C2MP_OPERATOR_UNARY_MINUS:
            case C2MP_OPERATOR_UNARY_PLUS:
            case C2MP_OPERATOR_LOGICAL_NOT:
            case C2MP_OPERATOR_BITWISE_NOT:
                printf("%s = %c ", 
                    getNameFromReference(currentQuad->assignment),
                    currentQuad->operator);
                printOperand(currentQuad->operand1);
                break;
            case C2MP_QUAD_ASSIGNMENT:
                printf("%s = ", getNameFromReference(currentQuad->assignment));
                printOperand(currentQuad->operand1);
                break;
            case C2MP_QUAD_IF:
                ++indent;
                printf("if ");
                printOperand(currentQuad->operand1);
                break;
            case C2MP_QUAD_ELSE:
                printf("\b\belse");
                break;
            case C2MP_QUAD_ENDIF:
                --indent;
                printf("\b\bendif");
                break;
            case C2MP_QUAD_WHILE:
                ++indent;
                printf("while %s", getNameFromReference(currentQuad->assignment));
                break;
            case C2MP_QUAD_DOWHILE:
                printf("do ");
                break;
            case C2MP_QUAD_ENDWHILE:
                --indent;
                printf("\b\bendwhile   // %s", getNameFromReference(currentQuad->assignment));
                break;
            case C2MP_QUAD_ENDDOWHILE:
                --indent;
                printf("\b\bwhile");
                break;
            default:
                fprintf(stderr, "Warning, unknown quad operation : %c\n", currentQuad->operator);
        }

        printf("\n");

        currentQuad = currentQuad->next;
    }while(currentQuad != firstQuad);
}
