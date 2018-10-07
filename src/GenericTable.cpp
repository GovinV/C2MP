#include "GenericTable.h"

GenericTable::GenericTable()
{}

GenericTable::~GenericTable()
{}

char *copyString(const char *str)
{
    std::size_t strLength = strlen(str)+1;
    char *ret = new char[strLength];
    return strcpy(ret, str);
}

