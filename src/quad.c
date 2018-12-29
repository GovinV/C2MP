#include "../include/quad.h"

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

quad *createQuad(int assignment, char operator, quadOperand value1, quadOperand value2)
{
    quad *q = malloc(sizeof(quad));
    q->assignment = assignment;
    q->operator = operator;
    q->operand1 = value1;
    q->operand2 = value2;

    q->next = q;
    q->previous = q;

    return q;
}

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
            return concatQuads(generatedQuads, createQuad(sq->assignment, sq->operator,
                                    createVariableOperand(generatedQuads->previous->assignment), createVoidOperand()));
            break;
        case C2MP_QUAD_ELSE:
        case C2MP_QUAD_ENDIF:
            return createQuad(-1, sq->operator, createVoidOperand(), createVoidOperand());
            break;
        case C2MP_QUAD_WHILE:
        case C2MP_QUAD_DOWHILE:
            //generatedQuads = generateQuadsFromAST_2(sq->expression);
            break;
        case C2MP_QUAD_ENDWHILE:
        case C2MP_QUAD_ENDDOWHILE:
            break;
        default:
            fprintf(stderr, "Warning, unknown semi quad operation : %d (%c)\n", sq->operator, sq->operator);
    }
    return NULL;
}

quad *generateQuadsFromAST_2(expressionAST *expr)
{
    if(expr == NULL)
    {
        fprintf(stderr, "Warning, tried to generate quads from NULL AST\n");
        return NULL;
    }
    quad *quadExpr1, *quadExpr2, *quadExpr, *finalQuads;
    int reference1, reference2, reference;
    switch(expr->operator)
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
            
            break;
        case C2MP_OPERATOR_UNARY_MINUS:
        case C2MP_OPERATOR_UNARY_PLUS:
        case C2MP_OPERATOR_LOGICAL_NOT:
        case C2MP_OPERATOR_BITWISE_NOT:
            
            break;
        case C2MP_CHARACTER_INTEGER: // number
           
            break;
        case C2MP_CHARACTER_FLOAT: // float
            
            break;
        case C2MP_CHARACTER_VARIABLE: // variable
           
            break;
        default:
            fprintf(stderr, "Warning, unknown expression operation : %c\n", expr->operator);
    }
    return NULL;
}

quad *generateQuadsFromAST(expressionAST *expr)
{
    if(expr == NULL)
    {
        fprintf(stderr, "Warning, tried to generate quads from NULL AST\n");
        return NULL;
    }

    quad *quadExpr1, *quadExpr2, *quadExpr, *finalQuads;
    int reference1, reference2, reference;
    switch(expr->operator)
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
            quadExpr1 = generateQuadsFromAST(expr->expression.e1);
            quadExpr2 = generateQuadsFromAST(expr->expression.e2);

            // the reference is the assigned variable of the last quad
            reference1 = quadExpr1->previous->assignment;
            reference2 = quadExpr2->previous->assignment;

            finalQuads = concatQuads(quadExpr1, quadExpr2);
            finalQuads = concatQuads(finalQuads, createQuad(newTemp().reference, expr->operator,
                                    createVariableOperand(reference1), createVariableOperand(reference2)));

            return finalQuads;
            break;
        case C2MP_OPERATOR_UNARY_MINUS:
        case C2MP_OPERATOR_UNARY_PLUS:
        case C2MP_OPERATOR_LOGICAL_NOT:
        case C2MP_OPERATOR_BITWISE_NOT:
            quadExpr = generateQuadsFromAST(expr->expression.e1);

            // the reference is the assigned variable of the last quad
            reference = quadExpr->previous->assignment;

            finalQuads = concatQuads(quadExpr, createQuad(newTemp().reference, expr->operator, createVariableOperand(reference), createVoidOperand()));

            return finalQuads;
            break;
        case C2MP_CHARACTER_INTEGER: // number
            return createQuad(newTemp().reference, C2MP_QUAD_ASSIGNMENT, createIntegerOperand(expr->valueInt), createVoidOperand());
            break;
        case C2MP_CHARACTER_FLOAT: // float
            return createQuad(newTemp().reference, C2MP_QUAD_ASSIGNMENT, createIntegerOperand(expr->valueFloat), createVoidOperand());
            break;
        case C2MP_CHARACTER_VARIABLE: // variable
            return createQuad(newTemp().reference, C2MP_QUAD_ASSIGNMENT, createVariableOperand(expr->valueVariable), createVoidOperand());
            break;
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

void printOperand(quadOperand operand)
{
    switch(operand.type)
    {
        case C2MP_QUAD_OPERAND_INTEGER:
            printf("%d", operand.valueInt);
            break;
        case C2MP_QUAD_OPERAND_FLOAT:
            printf("%f", operand.valueFloat);
            break;
        case C2MP_QUAD_OPERAND_VARIABLE:
            printf("%s", getNameFromReference(operand.reference));
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
                printf("%s = %c ", getNameFromReference(currentQuad->assignment), currentQuad->operator);
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
            default:
                fprintf(stderr, "Warning, unknown quad operation : %c\n", currentQuad->operator);
        }

        printf("\n");

        currentQuad = currentQuad->next;
    }while(currentQuad != firstQuad);
}

/*#include <stdlib.h>
#include <stdio.h>

Quad *quad_init(char* op, Symbol *arg1, Symbol *arg2, Symbol *res)
{
    Quad *new = (Quad *)malloc(sizeof(Quad));
    new->arg1 = arg1;
    new->arg2 = arg2;
    new->op = quad_operation(op);
    new->res = res;

    return new;
}

operation quad_operation(char* op)
{
    if (strcmp(op, "+") == 0)
    {
        return OP_ADD;
    }
    else if (strcmp(op, "-") == 0)
    {
        return OP_SUBTRACT;
    }
    else if (strcmp(op, "*") == 0)
    {
        return OP_MULTIPLY;
    }
    else if (strcmp(op, "/") == 0)
    {
        return OP_DIVIDE;
    }
    else if (strcmp(op, "") == 0)
    {
        return OP_LE;
    }
    else if (strcmp(op, "-") == 0)
    {
        return OP_LTE;
    }
    else if (strcmp(op, "-") == 0)
    {
        return OP_EQ;
    }
    else if (strcmp(op, "-") == 0)
    {
        return OP_NEQ;
    }
    else if (strcmp(op, "-") == 0)
    {
        return OP_GE;
    }
    else if (strcmp(op, "-") == 0)
    {
        return OP_GTE;
    }
    else if (strcmp(op, "=") == 0)
    {
        return OP_ASSIGN;
    }
    else panic("quad.c", "quad_operation", "no operation");
}

void quad_free(Quad *quad)
{
    free(quad);
}

void quad_add(Quad **dest, Quad *src)
{
    if(src != NULL)
    {
        Quad *current = dest[0];
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = src;
        src->next = NULL;
    }
}

void quad_print(Quad *quad)
{
    if(quad->arg2 != NULL) // if 2 args
    {
        printf("%s = %s(%d) %c %s(%d)\n",
            quad->res->id,
            quad->arg1->id,quad->arg1->val,
            quad->op,
            quad->arg2->id,quad->arg2->val
        );
    }
    else
    {
        printf("%s = %c %s(%d)\n",
            quad->res->id,
            quad->op,
            quad->arg1->id,quad->arg1->val
        );
    }
}*/
