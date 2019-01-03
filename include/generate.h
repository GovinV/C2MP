#include "utils.h"
#include "semiQuad.h"
#include "symbol.h"
#include "ast.h"


typedef struct quadOperand quadOperand;
typedef struct quad quad;


void create_file(void);
void create_variable(quad * quad);
void create_operation(quad * quad);
void close_file(void);