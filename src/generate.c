#include "generate.h"
#include "quad.h"

void generateCode(quad *q, char *rounding)
{
    if (q == NULL)
    {
        printf("no quads\n");
    }

    quad *firstQuad = q;
    quad *currentQuad = q;

    int indent = 0;

    do
    {
        for (int i = 0; i < indent; ++i)
        { // indent quads
            printf("  ");
        }

        switch (currentQuad->operator)
        {
        case C2MP_OPERATOR_BINARY_PLUS:
            printf("mpc_add(%s, ", getNameFromReference(currentQuad->assignment));
            printOperand(currentQuad->operand1);
            printf(", ");
            printOperand(currentQuad->operand2);
            printf(", %s);", rounding);
            break;

        case C2MP_OPERATOR_BINARY_MINUS:
            printf("mpc_sub(%s, ", getNameFromReference(currentQuad->assignment));
            printOperand(currentQuad->operand1);
            printf(", ");
            printOperand(currentQuad->operand2);
            printf(", %s);", rounding);
            break;

        case C2MP_OPERATOR_BINARY_DOT:
            printf("mpc_mul(%s, ", getNameFromReference(currentQuad->assignment));
            printOperand(currentQuad->operand1);
            printf(", ");
            printOperand(currentQuad->operand2);
            printf(", %s);", rounding);
            break;

        case C2MP_OPERATOR_BINARY_DIVIDE:
            printf("mpc_div(%s, ", getNameFromReference(currentQuad->assignment));
            printOperand(currentQuad->operand1);
            printf(", ");
            printOperand(currentQuad->operand2);
            printf(", %s);", rounding);
            break;

        // result of mpc_cmp must be < 0
        case C2MP_OPERATOR_LOWER_THAN:
            printf("%s = ", getNameFromReference(currentQuad->assignment));
            printf("mpc_cmp(");
            printOperand(currentQuad->operand1);
            printf(", ");
            printOperand(currentQuad->operand2);
            printf(") < 0;");
            break;

        // result of mpc_cmp must be > 0
        case C2MP_OPERATOR_GREATER_THAN:
            printf("%s = ", getNameFromReference(currentQuad->assignment));
            printf("mpc_cmp(");
            printOperand(currentQuad->operand1);
            printf(", ");
            printOperand(currentQuad->operand2);
            printf(") > 0;");
            break;

        // result of mpc_cmp must be <= 0
        case C2MP_OPERATOR_LOWER_OR_EQUAL:
            printf("%s = ", getNameFromReference(currentQuad->assignment));
            printf("mpc_cmp(");
            printOperand(currentQuad->operand1);
            printf(", ");
            printOperand(currentQuad->operand2);
            printf(") <= 0;");
            break;

        // result of mpc_cmp must be >= 0
        case C2MP_OPERATOR_GREATER_OR_EQUAL:
            printf("%s = ", getNameFromReference(currentQuad->assignment));
            printf("mpc_cmp(");
            printOperand(currentQuad->operand1);
            printf(", ");
            printOperand(currentQuad->operand2);
            printf(") >= 0;");
            break;

        // result of mpc_cmp must be == 0
        case C2MP_OPERATOR_EQUAL:
            printf("%s = ", getNameFromReference(currentQuad->assignment));
            printf("mpc_cmp(");
            printOperand(currentQuad->operand1);
            printf(", ");
            printOperand(currentQuad->operand2);
            printf(") == 0;");
            break;

        // result must be different from 0
        case C2MP_OPERATOR_NOT_EQUAL:
            printf("%s = ", getNameFromReference(currentQuad->assignment));
            printf("!mpc_cmp(");
            printOperand(currentQuad->operand1);
            printf(", ");
            printOperand(currentQuad->operand2);
            printf(");");
            break;

        case C2MP_QUAD_ASSIGNMENT:
            switch (currentQuad->operand1.type)
            {
                case C2MP_QUAD_OPERAND_VARIABLE:
                    printf("mpc_set(");
                    break;

                case C2MP_QUAD_OPERAND_INTEGER:
                    printf("mpc_set_si(");
                    break;
                
                // conflicts with float?
                case C2MP_QUAD_OPERAND_FLOAT:
                    printf("mpc_set_d(");
                    break;
                    
                default:
                    panic("generate", "generateCode", "Unknown operand type");
            }

            printf("%s, ", getNameFromReference(currentQuad->assignment));
            printOperand(currentQuad->operand1);
            printf(", %s);", rounding);
            break;

        case C2MP_QUAD_IF:
            ++indent;
            printf("if (");
            printOperand(currentQuad->operand1);
            printf(") {");
            break;

        case C2MP_QUAD_ELSE:
            printf("\b\b} else {");
            break;

        case C2MP_QUAD_ENDIF:
            --indent;
            printf("\b\b}");
            break;

        case C2MP_QUAD_WHILE:
            ++indent;
            printf("while (%s) {", getNameFromReference(currentQuad->assignment));
            break;

        case C2MP_QUAD_DOWHILE:
            printf("do {");
            break;

        case C2MP_QUAD_ENDWHILE:
            --indent;
            printf("\b\b}   // %s", getNameFromReference(currentQuad->assignment));
            break;

        case C2MP_QUAD_ENDDOWHILE:
            --indent;
            printf("\b\b while (%s);", getNameFromReference(currentQuad->assignment));
            break;

        // not supported yet
        case C2MP_OPERATOR_BITWISE_AND:
        case C2MP_OPERATOR_BITWISE_OR:
        case C2MP_OPERATOR_BITWISE_NOT:
        case C2MP_OPERATOR_BITWISE_XOR:
        case C2MP_OPERATOR_UNARY_MINUS:
        case C2MP_OPERATOR_UNARY_PLUS:
        case C2MP_OPERATOR_LOGICAL_OR:
        case C2MP_OPERATOR_LOGICAL_AND:
        case C2MP_OPERATOR_LOGICAL_NOT:
            break;

        default:
            fprintf(stderr, "Warning, unknown quad operation : %c\n", currentQuad->operator);
        }

        printf("\n");

        currentQuad = currentQuad->next;

    } while (currentQuad != firstQuad);
}