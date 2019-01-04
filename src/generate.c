#include "generate.h"
#include "quad.h"

void generateCode(quad *q, char *rounding)
{
    symbol ass;

    if (q == NULL)
    {
        printf("no quads\n");
    }

    quad *firstQuad = q;
    quad *currentQuad = q;

    int indent = 0;

    do
    {
        for (int i = 0; i < indent + 1; ++i)
        { // indent quads
            fprintf(output, "  ");
        }

        switch (currentQuad->operator)
        {
        case C2MP_OPERATOR_BINARY_PLUS:
            fprintf(output, "mpc_add(%s, ", getNameFromReference(currentQuad->assignment));
            printOperand(currentQuad->operand1);
            fprintf(output, ", ");
            printOperand(currentQuad->operand2);
            fprintf(output, ", %s);", rounding);
            break;

        case C2MP_OPERATOR_BINARY_MINUS:
            fprintf(output, "mpc_sub(%s, ", getNameFromReference(currentQuad->assignment));
            printOperand(currentQuad->operand1);
            fprintf(output, ", ");
            printOperand(currentQuad->operand2);
            fprintf(output, ", %s);", rounding);
            break;

        case C2MP_OPERATOR_BINARY_DOT:
            fprintf(output, "mpc_mul(%s, ", getNameFromReference(currentQuad->assignment));
            printOperand(currentQuad->operand1);
            fprintf(output, ", ");
            printOperand(currentQuad->operand2);
            fprintf(output, ", %s);", rounding);
            break;

        case C2MP_OPERATOR_BINARY_DIVIDE:
            fprintf(output, "mpc_div(%s, ", getNameFromReference(currentQuad->assignment));
            printOperand(currentQuad->operand1);
            fprintf(output, ", ");
            printOperand(currentQuad->operand2);
            fprintf(output, ", %s);", rounding);
            break;

        // result of mpc_cmp must be < 0
        case C2MP_OPERATOR_LOWER_THAN:
            fprintf(output, "%s = ", getNameFromReference(currentQuad->assignment));
            fprintf(output, "mpc_cmp(");
            printOperand(currentQuad->operand1);
            fprintf(output, ", ");
            printOperand(currentQuad->operand2);
            fprintf(output, ") < 0;");
            break;

        // result of mpc_cmp must be > 0
        case C2MP_OPERATOR_GREATER_THAN:
            fprintf(output, "%s = ", getNameFromReference(currentQuad->assignment));
            fprintf(output, "mpc_cmp(");
            printOperand(currentQuad->operand1);
            fprintf(output, ", ");
            printOperand(currentQuad->operand2);
            fprintf(output, ") > 0;");
            break;

        // result of mpc_cmp must be <= 0
        case C2MP_OPERATOR_LOWER_OR_EQUAL:
            fprintf(output, "%s = ", getNameFromReference(currentQuad->assignment));
            fprintf(output, "mpc_cmp(");
            printOperand(currentQuad->operand1);
            fprintf(output, ", ");
            printOperand(currentQuad->operand2);
            fprintf(output, ") <= 0;");
            break;

        // result of mpc_cmp must be >= 0
        case C2MP_OPERATOR_GREATER_OR_EQUAL:
            fprintf(output, "%s = ", getNameFromReference(currentQuad->assignment));
            fprintf(output, "mpc_cmp(");
            printOperand(currentQuad->operand1);
            fprintf(output, ", ");
            printOperand(currentQuad->operand2);
            fprintf(output, ") >= 0;");
            break;

        // result of mpc_cmp must be == 0
        case C2MP_OPERATOR_EQUAL:
            fprintf(output, "%s = ", getNameFromReference(currentQuad->assignment));
            fprintf(output, "mpc_cmp(");
            printOperand(currentQuad->operand1);
            fprintf(output, ", ");
            printOperand(currentQuad->operand2);
            fprintf(output, ") == 0;");
            break;

        // result must be different from 0
        case C2MP_OPERATOR_NOT_EQUAL:
            fprintf(output, "%s = ", getNameFromReference(currentQuad->assignment));
            fprintf(output, "!mpc_cmp(");
            printOperand(currentQuad->operand1);
            fprintf(output, ", ");
            printOperand(currentQuad->operand2);
            fprintf(output, ");");
            break;

        case C2MP_QUAD_ASSIGNMENT:
            ass = getSymbolFromReference(currentQuad->assignment);
            switch (ass.type_symbol)
            {
                case FLOAT_NUMBER:
                case INTEGER_NUMBER:
                    fprintf(output, "%s = ", getNameFromReference(currentQuad->assignment));
                    fprintf(output, "mpc_get_ldc(");
                    break;
                
                case MPC_T:
                    switch (getSymbolTypeFromOperand(currentQuad->operand1))
                    {
                        case FLOAT_NUMBER:
                            fprintf(output, "mpc_set_d(");
                            break;

                        case INTEGER_NUMBER:
                            fprintf(output, "mpc_set_si(");
                            break;

                        case MPC_T:
                            fprintf(output, "mpc_set(");
                            break;                           
                    
                        default:
                            panic("generate", "generateCode", "Unknown operand type");
                    }
                    // printed every time
                    fprintf(output, "%s, ", getNameFromReference(currentQuad->assignment));
                    break;

                default:
                    panic("generate", "generateCode", "Unknown operand type");
            }
            // this is printed every time
            printOperand(currentQuad->operand1);
            fprintf(output, ", %s);", rounding);
            break;

        case C2MP_QUAD_IF:
            ++indent;
            fprintf(output, "if (");
            printOperand(currentQuad->operand1);
            fprintf(output, ") {");
            break;

        case C2MP_QUAD_ELSE:
            fprintf(output, "\b\b} else {");
            break;

        case C2MP_QUAD_ENDIF:
            --indent;
            fprintf(output, "\b\b}");
            break;

        case C2MP_QUAD_WHILE:
            ++indent;
            fprintf(output, "while (%s) {", getNameFromReference(currentQuad->assignment));
            break;

        case C2MP_QUAD_DOWHILE:
            fprintf(output, "do {");
            break;

        case C2MP_QUAD_ENDWHILE:
            --indent;
            fprintf(output, "\b\b}   // %s", getNameFromReference(currentQuad->assignment));
            break;

        case C2MP_QUAD_ENDDOWHILE:
            --indent;
            fprintf(output, "\b\b while (%s);", getNameFromReference(currentQuad->assignment));
            break;

        case C2MP_FUNCTION_POW:
            fprintf(output, "mpc_pow(%s, ", getNameFromReference(currentQuad->assignment));
            printOperand(currentQuad->operand1);
            fprintf(output, ", ");
            printOperand(currentQuad->operand2);
            fprintf(output, ", %s);", rounding);
            break;
        
        case C2MP_FUNCTION_SQRT:
            fprintf(output, "mpc_sqrt(%s, ", getNameFromReference(currentQuad->assignment));
            printOperand(currentQuad->operand1);
            fprintf(output, ", %s);", rounding);
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

        fprintf(output, "\n");

        currentQuad = currentQuad->next;

    } while (currentQuad != firstQuad);
}