/** Data.h
  *
  * is a variable that can store any predefined type (number, string, pointer, boolean etc)
  * with that there is less need to care about having the right type.
  *
  *
  */

#ifndef DATA_H
#define DATA_H

#include <iostream>
#include <sstream>
#include <math.h>
#include <string.h>
#include <typeinfo>
#include <type_traits>
#include <cstdint>
#include <limits>
#include <stdint.h>

#define T_BOOL bool
#define T_FLOAT float
#define T_INT int64_t
#define T_STRING char
#define T_LINK char
#define T_OBJECT Object

#define STR_UNDEFINED_DATA "undefined"
#define STR_OBJECT "object"

#include "Object.h"
class Object;

data_str newData(void);
void freeData(data_str *data);
bool isNull(data_str data);
void link(data_str *data, T_OBJECT *value); // set, but for a link

template <typename t, typename std::enable_if <std::is_same<t,T_BOOL>::value> :: type* = nullptr>
void set(data_str *data, T_BOOL value)
{
    freeData(data);

    data->type = BOOL;
    data->value.b = value;
}

template <typename t, typename std::enable_if <std::is_same<t,T_INT>::value> :: type* = nullptr>
void set(data_str *data, T_INT value)
{
    freeData(data);

    data->type = INT;
    data->value.i = value;
}

template <typename t, typename std::enable_if <std::is_same<t,T_FLOAT>::value> :: type* = nullptr>
void set(data_str *data, T_FLOAT value)
{
    freeData(data);

    data->type = FLOAT;
    data->value.f = value;
}

template <typename t, typename std::enable_if <std::is_same<t,T_STRING>::value> :: type* = nullptr>
void set(data_str *data, const T_STRING* value)
{
    freeData(data);

    data->type = STRING;
    T_STRING *var = new char[strlen(value)+1];
    strcpy(var, value);
    data->value.pointer = var;
}

template <typename t, typename std::enable_if <std::is_same<t,T_OBJECT>::value> :: type* = nullptr>
void set(data_str *data, T_OBJECT *value)
{
    freeData(data);

    data->type = OBJECT;
    data->value.pointer = value;
}




template <typename t, typename std::enable_if <std::is_same<t,T_BOOL>::value> :: type* = nullptr>
T_BOOL get(data_str data)
{
    if(data.type == BOOL)
    {
        return *(T_BOOL*)data.value;
    }
    //else

    return false;
}

template <typename t, typename std::enable_if <std::is_same<t,T_INT>::value> :: type* = nullptr>
T_INT get(data_str data)
{
    if(data.type == INT)
    {
        return *(T_INT*)data.value;
    }
    //else

    return 0;
}

template <typename t, typename std::enable_if <std::is_same<t,T_FLOAT>::value> :: type* = nullptr>
T_FLOAT get(data_str data)
{
    if(data.type == FLOAT)
    {
        return *(T_FLOAT*)data.value;
    }
    //else

    return std::numeric_limits<double>::quiet_NaN();
}

template <typename t, typename std::enable_if <std::is_same<t,T_STRING>::value> :: type* = nullptr>
std::string get(data_str data)
{
    std::ostringstream buff;
    switch(data.type)
    {
        case STRING:
            return (T_STRING*)data.value.pointer;
        case INT:
            buff<<data.value.i;
            return buff.str();
        case FLOAT:
            buff<<data.value.f;
            return buff.str();
        case BOOL:
            return data.value.b?"true":"false";
        case OBJECT:
            return STR_OBJECT;
        default:
            return "null";
    }
}

template <typename t, typename std::enable_if <std::is_same<t,T_OBJECT>::value> :: type* = nullptr>
T_OBJECT *get(data_str data)
{
    if(data.type == OBJECT)
    {
        return (T_OBJECT*)data.value;
    }
    //else

    return NULL;
}

#endif // DATA_H
