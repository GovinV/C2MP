#ifndef OBJECTUTIL_H_INCLUDED
#define OBJECTUTIL_H_INCLUDED

#include "Object.h"

template <typename t>
void setEntry(Object *object, const char *name, t value)
{
    data_str data = newData();
    set<t>(&data, value);
    object->setEntry(name, data);
}

template <typename t, typename std::enable_if <std::is_same<t,T_STRING>::value> :: type* = nullptr>
void setEntry(Object *object, const char *name, const char *value)
{
    data_str data = newData();
    set<t>(&data, value);
    object->setEntry(name, data);
}

#endif // OBJECTUTIL_H_INCLUDED
