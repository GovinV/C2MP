#include "stdio.h"
#include "stdlib.h"

static char table_des_symboles[];

table_des_symboles = 
{ 
    "+",
    "-",
    "*",
    "/", 
    "==",
    "<",
    "<=",
    ">",
    ">=",
    "!=",
    "{",
    "}",
    "[",
    "]",
    "<<",
    ">>",
    "||",
    "&&",
    "char",
    "unsigned char",
    "int",
    "unsigned int",
    "long",
    "unsigned long",
    "double",
    "float",
    "while",
    "for",
    "if",
    "else",
    "do"
};

char index_2_symbole(int index)
{
    return table_des_symboles[index];
}

int symbole_2_index(char symbole)
{
    int i = 0;
    for( ; i < strlen(table_des_symboles)+1 ; i++)
    {
        if(table_des_symboles[i] == symbole)
            return i;
    }
    return -1;
}