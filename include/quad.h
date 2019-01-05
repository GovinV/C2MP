#ifndef QUAD_H
#define QUAD_H

#include "utils.h"
#include <string.h>
#include "../build/y.tab.h"
#include "semiQuad.h"
#include "symbol.h"
#include "ast.h"

#define C2MP_QUAD_OPERAND_VARIABLE 0
#define C2MP_QUAD_OPERAND_INTEGER 1
#define C2MP_QUAD_OPERAND_FLOAT 2
#define C2MP_QUAD_OPERAND_STRING 3

#define C2MP_QUAD_NO_ASSIGNEMENT -2

#define C2MP_QUAD_BINARY 2
#define C2MP_QUAD_UNARY  1

typedef struct quadOperand quadOperand;
typedef struct quad quad;

quadOperand createVariableOperand(int reference);
quadOperand createIntegerOperand(int value);
quadOperand createFloatOperand(float value);
quadOperand createVoidOperand(void);

/**
 * @brief Creates a string operand for a quad. Used by custom functions.
 * @param string The string (with quotes)
 * @return Retuns the created operand
 */
quadOperand createStringOperand(char *string);

/**
 * @brief Return the type of a symbol
 * @param ope The operand from which we want to know the type
 * @return The type of the symbol (cf. symbol.h)
 */
symbolType getSymbolTypeFromOperand(quadOperand ope);

/**
 * @brief Creates a quad structure
 * @param assignement The symbol (index in the table) to which assign the quad
 * @param operator the operator type
 * @param name The name of the function associated with the quad (optional)
 * @param operandsNum The number of operands used in the quad
 * @return The quad generated
 */
quad *createQuad(int assignment, char operator, char * name, int operandsNum, ...);

/**
 * @brief Creates a quad from an operand list
 * @param assignement The assignement to which belongs the resulting quad
 * @param operator The type of the quad
 * @param name The name of the function (optional), if it is one
 * @param argsNum The number of quad operands in the tab
 * @param list The list of the operands (a tab)
 * @return The created quad 
 */
quad *createQuadFromOperandList(int assignment, char operator, char *name, int argsNum, quadOperand *list);


/**
 * @brief Free the memory from quads
 * @param q A chained list of quads to be freed
 */
void freeQuads(quad *q);

quad *copySemiQuad(semiQuad *sq);
quad *generateQuadsFromAST_2(expressionAST *expr);
quad *generateQuadsFromAST(expressionAST *expr);
quad *getQuadFromSemiQuad(semiQuad *sq);
quad *concatQuads(quad *q1, quad *q2);
void printOperand(quadOperand operand);
void printQuads(quad* q);



/*typedef enum 
{
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_LE,
    OP_LTE,
    OP_EQ,
    OP_NEQ,
    OP_GE,
    OP_GTE,
    OP_ASSIGN
    //OP_LABEL
} operation;

typedef struct quad_s
{
    operation op;
    Symbol* arg1;
    Symbol* arg2;
    Symbol* res;
    struct quad_s *next;
} Quad;

Quad* quad_init(char * op, Symbol* arg1, Symbol* arg2, Symbol* res);
operation quad_operation(char* op);
void quad_free(Quad *quad);
void quad_add(Quad **dest, Quad *src);
void quad_print(Quad *quad);*/

#endif
