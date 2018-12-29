#include "../include/semiQuad.h"

semiQuad *createSemiQuad(char operator, int assignment, expressionAST *expression)
{
    semiQuad *quad = malloc(sizeof(semiQuad));
    quad->operator = operator;
    quad->assignment = assignment;
    quad->expression = expression;
    quad->next = quad;
    quad->previous = quad;

    return quad;
}

// concatenates 2 quads or 2 quadlists
semiQuad *concatSemiQuad(semiQuad *q1, semiQuad *q2)
{
    if(q2 == NULL)
    {
        return q1;
    }
    if(q1 == NULL)
    {
        return q2;
    }

    /*if(q2->next == q2)
    {
        q2->next = q1;
        q2->previous = q1->previous;
        q1->previous->next = q2;
        q1->previous = q2;
    }
    else
    {
        q1->previous->next = q2;
        q2->previous->next = q1;
        q1->previous = q2->previous;
        q2->previous = q1->previous;
    }*/
    semiQuad *head1 = q1;
    semiQuad *head2 = q2;
    semiQuad *tail1 = q1->previous;
    semiQuad *tail2 = q2->previous;

    head1->previous = tail2;
    tail1->next = head2;
    head2->previous = tail1;
    tail2->next = head1;

    return q1;
}

void printSemiQuads(semiQuad *q)
{
    if(q == NULL)
    {
        printf("no quad\n");
    }

    semiQuad *firstQuad = q;
    semiQuad *currentQuad = q;

    int indent = 0;

    do
    {
        for(int i=0;i<indent;++i)
        { // indent quads
            printf("  ");
        }

        switch(currentQuad->operator)
        {
            case C2MP_QUAD_ASSIGNMENT:
                printf("%s = ", getNameFromReference(currentQuad->assignment));
                printExpressionAST(currentQuad->expression);
                break;
            case C2MP_QUAD_IF:
                ++indent;
                printf("if ");
                printExpressionAST(currentQuad->expression);
                break;
            case C2MP_QUAD_ELSE:
                printf("\b\belse");
                break;
            case C2MP_QUAD_ENDIF:
                --indent;
                printf("\b\bendif");
                break;
            default:
                fprintf(stderr, "Warning, unknown semi quad operation : %d (%c)\n", currentQuad->operator, currentQuad->operator);
        }
        printf("\n");

        currentQuad = currentQuad->next;
    }while(currentQuad != firstQuad);
}