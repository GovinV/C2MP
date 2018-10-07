#include "Data.h"

data_str newData(void)
{
    data_str retValue;
    retValue.type = FREE;
    return retValue;
}

void freeData(data_str *data)
{
    variableType type = data->type;
    if(type != FREE)
    {
        switch(type)
        {
            case BOOL:
            case FLOAT:
            case INT:
                break;
            case STRING:
                delete[] (T_STRING*)data->value.pointer;
                break;
            case OBJECT:
                delete (T_OBJECT*)data->value.pointer;
                break;
            default:
                std::cerr << "error ! type of "<< data << " is " << type << std::endl;
                break;
        }

        data->type = FREE;
    }
}

bool isNull(data_str *data)
{
    return data->type == FREE;
}
