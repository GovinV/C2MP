#ifndef GENERIC_OBJECT_H
#define GENERIC_OBJECT_H

#include "Data.h"
#include "GenericTable.h"

class GenericObject
{
    public:
        GenericObject();
        virtual ~GenericObject();

        virtual data_str getEntry(const char *name) = 0;
        virtual void setEntry(const char *name, data_str data) = 0;
        virtual bool hasEntry(const char *name) = 0;
        virtual void clear(void) = 0;
        virtual unsigned int size(void) = 0;

        virtual void startIteration(void) = 0;
        virtual dataElement iterate(void) = 0;

    protected:

    private:
};

#endif // GENERIC_OBJECT_H
