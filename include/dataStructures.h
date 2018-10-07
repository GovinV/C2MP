#ifndef DATASTRUCTURES_H_INCLUDED
#define DATASTRUCTURES_H_INCLUDED

#define EMPTY_DATA(data) ((data).type==FREE)

typedef union data_value
{
    void *pointer;
    float f;
    int i;
    bool b;
} data_value;

enum variableType
{
    FREE, BOOL, INT, FLOAT, STRING,OBJECT
};

struct data_str
{
    variableType type;
    data_value value;
};

#endif // DATASTRUCTURES_H_INCLUDED
